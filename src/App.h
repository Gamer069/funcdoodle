#pragma once

#include "AssetLoader.h"
#include "Manager.h"
#include "Project.h"

#include <functional>

namespace FuncDoodle {
	class Application {
		public:
			Application(GLFWwindow* win, AssetLoader* assetLoader);
			~Application();
			void RenderImGui();
			void OpenFileDialog(std::function<void()> done);
			void SaveFileDialog(std::function<void()> done);
			void ReadProjectFile();
			void SaveProjectFile();
			void CheckKeybinds(char* newProj, char* open, char* save,
							   char* exportShortcut, char* quit, char* pref);
			void RenderOptions();
			void SaveChangesDialog();
			void OpenSaveChangesDialog();
			bool SaveChangesDialogOpen() { return m_SaveChangesOpen; }
			static void CustomStyle();
			static void CatppuccinMochaStyle();
			static void CatppuccinMacchiatoStyle();
			static void CatppuccinFrappeStyle();
			static void CatppuccinLatteStyle();
			inline ProjectFile* CurProj() { return m_CurrentProj; }
			inline ProjectFile* CacheProj() { return m_CacheProj; }
			inline bool ShouldClose() { return m_ShouldClose; }
			void DropCallback(GLFWwindow* win, int count, const char** paths);
			void RenderEditProj();
			void RenderNewProj();
			void RenderMainMenuBar(char* newProjShortcut, char* openShortcut, char* saveShortcut, char* exportShortcut, char* quitShortcut, char* prefShortcut);
			void RenderEditPrefs();
			void RenderExport();
			void RenderKeybinds();
		private:
			char* m_FilePath;
			bool m_NewProjOpen;
			ProjectFile* m_CurrentProj;
			ProjectFile* m_CacheProj;
			AnimationManager* m_Manager;
			GLFWwindow* m_Window;
			AssetLoader* m_AssetLoader;
			int m_ExportFormat = 0;
			bool m_EditProjOpen = false;
			bool m_ExportOpen = false;
			bool m_EditPrefsOpen = false;
			bool m_ShowKeybindsOpen = false;
			bool m_SaveChangesOpen = false;
			bool m_ShouldClose = false;
			int m_Theme = 0;
			int m_MigrationProjVersion = 0;
			bool m_SFXEnabled = true;
			bool m_PrevEnabled = false;
			float* m_CacheBGCol;
	};
}  // namespace FuncDoodle
