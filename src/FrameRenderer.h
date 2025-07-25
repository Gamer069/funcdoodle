#pragma once

#include "Frame.h"

#include "Grid.h"
#include "ToolManager.h"

#include "Player.h"

#include <vector>
#include <memory>

namespace FuncDoodle {
	inline std::vector<std::pair<int, int>> i_PixelsChangedByBucketTool =
		std::vector<std::pair<int, int>>();
	class FrameRenderer {
		public:
			FrameRenderer(Frame* frame, ToolManager* manager,
						  AnimationPlayer* player)
				: m_Frame(frame), m_ToolManager(manager), m_Grid(nullptr),
				  m_Player(player) {
				if (frame != nullptr)
					m_FrameRT = std::make_unique<Frame>(*frame);
				else
					m_FrameRT = nullptr;
			}
			~FrameRenderer() {
				delete m_Grid;
				// m_FrameRT will be automatically cleaned up by unique_ptr
			}
			void RenderFrame(unsigned long frameI, bool prevEnabled);
			void InitPixels(unsigned long frameI, bool prevEnabled);

			inline const Frame* AnimFrame() const { return m_Frame; }
			void SetFrame(Frame* frame) {
				m_Frame = frame;
				if (frame != nullptr) {
					m_FrameRT = std::make_unique<Frame>(*frame);
				} else {
					m_FrameRT.reset();
				}
			}
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

			void FloodFill(int x, int y, Col targetCol, Col fillCol, Frame* targetFrame);
			void RenderFramePixels(int startX, int startY, ImDrawList* drawList,
								   bool usePrevPxScale = true,
								   bool renderPreview = true);

		private:
			std::unique_ptr<Frame> m_FrameRT;
			Frame* m_Frame;
			Frame* m_PreviousFrame;
			int m_Index;
			ToolManager* m_ToolManager;
			int m_PixelScale = 8;
			ImVec2 m_LastMousePos = ImVec2(-1, -1);
			Grid* m_Grid;
			AnimationPlayer* m_Player;
			ImVec2 m_LastHoverMousePos = ImVec2(-1, -1);
	};
}  // namespace FuncDoodle
