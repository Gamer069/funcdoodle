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
            void CheckKeybinds(char* newProj, char* open, char* save, char* exportShortcut, char* quit);
            void RenderOptions();
        private:
            char* m_FilePath;
            bool m_NewProjOpen;
            ProjectFile* m_CurrentProj;
            ProjectFile* m_CacheProj;
            AnimationManager* m_Manager;
            bool m_EditProjOpen;
            GLFWwindow* m_Window;
            AssetLoader* m_AssetLoader;
            int m_ExportFormat = 0;
            bool m_ExportOpen = false;
            bool m_EditPrefsOpen = false;
            int m_Theme = 0;
    };
}
