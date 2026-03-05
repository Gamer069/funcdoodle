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
		if (!m_EditorController) {
			return;
		}
		EditorController::CanvasContext context;
		context.frame = m_Frame;
		context.previousFrame = m_PreviousFrame;
		context.index = m_Index;
		context.toolManager = m_ToolManager;
		context.player = m_Player;
		context.grid = m_Grid.get();
		context.pixelScale = &m_PixelScale;
		context.lastMousePos = &m_LastMousePos;
		context.frameI = frameI;
		context.prevEnabled = prevEnabled;
		m_EditorController->RenderCanvas(context);
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
