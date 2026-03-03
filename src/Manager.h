#pragma once

#include "Project.h"
#include "Ptr.h"

#include "FrameRenderer.h"

#include "ToolManager.h"

#include "Player.h"

#include "AssetLoader.h"

#include "Ptr.h"

namespace FuncDoodle {
	class AnimationManager {
		public:
			AnimationManager(
				SharedPtr<ProjectFile> proj, AssetLoader* assetLoader);
			~AnimationManager();
			void RenderTimeline(bool prevEnabled);
			void RenderControls();

			const SharedPtr<ProjectFile> Proj() const { return m_Proj; }
			void SetProj(SharedPtr<ProjectFile> proj) {
				m_Proj = proj;
				m_Player->SetProj(proj);
			}
			AnimationPlayer* Player() const { return m_Player.get(); }
			void SetPlayer(AnimationPlayer* player) { m_Player.reset(player); }

		private:
			SharedPtr<ProjectFile> m_Proj;
			unsigned long m_SelectedFrame;
			UniquePtr<FrameRenderer> m_FrameRenderer;
			UniquePtr<FrameRenderer> m_TimelineFrameRenderer;
			UniquePtr<ToolManager> m_ToolManager;
			UniquePtr<AnimationPlayer> m_Player;
			AssetLoader* m_AssetLoader;
	};
}  // namespace FuncDoodle
