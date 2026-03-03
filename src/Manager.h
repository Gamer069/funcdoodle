#pragma once

#include "Project.h"

#include "FrameRenderer.h"

#include "ToolManager.h"

#include "Player.h"

#include "AssetLoader.h"

namespace FuncDoodle {
	class AnimationManager {
		public:
			AnimationManager(
				std::shared_ptr<ProjectFile> proj, AssetLoader* assetLoader);
			~AnimationManager();
			void RenderTimeline(bool prevEnabled);
			void RenderControls();

			const std::shared_ptr<ProjectFile> Proj() const { return m_Proj; }
			void SetProj(std::shared_ptr<ProjectFile> proj) {
				m_Proj = proj;
				m_Player->SetProj(proj);
			}
			AnimationPlayer* Player() const { return m_Player; }
			void SetPlayer(AnimationPlayer* player) { m_Player = player; }

		private:
			std::shared_ptr<ProjectFile> m_Proj;
			unsigned long m_SelectedFrame;
			std::unique_ptr<FrameRenderer> m_FrameRenderer;
			std::unique_ptr<FrameRenderer> m_TimelineFrameRenderer;
			std::unique_ptr<ToolManager> m_ToolManager;
			AnimationPlayer* m_Player;
			AssetLoader* m_AssetLoader;
	};
}  // namespace FuncDoodle
