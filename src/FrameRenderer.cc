#include "FrameRenderer.h"

#include "Gui.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Frame.h"

#include <iostream>

#include <memory>
#include <utility>

#include "MacroUtils.h"

#include "Player.h"

#include <cmath>

namespace FuncDoodle {
	void FrameRenderer::RenderFrame(unsigned long frameI, bool prevEnabled) {
		ImGui::SetNextWindowPos(ImVec2(0, 32), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1073, 886), ImGuiCond_FirstUseEver);
		ImGui::Begin("Frame");

		if (!m_Frame || !m_ToolManager) {
			ImGui::End();
			return;
		}

		if (!m_Grid) {
			m_Grid.reset(new Grid(m_Frame->Width(), m_Frame->Height(),
				m_Player->Proj()->BgCol()));
		}

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("Zoom out", "-")) {
				m_PixelScale = std::max(1, m_PixelScale - 1);
			}
			if (ImGui::MenuItem("Zoom equal", "0")) {
				m_PixelScale = 1;
			}
			if (ImGui::MenuItem("Zoom in", "=")) {
				m_PixelScale++;
			}
			if (ImGui::MenuItem("Toggle Grid", "G")) {
				if (m_Grid->GridVisibility())
					m_Grid->HideGrid();
				else
					m_Grid->ShowGrid();
			}
			if (ImGui::MenuItem("Make grid width bigger", "Y")) {
				m_Grid->SetGridWidth(m_Grid->GridWidth() + 1);
				m_Grid->SetGridHeight(m_Grid->GridHeight() + 1);
			}
			if (ImGui::MenuItem("Make grid width smaller", "T")) {
				if (m_Grid->GridWidth() > 1)
					m_Grid->SetGridWidth(m_Grid->GridWidth() - 1);
				if (m_Grid->GridHeight() > 1)
					m_Grid->SetGridHeight(m_Grid->GridHeight() - 1);
			}
			ImGui::EndPopup();
		}
		InitPixels(frameI, prevEnabled);

		ImGui::End();
	}

	void FrameRenderer::InitPixels(unsigned long frameI, bool prevEnabled) {
		(void)prevEnabled;
		if (!m_Frame || !m_Player || !m_Player->Proj()) {
			return;
		}
		const ImageArray* pixels = m_Frame->Pixels();
		if (!pixels) {
			return;
		}

		// Get window dimensions before handling zoom
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();

		// Handle zoom input with window size constraints
		if (!ImGui::IsAnyItemActive() && ImGui::IsWindowFocused()) {
			if (ImGui::IsKeyPressed(ImGuiKey_Equal)) {
				m_PixelScale += 1;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Minus)) {
				m_PixelScale = std::max(1, m_PixelScale - 1);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_0)) {
				m_PixelScale = 1;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_T)) {
				if (m_Grid->GridWidth() > 1)
					m_Grid->SetGridWidth(m_Grid->GridWidth() - 1);
				if (m_Grid->GridHeight() > 1)
					m_Grid->SetGridHeight(m_Grid->GridHeight() - 1);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Y)) {
				m_Grid->SetGridWidth(m_Grid->GridWidth() + 1);
				m_Grid->SetGridHeight(m_Grid->GridHeight() + 1);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_G)) {
				if (m_Grid->GridVisibility())
					m_Grid->HideGrid();
				else
					m_Grid->ShowGrid();
			}
		}

		// Calculate total frame dimensions
		float frameWidth = pixels->Width() * m_PixelScale;
		float frameHeight = pixels->Height() * m_PixelScale;

		// Calculate starting position to center the frame
		float startX = windowPos.x + (contentRegion.x - frameWidth) * 0.5f + 9;
		float startY =
			windowPos.y + (contentRegion.y - frameHeight) * 0.5f + 41;

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Handle mouse input first
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 frameMin(startX, startY);
		ImVec2 frameMax(startX + frameWidth, startY + frameHeight);

		// Handle drawing conditions
		bool shouldDraw = ImGui::IsMouseDown(0);

		// Check if mouse is within frame bounds and conditions are met
		if (ImGui::IsMouseHoveringRect(frameMin, frameMax) && shouldDraw) {
			// Calculate current pixel coordinates
			ImVec2 currentPixel((mousePos.x - startX) / m_PixelScale,
				(mousePos.y - startY) / m_PixelScale);

			// Ensure pixel coordinates are within bounds
			if (currentPixel.x >= 0 && currentPixel.x < pixels->Width() &&
				currentPixel.y >= 0 && currentPixel.y < pixels->Height()) {

				int selectedTool = m_ToolManager->SelectedTool();

				// Check if frame window is focused
				ImGuiContext* ctx = ImGui::GetCurrentContext();
				ImGuiWindow* focusedWindow = ctx->NavWindow;
				bool isFrameWindowFocused =
					focusedWindow && strcmp(focusedWindow->Name, "Frame") == 0;

				if (isFrameWindowFocused && m_EditorController) {
					// Handle tool interpolation for smooth drawing
					if (m_LastMousePos.x >= 0 && m_LastMousePos.y >= 0 &&
						(selectedTool == 0 ||
							selectedTool == 1)) {  // Pencil or eraser

						float dx = currentPixel.x - m_LastMousePos.x;
						float dy = currentPixel.y - m_LastMousePos.y;
						int steps =
							std::max(1, std::max(static_cast<int>(std::abs(dx)),
											static_cast<int>(std::abs(dy))));

						for (int i = 0; i <= steps; i++) {
							float t = static_cast<float>(i) / steps;
							ImVec2 interpPixel(m_LastMousePos.x + dx * t,
								m_LastMousePos.y + dy * t);
							m_EditorController->Paint(m_Frame, frameI,
								m_ToolManager, m_Player,
								static_cast<int>(interpPixel.x),
								static_cast<int>(interpPixel.y), true, false);
						}
					} else {
						m_EditorController->Paint(m_Frame, frameI,
							m_ToolManager, m_Player,
							static_cast<int>(currentPixel.x),
							static_cast<int>(currentPixel.y), shouldDraw,
							ImGui::IsMouseClicked(0));
					}
				}
				m_LastMousePos = currentPixel;
			}
		} else if (!ImGui::IsMouseDown(0) ||
				   !ImGui::IsMouseHoveringRect(frameMin, frameMax)) {
			m_LastMousePos = ImVec2(-1, -1);
		}
		if (!ImGui::IsMouseDown(0) && m_EditorController) {
			m_EditorController->EndStroke(m_Player);
		}

		RenderFramePixels(startX, startY, drawList);

		// Draw the previous frame with transparency if available
		if (m_Index > 0 && m_PreviousFrame && m_PreviousFrame->Pixels() &&
			!m_Player->Playing()) {
			const ImageArray* prevPixels = m_PreviousFrame->Pixels();

			for (int y = 0; y < prevPixels->Height(); y++) {
				for (int x = 0; x < prevPixels->Width(); x++) {
					Col col = prevPixels->Get(x, y);
					Col curCol = pixels->Get(x, y);

					// Only draw previous frame where current frame is white
					// (transparent)
					if (curCol.r == 255 && curCol.g == 255 && curCol.b == 255) {
						ImVec2 topLeft(startX + x * m_PixelScale,
							startY + y * m_PixelScale);
						ImVec2 bottomRight(startX + (x + 1) * m_PixelScale,
							startY + (y + 1) * m_PixelScale);
						drawList->AddRectFilled(topLeft, bottomRight,
							IM_COL32(col.r, col.g, col.b, 128));
					}
				}
			}
		}

		if (m_Grid) {
			m_Grid->RenderWithDrawList(drawList, ImVec2(startX, startY),
				ImVec2(startX + frameWidth, startY + frameHeight));
		}
	}

	void FrameRenderer::RenderFramePixels(int startX, int startY,
		ImDrawList* drawList, bool usePrevPxScale, bool renderPreview) {
		// Use the appropriate frame for rendering
		const ImageArray* pixels = m_Frame->Pixels();

		for (int y = 0; y < pixels->Height(); y++) {
			for (int x = 0; x < pixels->Width(); x++) {
				Col col = pixels->Get(x, y);

				ImVec2 topLeft, bottomRight;
				if (usePrevPxScale) {
					topLeft = ImVec2(
						startX + x * m_PixelScale, startY + y * m_PixelScale);
					bottomRight = ImVec2(startX + (x + 1) * m_PixelScale,
						startY + (y + 1) * m_PixelScale);
				} else {
					topLeft = ImVec2(startX + x, startY + y);
					bottomRight = ImVec2(startX + x + 1, startY + y + 1);
				}

				drawList->AddRectFilled(
					topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, 255));
			}
		}
	}
}  // namespace FuncDoodle
