#include "FrameRenderer.h"

#include "Gui.h"

#include "imgui_internal.h"

#include "Frame.h"

#include <iostream>

#include <utility>

#include "Player.h"

namespace FuncDoodle {
	void FrameRenderer::RenderFrame(long frameI) {
		ImGui::SetNextWindowPos(ImVec2(0, 32), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1073, 886), ImGuiCond_FirstUseEver);
		ImGui::Begin("Frame");

		if (!m_Frame || !m_ToolManager || !m_Frame->Pixels())
			return;

		if (!m_Grid) {
			m_Grid = new Grid(m_Frame->Width(), m_Frame->Height());
		}

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::MenuItem("Zoom out", "-")) {
				m_PixelScale--;
			}
			if (ImGui::MenuItem("Zoom equal", "0")) {
				m_PixelScale = 1;
			}
			if (ImGui::MenuItem("Zoom in", "=")) {
				m_PixelScale++;
			}
			if (ImGui::MenuItem("Toggle Grid", "G")) {
				// grid
				if (m_Grid->GridVisibility())
					m_Grid->HideGrid();
				else
					m_Grid->ShowGrid();
			}
			if (ImGui::MenuItem("Make grid width bigger", "Y")) {
				// grid
				m_Grid->SetGridWidth(m_Grid->GridWidth() + 1);
				m_Grid->SetGridHeight(m_Grid->GridHeight() + 1);
			}
			if (ImGui::MenuItem("Make grid width smaller", "T")) {
				// grid
				if (m_Grid->GridWidth() > 1)
					m_Grid->SetGridWidth(m_Grid->GridWidth() - 1);
				if (m_Grid->GridHeight() > 1)
					m_Grid->SetGridHeight(m_Grid->GridHeight() - 1);
			}
			ImGui::EndPopup();
		}
		InitPixels(frameI);

		ImGui::End();
	}
	void FrameRenderer::InitPixels(long frameI) {
		const ImageArray* pixels = m_Frame->Pixels();

		// Get window dimensions before handling zoom
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();

		// Handle zoom input with window size constraints
		if (!ImGui::IsAnyItemActive()) {
			if (ImGui::IsKeyPressed(ImGuiKey_Equal)) {	// = key for zoom in
				m_PixelScale += 1;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Minus)) {	// - key for zoom out
				m_PixelScale -= 1;
				m_PixelScale = std::max(1, m_PixelScale);  // Minimum of 1
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
			if (ImGui::IsKeyPressed(ImGuiKey_F)) {
				// fullscreen
			}
		}

		// Calculate total frame dimensions
		float frameWidth = pixels->getWidth() * m_PixelScale;
		float frameHeight = pixels->getHeight() * m_PixelScale;

		// Calculate starting position to center the frame
		float startX = windowPos.x + (contentRegion.x - frameWidth) * 0.5f + 9;
		float startY = windowPos.y + (contentRegion.y - frameHeight) * 0.5f + 41;

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Handle mouse input first
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 frameMin(startX, startY);
		ImVec2 frameMax(startX + frameWidth, startY + frameHeight);
		unsigned char colNew[3] = {255, 255, 255};

		auto pencil = [&](ImVec2 currentPixel) {
			const float* colOld = m_ToolManager->Col();
			for (int j = 0; j < 3; j++) {
				colNew[j] =
					static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
			}

			int size = m_ToolManager->Size();

			for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
				for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
					int newX = currentPixel.x + offsetX;
					int newY = currentPixel.y + offsetY;
					if (newX >= 0 && newX < m_Frame->Pixels()->getWidth() &&
						newY >= 0 && newY < m_Frame->Pixels()->getHeight()) {
						Col prevColor = m_Frame->Pixels()->get(newX, newY);
						m_Frame->SetPixel(newX, newY,
										  Col{.r = colNew[0],
											  .g = colNew[1],
											  .b = colNew[2]});
						Col nextColor = m_Frame->Pixels()->get(newX, newY);
						DrawAction action = DrawAction(newX, newY, prevColor, nextColor, frameI, m_Player->Proj());
						if (prevColor != nextColor) {
							m_Player->Proj()->PushUndoableDrawAction(action);
						}
					}
				}
			}
		};
		auto eraser = [&](ImVec2 currentPixel) {
			int size = m_ToolManager->Size();

			for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
				for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
					int newX = currentPixel.x + offsetX;
					int newY = currentPixel.y + offsetY;
					if (newX >= 0 && newX < m_Frame->Pixels()->getWidth() &&
						newY >= 0 && newY < m_Frame->Pixels()->getHeight()) {
						Col prevColor = m_Frame->Pixels()->get(newX, newY);
						m_Frame->SetPixel(newX, newY,
										  Col{.r = colNew[0],
											  .g = colNew[1],
											  .b = colNew[2]});
						Col nextColor = m_Frame->Pixels()->get(newX, newY);
						DrawAction action = DrawAction(newX, newY, prevColor, nextColor, frameI, m_Player->Proj());
						if (prevColor != nextColor) {
							m_Player->Proj()->PushUndoableDrawAction(action);
						}
					}
				}
			}
		};

		auto bucket = [&](ImVec2 currentPixel) {
			const float* colOld = m_ToolManager->Col();
			unsigned char colResult[3];
			for (int j = 0; j < 3; j++) {
				colResult[j] =
					static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
			}
			// FLOODFILL
			Col curPixelCol = pixels->get(currentPixel.x, currentPixel.y);
			FloodFill(
				currentPixel.x, currentPixel.y, curPixelCol,
				Col{.r = colResult[0], .g = colResult[1], .b = colResult[2]});

			FillAction action = FillAction(curPixelCol, Col{.r = colResult[0], .g = colResult[1], .b = colResult[2]}, frameI, m_Player->Proj(), i_PixelsChangedByBucketTool);
			if (curPixelCol != Col{.r = colResult[0], .g = colResult[1], .b = colResult[2]}) {
				m_Player->Proj()->PushUndoableFillAction(action);
			}

			colNew[0] = colResult[0];
			colNew[1] = colResult[1];
			colNew[2] = colResult[2];
		};
		auto picker = [&](ImVec2 currentPixel) {
			const Col& col = m_Frame->Pixels()->get(currentPixel.x, currentPixel.y);
			m_ToolManager->SetCol(col);
			colNew[0] = (float)col.r / 255.0f;
			colNew[1] = (float)col.g / 255.0f;
			colNew[2] = (float)col.b / 255.0f;
		};

		auto text = [&](ImVec2 currentPixel) {
			// TODO: implement this
			Col curPxCol =
				m_Frame->Pixels()->get(currentPixel.x, currentPixel.y);
			colNew[0] = curPxCol.r;
			colNew[1] = curPxCol.g;
			colNew[2] = curPxCol.b;
		};

		// Check if mouse is within frame bounds and mouse button is down
		if (ImGui::IsMouseHoveringRect(frameMin, frameMax) &&
				ImGui::IsMouseDown(0)) {
			if (m_ToolManager == nullptr) {
				std::cerr << "???????" << std::endl;
				std::exit(-1);
			}

			// Calculate current pixel coordinates
			ImVec2 currentPixel((mousePos.x - startX) / m_PixelScale,
					(mousePos.y - startY) / m_PixelScale);

			// Draw at current position
			if (currentPixel.x >= 0 && currentPixel.x < pixels->getWidth() &&
					currentPixel.y >= 0 && currentPixel.y < pixels->getHeight()) {
				int selectedTool = m_ToolManager->SelectedTool();

				// check if other window is focused or not
				ImGuiContext* ctx = ImGui::GetCurrentContext();
				ImGuiWindow* focusedWindow = ctx->NavWindow;

				char* curName = (char*)"Frame";

				// If we have a valid last position, interpolate
				if (m_LastMousePos.x >= 0 && m_LastMousePos.y >= 0) {
					float dx = currentPixel.x - m_LastMousePos.x;
					float dy = currentPixel.y - m_LastMousePos.y;
					int steps =
						std::max(1, std::max(abs(static_cast<int>(dx)),
									abs(static_cast<int>(dy))));

					// Pre-calculate color values outside the loop
					if (focusedWindow && strcmp(focusedWindow->Name, curName) == 0) {
						if (selectedTool == 0) {
							pencil(currentPixel);
						} else if (selectedTool == 1) {
							eraser(currentPixel);
						} else if (selectedTool == 2) {
							bucket(currentPixel);
						} else if (selectedTool == 3) {
							picker(currentPixel);
							steps = 0;
						} else if (selectedTool == 4) {
							text(currentPixel);
							steps = 0;
						}
					}

					for (int i = 0; i <= steps; i++) {
						float t = static_cast<float>(i) / steps;
						int interpX =
							static_cast<int>(m_LastMousePos.x + dx * t);
						int interpY =
							static_cast<int>(m_LastMousePos.y + dy * t);

						if (interpX >= 0 && interpX < pixels->getWidth() &&
								interpY >= 0 && interpY < pixels->getHeight()) {
							m_Frame->SetPixel(interpX, interpY,
									Col{.r = colNew[0],
									.g = colNew[1],
									.b = colNew[2]});
						}
					}
				} else {
					// Draw single pixel if no last position
					unsigned char colNew[3] = {255, 255, 255};	 // Default white for eraser
					if (focusedWindow && strcmp(focusedWindow->Name, curName) == 0) {
						if (selectedTool == 0) {
							pencil(currentPixel);
						} else if (selectedTool == 1) {
							eraser(currentPixel);
						} else if (selectedTool == 2) {
							bucket(currentPixel);
						} else if (selectedTool == 3) {
							picker(currentPixel);
						} else if (selectedTool == 4) {
							text(currentPixel);
						}
					}

					if (selectedTool !=
							3) {  // Only draw if not using color picker
						m_Frame->SetPixel(currentPixel.x, currentPixel.y,
								Col{.r = colNew[0],
								.g = colNew[1],
								.b = colNew[2]});
					}
				}
				m_LastMousePos = currentPixel;
			}
		} else if (!ImGui::IsMouseDown(0) || !ImGui::IsMouseHoveringRect(frameMin, frameMax)) {
			m_LastMousePos = ImVec2(-1, -1);
		}

		for (int y = 0; y < pixels->getHeight(); y++) {
			for (int x = 0; x < pixels->getWidth(); x++) {
				if (pixels == nullptr) {
					std::cout << "PX NULLPTR" << std::endl;
				}
				Col col = pixels->get(x, y);
				ImVec2 topLeft(startX + x * m_PixelScale,
						startY + y * m_PixelScale);
				ImVec2 bottomRight(startX + (x + 1) * m_PixelScale,
						startY + (y + 1) * m_PixelScale);
				drawList->AddRectFilled(topLeft, bottomRight,
						IM_COL32(col.r, col.g, col.b, 255));
			}
		}

		// Draw the previous frame with transparency if we're not on the first
		// frame
		if (m_Index > 0 && m_PreviousFrame != nullptr && !m_Player->Playing()) {
			const ImageArray* prevPixels = m_PreviousFrame->Pixels();

			// Draw each pixel of the previous frame with 50% transparency
			for (int y = 0; y < prevPixels->getHeight(); y++) {
				for (int x = 0; x < prevPixels->getWidth(); x++) {
					Col col = prevPixels->get(x, y);
					ImVec2 topLeft(startX + x * m_PixelScale,
							startY + y * m_PixelScale);
					ImVec2 bottomRight(startX + (x + 1) * m_PixelScale,
							startY + (y + 1) * m_PixelScale);
					Col curCol = pixels->get(x, y);
					if (curCol.r == 255 && curCol.g == 255 && curCol.b == 255)
						drawList->AddRectFilled(
								topLeft, bottomRight,
								IM_COL32(col.r, col.g, col.b, 128));
				}
			}
		}

		m_Grid->RenderWithDrawList(
			drawList, ImVec2(startX, startY),
			ImVec2(startX + frameWidth, startY + frameHeight));
	}

	void FrameRenderer::FloodFill(int x, int y, Col targetCol, Col fillCol) {
		if (x < 0 || x >= m_Frame->Pixels()->getWidth() || y < 0 ||
			y >= m_Frame->Pixels()->getHeight())
			return;

		if (m_Frame->Pixels()->get(x, y) != targetCol ||
			m_Frame->Pixels()->get(x, y) == fillCol)
			return;

		i_PixelsChangedByBucketTool.emplace_back(x, y);

		m_Frame->SetPixel(x, y, fillCol);

		FloodFill(x + 1, y, targetCol, fillCol);
		FloodFill(x - 1, y, targetCol, fillCol);
		FloodFill(x, y + 1, targetCol, fillCol);
		FloodFill(x, y - 1, targetCol, fillCol);
	}
}  // namespace FuncDoodle
