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

namespace FuncDoodle {
	void FrameRenderer::RenderFrame(unsigned long frameI, bool prevEnabled) {
		ImGui::SetNextWindowPos(ImVec2(0, 32), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1073, 886), ImGuiCond_FirstUseEver);
		ImGui::Begin("Frame");

		if (!m_Frame || !m_ToolManager || !m_FrameRT->Pixels() || !m_FrameRT ||
			!m_FrameRT->Pixels())
			return;

		if (!m_Grid) {
			m_Grid = new Grid(m_Frame->Width(), m_Frame->Height(),
							  m_Player->Proj()->BgCol());
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
		InitPixels(frameI, prevEnabled);

		ImGui::End();
	}
	void FrameRenderer::InitPixels(unsigned long frameI, bool prevEnabled) {
		const ImageArray* pixels;
		if (prevEnabled)
			pixels = m_FrameRT->Pixels();
		else
			pixels = m_Frame->Pixels();

		// Get window dimensions before handling zoom
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();

		// Handle zoom input with window size constraints
		if (!ImGui::IsAnyItemActive() && ImGui::IsWindowFocused()) {
			if (ImGui::IsKeyPressed(ImGuiKey_Equal)) {	// = key for zoom in
				m_PixelScale += 1;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Minus)) {	// - key for zoom out
				m_PixelScale -= 1;
#ifdef _WIN32
				m_PixelScale = max(1, m_PixelScale);
#else
				m_PixelScale = std::max(1, m_PixelScale);
#endif
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
		unsigned char colNew[3] = {m_Player->Proj()->BgCol().r,
								   m_Player->Proj()->BgCol().g,
								   m_Player->Proj()->BgCol().b};

		auto pencil = [&](ImVec2 currentPixel) {
			// if (m_ToolManager->PickerSelected()) return;
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
					if (newX >= 0 && newX < m_Frame->Pixels()->Width() &&
						newY >= 0 && newY < m_Frame->Pixels()->Height()) {
						if (prevEnabled) {
							Col prevColor =
								m_FrameRT->Pixels()->Get(newX, newY);
							m_FrameRT->SetPixel(newX, newY,
												Col{.r = colNew[0],
													.g = colNew[1],
													.b = colNew[2]});
							if (ImGui::IsMouseDown(0)) {
								Col nextColor =
									m_FrameRT->Pixels()->Get(newX, newY);
								DrawAction action =
									DrawAction(newX, newY, prevColor, nextColor,
											   frameI, m_Player->Proj());
								if (prevColor != nextColor) {
									m_Player->Proj()->PushUndoableDrawAction(
										action);
								}
							}
						} else {
							Col prevColor = m_Frame->Pixels()->Get(newX, newY);
							m_Frame->SetPixel(newX, newY,
											  Col{.r = colNew[0],
												  .g = colNew[1],
												  .b = colNew[2]});
							if (ImGui::IsMouseDown(0)) {
								Col nextColor =
									m_Frame->Pixels()->Get(newX, newY);
								DrawAction action =
									DrawAction(newX, newY, prevColor, nextColor,
											   frameI, m_Player->Proj());
								if (prevColor != nextColor) {
									m_Player->Proj()->PushUndoableDrawAction(
										action);
								}
							}
						}
					}
				}
			}
		};
		auto eraser = [&](ImVec2 currentPixel) {
			// if (m_ToolManager->PickerSelected()) return;
			int size = m_ToolManager->Size();

			for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
				for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
					int newX = currentPixel.x + offsetX;
					int newY = currentPixel.y + offsetY;
					if (newX >= 0 && newX < m_Frame->Pixels()->Width() &&
						newY >= 0 && newY < m_Frame->Pixels()->Height()) {
						Col prevColor = m_FrameRT->Pixels()->Get(newX, newY);
						m_FrameRT->SetPixel(newX, newY,
											Col{.r = colNew[0],
												.g = colNew[1],
												.b = colNew[2]});
						Col nextColor = m_FrameRT->Pixels()->Get(newX, newY);
						DrawAction action =
							DrawAction(newX, newY, prevColor, nextColor, frameI,
									   m_Player->Proj());
						if (prevColor != nextColor) {
							m_Player->Proj()->PushUndoableDrawAction(action);
						}
					}
				}
			}
		};

		auto bucket = [&](ImVec2 currentPixel) {
			// if (m_ToolManager->PickerSelected()) return;
			const float* colOld = m_ToolManager->Col();
			unsigned char colResult[3];
			for (int j = 0; j < 3; j++) {
				colResult[j] =
					static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
			}
			// FLOODFILL
			Col curPixelCol =
				pixels->Get(floor(currentPixel.x), floor(currentPixel.y));
			FloodFill(
				currentPixel.x, currentPixel.y, curPixelCol,
				Col{.r = colResult[0], .g = colResult[1], .b = colResult[2]});

			FillAction action = FillAction(
				curPixelCol,
				Col{.r = colResult[0], .g = colResult[1], .b = colResult[2]},
				frameI, m_Player->Proj(), i_PixelsChangedByBucketTool);
			if (curPixelCol !=
				Col{.r = colResult[0], .g = colResult[1], .b = colResult[2]}) {
				m_Player->Proj()->PushUndoableFillAction(action);
			}

			colNew[0] = colResult[0];
			colNew[1] = colResult[1];
			colNew[2] = colResult[2];
		};
		auto picker = [&](ImVec2 currentPixel) {
			const Col& col = m_Frame->Pixels()->Get(floor(currentPixel.x),
													floor(currentPixel.y));
			m_ToolManager->SetCol(col);
			colNew[0] = (uint8_t)((float)col.r / 255.0f);
			colNew[1] = (uint8_t)((float)col.g / 255.0f);
			colNew[2] = (uint8_t)((float)col.b / 255.0f);
		};
		if (m_ToolManager == nullptr) {
			FUNC_FATAL("Tool manager is nullptr");
		}
		if (prevEnabled) {
			if (!ImGui::IsMouseDown(0)) {
				// hacky but whatever
				delete m_Frame;
				m_Frame = new Frame(m_FrameRT->PixelsMut());
				// *m_Frame = *m_FrameRT;
			} else {
				delete m_FrameRT;
				m_FrameRT = new Frame(m_Frame->PixelsMut());
				// *m_FrameRT = *m_Frame;
			}
		}

		bool cond = true;
		if (!prevEnabled)
			cond = ImGui::IsMouseDown(0);

		// Check if mouse is within frame bounds and mouse button is down
		if (ImGui::IsMouseHoveringRect(frameMin, frameMax) && cond) {
			// Calculate current pixel coordinates
			ImVec2 currentPixel((mousePos.x - startX) / m_PixelScale,
								(mousePos.y - startY) / m_PixelScale);

			// Draw at current position
			if (currentPixel.x >= 0 && currentPixel.x < pixels->Width() &&
				currentPixel.y >= 0 && currentPixel.y < pixels->Height()) {
				int selectedTool = m_ToolManager->SelectedTool();

				// check if other window is focused or not
				ImGuiContext* ctx = ImGui::GetCurrentContext();
				ImGuiWindow* focusedWindow = ctx->NavWindow;

				char* curName = (char*)"Frame";

				// If we have a valid last position, interpolate
				if (m_LastMousePos.x >= 0 && m_LastMousePos.y >= 0) {
					float dx = currentPixel.x - m_LastMousePos.x;
					float dy = currentPixel.y - m_LastMousePos.y;
#ifdef _WIN32
					int steps = max(1, max(abs(static_cast<int>(dx)), abs(static_cast<int>(dy))));
#else
					int steps = std::max(1, std::max(abs(static_cast<int>(dx)), abs(static_cast<int>(dy))));
#endif

					// Pre-calculate color values outside the loop
					if (focusedWindow &&
						strcmp(focusedWindow->Name, curName) == 0) {
						if (selectedTool == 0) {
							pencil(currentPixel);
						} else if (selectedTool == 1) {
							eraser(currentPixel);
						} else if (selectedTool == 2) {
							bucket(currentPixel);
						} else if (selectedTool == 3) {
							if (ImGui::IsMouseDown(0)) {
								picker(currentPixel);
								steps = 0;
							} else {
								steps = 0;
							}
						}
					}

					for (int i = 0; i <= steps; i++) {
						float t = static_cast<float>(i) / steps;
						int interpX =
							static_cast<int>(m_LastMousePos.x + dx * t);
						int interpY =
							static_cast<int>(m_LastMousePos.y + dy * t);

						if (interpX >= 0 && interpX < pixels->Width() &&
							interpY >= 0 && interpY < pixels->Height()) {
							if (prevEnabled)
								m_FrameRT->SetPixel(interpX, interpY,
													Col{.r = colNew[0],
														.g = colNew[1],
														.b = colNew[2]});
							else
								m_Frame->SetPixel(interpX, interpY,
												  Col{.r = colNew[0],
													  .g = colNew[1],
													  .b = colNew[2]});
						}
					}
				} else {
					// Draw single pixel if no last position
					unsigned char colNew[3] = {m_Player->Proj()->BgCol().r,
											   m_Player->Proj()->BgCol().g,
											   m_Player->Proj()->BgCol().b};
					if (focusedWindow &&
						strcmp(focusedWindow->Name, curName) == 0) {
						if (selectedTool == 0) {
							pencil(currentPixel);
						} else if (selectedTool == 1) {
							eraser(currentPixel);
						} else if (selectedTool == 2) {
							bucket(currentPixel);
						} else if (selectedTool == 3) {
							picker(currentPixel);
						}
					}

					if (prevEnabled &&
						selectedTool !=
							3) {  // Only draw if not using color picker
						m_FrameRT->SetPixel(currentPixel.x, currentPixel.y,
											Col{.r = colNew[0],
												.g = colNew[1],
												.b = colNew[2]});
					} else if (!prevEnabled) {
						m_Frame->SetPixel(currentPixel.x, currentPixel.y,
										  Col{.r = colNew[0],
											  .g = colNew[1],
											  .b = colNew[2]});
					}
				}
				m_LastMousePos = currentPixel;
			}
		} else if (!ImGui::IsMouseDown(0) ||
				   !ImGui::IsMouseHoveringRect(frameMin, frameMax)) {
			m_LastMousePos = ImVec2(-1, -1);
		}

		RenderFramePixels(startX, startY, drawList);

		// Draw the previous frame with transparency if we're not on the first
		// frame
		if (m_Index > 0 && m_PreviousFrame != nullptr && !m_Player->Playing()) {
			const ImageArray* prevPixels = m_PreviousFrame->Pixels();

			// Draw each pixel of the previous frame with 50% transparency
			for (int y = 0; y < prevPixels->Height(); y++) {
				for (int x = 0; x < prevPixels->Width(); x++) {
					Col col = prevPixels->Get(x, y);
					ImVec2 topLeft(startX + x * m_PixelScale,
								   startY + y * m_PixelScale);
					ImVec2 bottomRight(startX + (x + 1) * m_PixelScale,
									   startY + (y + 1) * m_PixelScale);
					Col curCol = pixels->Get(x, y);
					if (curCol.r == 255 && curCol.g == 255 && curCol.b == 255) {
						drawList->AddRectFilled(
							topLeft, bottomRight,
							IM_COL32(col.r, col.g, col.b, 128));
					}
				}
			}
		}

		m_Grid->RenderWithDrawList(
			drawList, ImVec2(startX, startY),
			ImVec2(startX + frameWidth, startY + frameHeight));
	}

	void FrameRenderer::FloodFill(int x, int y, Col targetCol, Col fillCol) {
		if (x < 0 || x >= m_FrameRT->Pixels()->Width() || y < 0 ||
			y >= m_FrameRT->Pixels()->Height())
			return;

		if (m_FrameRT->Pixels()->Get(x, y) != targetCol ||
			m_FrameRT->Pixels()->Get(x, y) == fillCol)
			return;

		i_PixelsChangedByBucketTool.emplace_back(x, y);

		m_FrameRT->SetPixel(x, y, fillCol);

		FloodFill(x + 1, y, targetCol, fillCol);
		FloodFill(x - 1, y, targetCol, fillCol);
		FloodFill(x, y + 1, targetCol, fillCol);
		FloodFill(x, y - 1, targetCol, fillCol);
	}
	void FrameRenderer::RenderFramePixels(int startX, int startY,
										  ImDrawList* drawList,
										  bool usePrevPxScale,
										  bool renderPreview) {
		const ImageArray* pixels = m_FrameRT->Pixels();
		for (int y = 0; y < pixels->Height(); y++) {
			for (int x = 0; x < pixels->Width(); x++) {
				if (pixels == nullptr) {
					FUNC_DBG("PX NULLPTR");
				}
				Col col = pixels->Get(x, y);
				ImVec2 topLeft;
				ImVec2 bottomRight;
				if (usePrevPxScale) {
					topLeft = ImVec2(startX + x * m_PixelScale,
									 startY + y * m_PixelScale);
					bottomRight = ImVec2(startX + (x + 1) * m_PixelScale,
										 startY + (y + 1) * m_PixelScale);
				} else {
					topLeft = ImVec2(startX + x, startY + y);
					bottomRight = ImVec2(startX + x + 1, startY + y + 1);
				}
				drawList->AddRectFilled(topLeft, bottomRight,
										IM_COL32(col.r, col.g, col.b, 255));
			}
		}
	}
}  // namespace FuncDoodle
