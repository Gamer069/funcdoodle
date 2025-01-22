#pragma once

#include "AssetLoader.h"
#include "Manager.h"
#include "Project.h"

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
			void CheckKeybinds(char* newProj, char* open, char* save,
							   char* exportShortcut, char* quit);
			void RenderOptions();
			void SaveChangesDialog();
			static void CustomStyle();
			inline ProjectFile* CurProj() { return m_CurrentProj; }
			inline ProjectFile* CacheProj() { return m_CacheProj; }
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
			bool m_ShowKeybindsOpen = false;
			int m_Theme = 0;
	};
}  // namespace FuncDoodle
