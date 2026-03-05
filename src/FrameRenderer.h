#pragma once

#include "Frame.h"

#include "Action.h"
#include "Grid.h"
#include "ToolManager.h"

#include "Player.h"

#include "Ptr.h"

#include <memory>

#include "EditorController.h"

namespace FuncDoodle {
	class FrameRenderer {
		public:
			FrameRenderer(Frame* frame, ToolManager* manager,
				AnimationPlayer* player,
				const SharedPtr<EditorController>& editorController)
				: m_Frame(frame), m_ToolManager(manager), m_Grid(nullptr),
				  m_Player(player), m_EditorController(editorController) {}
			~FrameRenderer() {}
			void RenderFrame(unsigned long frameI, bool prevEnabled);
			void InitPixels(unsigned long frameI, bool prevEnabled);

			inline const Frame* AnimFrame() const { return m_Frame; }
			void SetFrame(Frame* frame) { m_Frame = frame; }
			inline const ToolManager* GetToolManager() const {
				return m_ToolManager;
			}
			void SetToolManager(ToolManager* toolManager) {
				m_ToolManager = toolManager;
			}
			const int Index() const { return m_Index; }
			void SetIndex(int index) { m_Index = index; }
			void SetPreviousFrame(Frame* frame) { m_PreviousFrame = frame; }
			void SetPlayer(AnimationPlayer* player) { m_Player = player; }
			const Frame* PreviousFrame() const { return m_PreviousFrame; }
			inline void SetPixelScale(int pixelScale) {
				m_PixelScale = pixelScale;
			}
			void SetUndoByStroke(bool undoByStroke) {
				if (m_EditorController) {
					m_EditorController->SetUndoByStroke(undoByStroke, m_Player);
				}
			}
			void RenderFramePixels(int startX, int startY, ImDrawList* drawList,
				bool usePrevPxScale = true, bool renderPreview = true);

		private:
			Frame* m_Frame;
			Frame* m_PreviousFrame;
			SharedPtr<EditorController> m_EditorController;
			int m_Index;
			ToolManager* m_ToolManager;
			int m_PixelScale = 8;
			ImVec2 m_LastMousePos = ImVec2(-1, -1);
			UniquePtr<Grid> m_Grid;
			AnimationPlayer* m_Player;
			ImVec2 m_LastHoverMousePos = ImVec2(-1, -1);
	};
}  // namespace FuncDoodle
