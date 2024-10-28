#pragma once

#include "Project.h"

#include "FrameRenderer.h"

#include "ToolManager.h"

#include "Player.h"

namespace FuncDoodle {
    class AnimationManager {
        public:
            AnimationManager(ProjectFile* proj);
            ~AnimationManager();
            void RenderTimeline();
            void RenderControls();

            const ProjectFile* Proj() const { return m_Proj; }
            void SetProj(ProjectFile* proj) { m_Proj = proj; }
            const AnimationPlayer* Player() const { return m_Player; }
            void SetPlayer(AnimationPlayer* player) { m_Player = player; }
        private:
            ProjectFile* m_Proj;
            long m_SelectedFrame;
            FrameRenderer* m_FrameRenderer;
            ToolManager* m_ToolManager;
            AnimationPlayer* m_Player;
    };
}
