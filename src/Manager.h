#pragma once

#include "Project.h"

#include "FrameRenderer.h"

#include "ToolManager.h"

#include "Player.h"

#include "AssetLoader.h"

namespace FuncDoodle {
	class AnimationManager {
		public:
			AnimationManager(ProjectFile* proj, AssetLoader* assetLoader);
			~AnimationManager();
			void RenderTimeline();
			void RenderControls();

			const ProjectFile* Proj() const { return m_Proj; }
			void SetProj(ProjectFile* proj) { m_Proj = proj; }
			AnimationPlayer* Player() const { return m_Player; }
			void SetPlayer(AnimationPlayer* player) { m_Player = player; }

		private:
			ProjectFile* m_Proj;
			long m_SelectedFrame;
			FrameRenderer* m_FrameRenderer;
			ToolManager* m_ToolManager;
			AnimationPlayer* m_Player;
			AssetLoader* m_AssetLoader;
	};
}  // namespace FuncDoodle
