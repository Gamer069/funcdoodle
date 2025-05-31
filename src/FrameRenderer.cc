#include "FrameRenderer.h"

#include "Gui.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Frame.h"

#include <iostream>

#include <utility>

#include "MacroUtils.h"

#include "Player.h"

#include <cmath>
#include <stack>

namespace FuncDoodle {
	void FrameRenderer::RenderFrame(unsigned long frameI, bool prevEnabled) {
		ImGui::SetNextWindowPos(ImVec2(0, 32), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1073, 886), ImGuiCond_FirstUseEver);
		ImGui::Begin("Frame");

		if (!m_Frame || !m_ToolManager || !m_FrameRT || !m_FrameRT->Pixels()) {
			ImGui::End();
			return;
		}

		if (!m_Grid) {
			m_Grid = new Grid(m_Frame->Width(), m_Frame->Height(),
							  m_Player->Proj()->BgCol());
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
		const ImageArray* pixels;
		if (prevEnabled && m_FrameRT && m_FrameRT->Pixels())
			pixels = m_FrameRT->Pixels();
		else if (m_Frame && m_Frame->Pixels())
			pixels = m_Frame->Pixels();
		else
			return;

		// Get window dimensions before handling zoom
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
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
		float startY = windowPos.y + (contentRegion.y - frameHeight) * 0.5f + 41;

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Handle mouse input first
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 frameMin(startX, startY);
		ImVec2 frameMax(startX + frameWidth, startY + frameHeight);

		// Define tool functions
		auto pencil = [&](ImVec2 currentPixel) -> bool {
			const float* colOld = m_ToolManager->Col();
			unsigned char colNew[3] = {
				static_cast<unsigned char>(colOld[0] * 255.0f + 0.5f),
				static_cast<unsigned char>(colOld[1] * 255.0f + 0.5f),
				static_cast<unsigned char>(colOld[2] * 255.0f + 0.5f)
			};

			int size = m_ToolManager->Size();
			bool actionPerformed = false;

			Frame* targetFrame = prevEnabled ? m_FrameRT : m_Frame;
			if (!targetFrame || !targetFrame->Pixels()) return false;

			for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
				for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
					int newX = static_cast<int>(currentPixel.x) + offsetX;
					int newY = static_cast<int>(currentPixel.y) + offsetY;
					
					if (newX >= 0 && newX < targetFrame->Pixels()->Width() &&
						newY >= 0 && newY < targetFrame->Pixels()->Height()) {
						
						Col prevColor = targetFrame->Pixels()->Get(newX, newY);
						Col newColor = {colNew[0], colNew[1], colNew[2]};
						
						targetFrame->SetPixel(newX, newY, newColor);
						
						if (ImGui::IsMouseDown(0) && prevColor != newColor) {
							DrawAction action(newX, newY, prevColor, newColor, frameI, m_Player->Proj());
							m_Player->Proj()->PushUndoableDrawAction(action);
							actionPerformed = true;
						}
					}
				}
			}
			return actionPerformed;
		};

		auto eraser = [&](ImVec2 currentPixel) -> bool {
			int size = m_ToolManager->Size();
			bool actionPerformed = false;
			Col bgColor = m_Player->Proj()->BgCol();

			Frame* targetFrame = prevEnabled ? m_FrameRT : m_Frame;
			if (!targetFrame || !targetFrame->Pixels()) return false;

			for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
				for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
					int newX = static_cast<int>(currentPixel.x) + offsetX;
					int newY = static_cast<int>(currentPixel.y) + offsetY;
					
					if (newX >= 0 && newX < targetFrame->Pixels()->Width() &&
						newY >= 0 && newY < targetFrame->Pixels()->Height()) {
						
						Col prevColor = targetFrame->Pixels()->Get(newX, newY);
						targetFrame->SetPixel(newX, newY, bgColor);
						
						if (prevColor != bgColor) {
							DrawAction action(newX, newY, prevColor, bgColor, frameI, m_Player->Proj());
							m_Player->Proj()->PushUndoableDrawAction(action);
							actionPerformed = true;
						}
					}
				}
			}
			return actionPerformed;
		};

		auto bucket = [&](ImVec2 currentPixel) -> bool {
			const float* colOld = m_ToolManager->Col();
			unsigned char colResult[3] = {
				static_cast<unsigned char>(colOld[0] * 255.0f + 0.5f),
				static_cast<unsigned char>(colOld[1] * 255.0f + 0.5f),
				static_cast<unsigned char>(colOld[2] * 255.0f + 0.5f)
			};

			int pixelX = static_cast<int>(std::floor(currentPixel.x));
			int pixelY = static_cast<int>(std::floor(currentPixel.y));
			
			if (pixelX < 0 || pixelX >= pixels->Width() || 
				pixelY < 0 || pixelY >= pixels->Height()) {
				return false;
			}

			Col curPixelCol = pixels->Get(pixelX, pixelY);
			Col fillColor = {colResult[0], colResult[1], colResult[2]};
			
			if (curPixelCol == fillColor) {
				return false; // No change needed
			}

			// Clear the bucket tool change tracking
			i_PixelsChangedByBucketTool.clear();
			
			// Perform flood fill
			FloodFill(pixelX, pixelY, curPixelCol, fillColor);

			// Create fill action if pixels were changed
			if (!i_PixelsChangedByBucketTool.empty()) {
				FillAction action(curPixelCol, fillColor, frameI, m_Player->Proj(), i_PixelsChangedByBucketTool);
				m_Player->Proj()->PushUndoableFillAction(action);
				return true;
			}
			return false;
		};

		auto picker = [&](ImVec2 currentPixel) -> bool {
			int pixelX = static_cast<int>(std::floor(currentPixel.x));
			int pixelY = static_cast<int>(std::floor(currentPixel.y));
			
			if (pixelX < 0 || pixelX >= m_Frame->Pixels()->Width() || 
				pixelY < 0 || pixelY >= m_Frame->Pixels()->Height()) {
				return false;
			}

			const Col& col = m_Frame->Pixels()->Get(pixelX, pixelY);
			m_ToolManager->SetCol(col);
			return true;
		};

		// Handle frame swapping for preview mode
		if (prevEnabled) {
			if (!ImGui::IsMouseDown(0) && m_FrameRT) {
				m_Frame = std::move(m_FrameRT);
			} else if (ImGui::IsMouseDown(0) && m_Frame) {
				m_FrameRT = std::move(m_Frame);
			}
		}

		bool shouldDraw = prevEnabled || ImGui::IsMouseDown(0);

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
				bool isFrameWindowFocused = focusedWindow && 
					strcmp(focusedWindow->Name, "Frame") == 0;

				if (isFrameWindowFocused) {
					// Handle tool interpolation for smooth drawing
					if (m_LastMousePos.x >= 0 && m_LastMousePos.y >= 0 && 
						(selectedTool == 0 || selectedTool == 1)) { // Pencil or eraser
						
						float dx = currentPixel.x - m_LastMousePos.x;
						float dy = currentPixel.y - m_LastMousePos.y;
						int steps = std::max(1, std::max(static_cast<int>(std::abs(dx)), 
														  static_cast<int>(std::abs(dy))));

						for (int i = 0; i <= steps; i++) {
							float t = static_cast<float>(i) / steps;
							ImVec2 interpPixel(m_LastMousePos.x + dx * t,
											   m_LastMousePos.y + dy * t);

							switch (selectedTool) {
								case 0: pencil(interpPixel); break;
								case 1: eraser(interpPixel); break;
							}
						}
					} else {
						// Single operation for bucket fill and color picker
						switch (selectedTool) {
							case 0: pencil(currentPixel); break;
							case 1: eraser(currentPixel); break;
							case 2: 
								if (ImGui::IsMouseClicked(0)) {
									bucket(currentPixel);
								}
								break;
							case 3:
								if (ImGui::IsMouseClicked(0)) {
									picker(currentPixel);
								}
								break;
						}
					}
				}
				m_LastMousePos = currentPixel;
			}
		} else if (!ImGui::IsMouseDown(0) || !ImGui::IsMouseHoveringRect(frameMin, frameMax)) {
			m_LastMousePos = ImVec2(-1, -1);
		}

		RenderFramePixels(startX, startY, drawList);

		// Draw the previous frame with transparency if available
		if (m_Index > 0 && m_PreviousFrame && m_PreviousFrame->Pixels() && !m_Player->Playing()) {
			const ImageArray* prevPixels = m_PreviousFrame->Pixels();

			for (int y = 0; y < prevPixels->Height(); y++) {
				for (int x = 0; x < prevPixels->Width(); x++) {
					Col col = prevPixels->Get(x, y);
					Col curCol = pixels->Get(x, y);
					
					// Only draw previous frame where current frame is white (transparent)
					if (curCol.r == 255 && curCol.g == 255 && curCol.b == 255) {
						ImVec2 topLeft(startX + x * m_PixelScale, startY + y * m_PixelScale);
						ImVec2 bottomRight(startX + (x + 1) * m_PixelScale, startY + (y + 1) * m_PixelScale);
						drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, 128));
					}
				}
			}
		}

		if (m_Grid) {
			m_Grid->RenderWithDrawList(drawList, ImVec2(startX, startY),
									   ImVec2(startX + frameWidth, startY + frameHeight));
		}
	}

	void FrameRenderer::FloodFill(int x, int y, Col targetCol, Col fillCol) {
		if (!m_FrameRT || !m_FrameRT->Pixels()) return;

		// Use iterative approach to avoid stack overflow
		std::stack<std::pair<int, int>> pixelStack;
		pixelStack.push({x, y});

		while (!pixelStack.empty()) {
			auto [currentX, currentY] = pixelStack.top();
			pixelStack.pop();

			// Check bounds
			if (currentX < 0 || currentX >= m_FrameRT->Pixels()->Width() || 
				currentY < 0 || currentY >= m_FrameRT->Pixels()->Height()) {
				continue;
			}

			// Check if pixel needs to be filled
			Col currentCol = m_FrameRT->Pixels()->Get(currentX, currentY);
			if (currentCol != targetCol || currentCol == fillCol) {
				continue;
			}

			// Fill the pixel
			i_PixelsChangedByBucketTool.emplace_back(currentX, currentY);
			m_FrameRT->SetPixel(currentX, currentY, fillCol);

			// Add neighboring pixels to stack
			pixelStack.push({currentX + 1, currentY});
			pixelStack.push({currentX - 1, currentY});
			pixelStack.push({currentX, currentY + 1});
			pixelStack.push({currentX, currentY - 1});
		}
	}

	void FrameRenderer::RenderFramePixels(int startX, int startY, ImDrawList* drawList,
										  bool usePrevPxScale, bool renderPreview) {
		if (!m_FrameRT || !m_FrameRT->Pixels()) {
			FUNC_DBG("FrameRT or pixels are null");
			return;
		}

		const ImageArray* pixels = m_FrameRT->Pixels();
		
		for (int y = 0; y < pixels->Height(); y++) {
			for (int x = 0; x < pixels->Width(); x++) {
				Col col = pixels->Get(x, y);
				
				ImVec2 topLeft, bottomRight;
				if (usePrevPxScale) {
					topLeft = ImVec2(startX + x * m_PixelScale, startY + y * m_PixelScale);
					bottomRight = ImVec2(startX + (x + 1) * m_PixelScale, startY + (y + 1) * m_PixelScale);
				} else {
					topLeft = ImVec2(startX + x, startY + y);
					bottomRight = ImVec2(startX + x + 1, startY + y + 1);
				}
				
				drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, 255));
			}
		}
	}
}  // namespace FuncDoodle
