#pragma once

#include "Project.h"
#include "Manager.h"

namespace FuncDoodle {
    class Application {
        public:
            Application(GLFWwindow* win);
            ~Application();
            void RenderImGui();
            void OpenFileDialog();
            void SaveFileDialog();
            void ReadProjectFile();
            void SaveProjectFile();
            void CheckKeybinds(char* newProj, char* open, char* save);
        private:
            char* m_FilePath;
            bool m_NewProjOpen;
            ProjectFile* m_CurrentProj;
            ProjectFile* m_CacheProj;
            AnimationManager* m_Manager;
            GLFWwindow* m_Window;
    };
}
