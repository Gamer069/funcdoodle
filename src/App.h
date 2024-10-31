#pragma once

#include "Project.h"
#include "Manager.h"
#include "AssetLoader.h"

namespace FuncDoodle {
    class Application {
        public:
            Application(GLFWwindow* win, AssetLoader* assetLoader);
            ~Application();
            void RenderImGui();
            void OpenFileDialog();
            void SaveFileDialog();
            void ReadProjectFile();
            void SaveProjectFile();
            void CheckKeybinds(char* newProj, char* open, char* save);
            void RenderOptions();
        private:
            char* m_FilePath;
            bool m_NewProjOpen;
            ProjectFile* m_CurrentProj;
            ProjectFile* m_CacheProj;
            AnimationManager* m_Manager;
            GLFWwindow* m_Window;
            AssetLoader* m_AssetLoader;
    };
}
