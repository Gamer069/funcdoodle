#pragma once

#include "Frame.h"

#include "ToolManager.h"
#include "Grid.h"

#include "Player.h"

namespace FuncDoodle {
	class FrameRenderer {
		public:
			FrameRenderer(Frame* frame, ToolManager* manager, AnimationPlayer* player) : m_Frame(frame), m_ToolManager(manager), m_Grid(nullptr), m_Player(player) {}
			~FrameRenderer() { 
				delete m_Grid;
			;}
			void RenderFrame();
			void InitPixels();

			inline const Frame* AnimFrame() const { return m_Frame; }
			void SetFrame(Frame* frame) { m_Frame = frame; }
			inline const ToolManager* GetToolManager() const { return m_ToolManager; }
			void SetToolManager(ToolManager* toolManager) { m_ToolManager = toolManager; }
			const int Index() const { return m_Index; }
			void SetIndex(int index) { m_Index = index; }
			void SetPreviousFrame(Frame* frame) { m_PreviousFrame = frame; }
			const Frame* PreviousFrame() const { return m_PreviousFrame; }

			void FloodFill(int x, int y, Col targetCol, Col fillCol);
		private:
			Frame* m_Frame;
			Frame* m_PreviousFrame;
			int m_Index;
			ToolManager* m_ToolManager;
			int m_PixelScale = 8;
			ImVec2 m_LastMousePos = ImVec2(-1, -1);
			Grid* m_Grid;
			AnimationPlayer* m_Player;
	};
}
