#pragma once

#include "Project.h"
#include "Manager.h"

namespace FuncDoodle {
    class Application {
        public:
            Application();
            ~Application();
            void RenderImGui();
            void OpenFileDialog();
            void SaveFileDialog();
            void ReadProjectFile();
            void SaveProjectFile();
        private:
            char* m_FilePath;
            bool m_NewProjOpen;
            ProjectFile* m_CurrentProj;
            ProjectFile* m_CacheProj;
            AnimationManager* m_Manager;
    };
}
