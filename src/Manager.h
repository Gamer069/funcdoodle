#pragma once

#include "Project.h"

namespace FuncDoodle {
    class AnimationManager {
        public:
            AnimationManager(ProjectFile* proj);
            ~AnimationManager();
            void RenderTimeline();

            const ProjectFile* Proj() const { return m_Proj; }
            void SetProj(ProjectFile* proj) { m_Proj = proj; }
        private:
            ProjectFile* m_Proj;
            long m_SelectedFrame;
    };
}
