#include "Manager.h"

#include "Gui.h"

#include "Project.h"

#include "DynArr.h"

#include "FrameRenderer.h"

#include "ToolManager.h"

#include <iostream>
#include <string>

#include "LoadedAssets.h"

namespace FuncDoodle {
	AnimationManager::AnimationManager(ProjectFile* proj,
									   AssetLoader* assetLoader)
		: m_Proj(proj), m_SelectedFrame(0), m_Player(new AnimationPlayer(proj)),
		  m_AssetLoader(assetLoader) {
		m_ToolManager = new ToolManager(assetLoader);
		m_FrameRenderer = new FrameRenderer(nullptr, m_ToolManager, m_Player);
		m_TimelineFrameRenderer =
			new FrameRenderer(nullptr, m_ToolManager, m_Player);
	}
	AnimationManager::~AnimationManager() {
		delete m_ToolManager;
		delete m_FrameRenderer;
		delete m_TimelineFrameRenderer;
		delete m_Player;
	}
	void AnimationManager::RenderTimeline(bool prevEnabled) {
		// Set scrollbar size (thickness)
		ImGui::GetStyle().ScrollbarSize =
			20.0f;	// Increase the thickness of the scrollbars

		// Lock window height but allow horizontal resizing
		float fixedHeight = 160.0f;
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, fixedHeight),
											ImVec2(FLT_MAX, fixedHeight));

		// Begin the window with horizontal scrollbar enabled
		ImGui::SetNextWindowPos(ImVec2(0, 920), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1074, 160), ImGuiCond_FirstUseEver);
		ImGui::Begin("Timeline", nullptr,
					 ImGuiWindowFlags_HorizontalScrollbar |
						 ImGuiWindowFlags_NoBackground);
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Define frame size and padding
		// float frameWidth = 200.0f;
		// float frameHeight = 100.0f;
		float frameWidth = (float)m_Proj->AnimWidth();
		float frameHeight = (float)m_Proj->AnimHeight();
		float padding = 25.0f;

		// Calculate total width required for all frames
		float totalWidth = m_Proj->AnimFrameCount() * (frameWidth + padding);

		// Create a scrollable region
		ImGui::BeginChild(
			"FrameScrollRegion",
			ImVec2(ImGui::GetContentRegionAvail().x, frameHeight + padding),
			false, ImGuiWindowFlags_HorizontalScrollbar);

		// Get the initial top-left position
		ImVec2 topLeft = ImGui::GetCursorScreenPos();
		ImVec2 bottomRight =
			ImVec2(topLeft.x + frameWidth, topLeft.y + frameHeight);

		ImFont* font = ImGui::GetFont();
		float fontSize = ImGui::GetFontSize();

		if (!ImGui::IsAnyItemActive()) {
			if (ImGui::IsKeyPressed(ImGuiKey_LeftBracket, true)) {
				if (m_SelectedFrame > 0)
					m_SelectedFrame--;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_RightBracket, true)) {
				if (m_SelectedFrame < m_Proj->AnimFrameCount() - 1)
					++m_SelectedFrame;
			}
			const int X1 = 3;
			const int X2 = 4;
			if (ImGui::IsKeyPressed(ImGuiKey_E, true) ||
				ImGui::IsMouseClicked(X1)) {
				m_Proj->Undo();
			}
			if (ImGui::IsKeyPressed(ImGuiKey_R, true) ||
				ImGui::IsMouseClicked(X2)) {
				m_Proj->Redo();
			}
			if (ImGui::IsKeyPressed(ImGuiKey_P, true)) {
				m_Proj->AnimFrames()->InsertAfterEmpty(m_SelectedFrame);
				InsertFrameAction action =
					InsertFrameAction(m_SelectedFrame + 1, m_Proj);
				m_Proj->PushUndoableInsertFrameAction(action);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_O, true)) {
				m_Proj->AnimFrames()->InsertBeforeEmpty(m_SelectedFrame);
				m_SelectedFrame++;
				InsertFrameAction action =
					InsertFrameAction(m_SelectedFrame - 1, m_Proj);
				m_Proj->PushUndoableInsertFrameAction(action);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_I, true)) {
				m_Proj->AnimFrames()->MoveForward(m_SelectedFrame);
				m_SelectedFrame++;
				MoveFrameRightAction action =
					MoveFrameRightAction(m_SelectedFrame, m_Proj);
				m_Proj->PushUndoableMoveFrameRightAction(action);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_U, true)) {
				if (m_SelectedFrame != 0) {
					m_Proj->AnimFrames()->MoveBackward(m_SelectedFrame);
					MoveFrameLeftAction action =
						MoveFrameLeftAction(m_SelectedFrame, m_Proj);
					m_SelectedFrame--;
					m_Proj->PushUndoableMoveFrameLeftAction(action);
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Backslash, true)) {
				if (m_Proj->AnimFrameCount() != 1) {
					Frame deletedFrame =
						*m_Proj->AnimFrames()->Get(m_SelectedFrame);
					m_Proj->AnimFrames()->Remove(m_SelectedFrame);
					DeleteFrameAction action = DeleteFrameAction(
						m_SelectedFrame, &deletedFrame, m_Proj);
					m_Proj->PushUndoableDeleteFrameAction(action);
				}
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Comma, true)) {
				m_Proj->AnimFrames()->Get(m_SelectedFrame)->CopyToClipboard();
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Period, true)) {
				// paste before
				Frame* frame = Frame::PastedFrame();
				m_Proj->AnimFrames()->InsertBefore(m_SelectedFrame, frame);
				m_SelectedFrame++;
			}
			if (ImGui::IsKeyPressed(ImGuiKey_Slash, true)) {
				// paste after
				Frame* frame = Frame::PastedFrame();
				m_Proj->AnimFrames()->InsertAfter(m_SelectedFrame, frame);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_M, true)) {
				Frame* frame =
					new Frame(*m_Proj->AnimFrames()->Get(m_SelectedFrame));
				m_Proj->AnimFrames()->InsertAfter(m_SelectedFrame, frame);
				InsertFrameAction action =
					InsertFrameAction(m_SelectedFrame + 1, m_Proj);
				m_Proj->PushUndoableInsertFrameAction(action);
			}
			if (ImGui::IsKeyPressed(ImGuiKey_N, true)) {
				Frame* frame =
					new Frame(*m_Proj->AnimFrames()->Get(m_SelectedFrame));
				m_Proj->AnimFrames()->InsertBefore(m_SelectedFrame, frame);
				m_SelectedFrame++;
				InsertFrameAction action =
					InsertFrameAction(m_SelectedFrame - 1, m_Proj);
				m_Proj->PushUndoableInsertFrameAction(action);
			}
		}
		if (m_SelectedFrame >= m_Proj->AnimFrameCount()) {
			m_SelectedFrame = m_Proj->AnimFrameCount() - 1;
		}

		// Render frames
		for (unsigned long i = 0; i < m_Proj->AnimFrameCount(); i++) {
			drawList->AddText(
				font, fontSize,
				m_SelectedFrame == i
					? ImVec2(topLeft.x + frameWidth / 2, bottomRight.y + 10)
					: ImVec2(topLeft.x + frameWidth / 2, bottomRight.y),
				IM_COL32(255, 255, 255, 255), std::to_string(i).c_str());

			if (m_TimelineFrameRenderer->AnimFrame() != m_Proj->AnimFrames()->Get(i)) {
				m_TimelineFrameRenderer->SetFrame(m_Proj->AnimFrames()->Get(i));
			}

			float width = bottomRight.x - topLeft.x;
			float height = bottomRight.y - topLeft.y;
			float scaleX = width / frameWidth;
			float scaleY = width / frameHeight;
			m_TimelineFrameRenderer->SetPixelScale(std::min<float>(scaleX, scaleY));
			m_TimelineFrameRenderer->RenderFramePixels(
				topLeft.x, topLeft.y, ImGui::GetWindowDrawList(), true);

			if ((m_Player->Playing() && m_Player->CurFrame() == i) ||
				(!m_Player->Playing() && m_SelectedFrame == i)) {
				const auto frames = m_Proj->AnimFrames();
				if (m_FrameRenderer->AnimFrame() != frames->Get(i))
					m_FrameRenderer->SetFrame(frames->Get(i));
				m_FrameRenderer->SetIndex(i);
				if (i > 0) {
					m_FrameRenderer->SetPreviousFrame(frames->Get(i - 1));
				}
				m_FrameRenderer->RenderFrame(i, prevEnabled);
				drawList->AddRect(
					topLeft, ImVec2(bottomRight.x, bottomRight.y),
					IM_COL32(255, 0, 0, 255),  // Red color
					0.0f,					   // rounding
					0,						   // flags
					8.0f  // thickness - increased to make it much thicker
				);
			}
			ImVec2 mousePos = ImGui::GetMousePos();
			bool isHovered =
				(mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
				 mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y);

			char menuName[32];	// Make buffer big enough for "frame" + numbers
								// + "menu" + null terminator
			snprintf(menuName, 31, "##frame%ldmenu", i);
			char* menuNamePtr = menuName;

			// Create unique ID for this popup
			if (isHovered && ImGui::IsMouseClicked(1)) {
				ImGui::OpenPopup(menuNamePtr);
			} else if (isHovered && ImGui::IsMouseClicked(0)) {
				m_SelectedFrame = i;
			}

			if (ImGui::BeginPopup(menuNamePtr)) {
				if (ImGui::MenuItem("Delete", "\\")) {
					if (m_Proj->AnimFrameCount() != 1) {
						Frame deletedFrame =
							*m_Proj->AnimFrames()->Get(m_SelectedFrame);
						m_Proj->AnimFrames()->Remove(m_SelectedFrame);
						DeleteFrameAction action = DeleteFrameAction(
							m_SelectedFrame, &deletedFrame, m_Proj);
						m_Proj->PushUndoableDeleteFrameAction(action);
						m_Proj->AnimFrames()->Remove(i);
					}
				}
				if (ImGui::MenuItem("Insert before", "O")) {
					m_Proj->AnimFrames()->InsertBeforeEmpty(m_SelectedFrame);
					m_SelectedFrame++;
					InsertFrameAction action =
						InsertFrameAction(m_SelectedFrame - 1, m_Proj);
					m_Proj->PushUndoableInsertFrameAction(action);
				}
				if (ImGui::MenuItem("Insert after", "P")) {
					m_Proj->AnimFrames()->InsertAfterEmpty(m_SelectedFrame);
					InsertFrameAction action =
						InsertFrameAction(m_SelectedFrame + 1, m_Proj);
					m_Proj->PushUndoableInsertFrameAction(action);
				}
				if (ImGui::MenuItem("Move forward", "I")) {
					m_Proj->AnimFrames()->MoveForward(i);
				}
				if (ImGui::MenuItem("Move backward", "U")) {
					m_Proj->AnimFrames()->MoveBackward(i);
				}
				if (ImGui::MenuItem("Copy", ",")) {
					m_Proj->AnimFrames()->Get(i)->CopyToClipboard();
				}
				if (ImGui::MenuItem("Paste before", ".")) {
					Frame* frame = Frame::PastedFrame();
					m_Proj->AnimFrames()->InsertBefore(i, frame);
				}
				if (ImGui::MenuItem("Paste after", "/")) {
					Frame* frame = Frame::PastedFrame();
					m_Proj->AnimFrames()->InsertAfter(i, frame);
				}
				ImGui::EndPopup();
			}

			topLeft.x += frameWidth + padding;
			bottomRight.x += frameWidth + padding;
		}

		// Ensure the scroll region size is based on total width of all frames
		ImGui::Dummy(ImVec2(totalWidth - 25, frameHeight));

		ImGui::EndChild();
		ImGui::End();

		m_ToolManager->RenderTools();
	}

	void AnimationManager::RenderControls() {
		ImGui::Begin("Controls");

		if (ImGui::ImageButton("rewind", (ImTextureID)(intptr_t)s_RewindTexId,
							   ImVec2(20, 20)) ||
			(ImGui::IsKeyPressed(ImGuiKey_J, true) &&
			 !ImGui::IsAnyItemActive())) {
			m_SelectedFrame = 0;
			m_Player->Rewind();
		}

		ImGui::SameLine();

		if (ImGui::ImageButton("togglePlay",
							   m_Player->Playing()
								   ? (ImTextureID)(intptr_t)s_PauseTexId
								   : (ImTextureID)(intptr_t)s_PlayTexId,
							   ImVec2(20, 20)) ||
			(ImGui::IsKeyPressed(ImGuiKey_K, true) &&
			 !ImGui::IsAnyItemActive())) {
			m_Player->SetPlaying(!m_Player->Playing());
		}

		ImGui::SameLine();

		if (ImGui::ImageButton("end", (ImTextureID)(intptr_t)s_EndTexId,
							   ImVec2(20, 20)) ||
			(ImGui::IsKeyPressed(ImGuiKey_L, true) &&
			 !ImGui::IsAnyItemActive())) {
			m_SelectedFrame = m_Proj->AnimFrameCount() - 1;
			m_Player->End();
		}

		ImGui::End();
	}
}  // namespace FuncDoodle
