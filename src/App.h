#pragma once

#include "AssetLoader.h"
#include "Manager.h"
#include "Project.h"
#include "UUID.h"

#include <functional>

namespace FuncDoodle {
	class Application {
		public:
			Application(GLFWwindow* win, AssetLoader* assetLoader,
						std::filesystem::path themesPath);
			~Application();
			void RenderImGui();
			void OpenFileDialog(std::function<void()> done);
			void SaveFileDialog(std::function<void()> done);
			void ReadProjectFile();
			void SaveProjectFile();
			void CheckKeybinds(char* newProj, char* open, char* save,
							   char* exportShortcut, char* quit, char* pref,
							   char* themeEditorShortcut);
			void RenderOptions();
			void SaveChangesDialog();
			void OpenSaveChangesDialog();
			bool SaveChangesDialogOpen() { return m_SaveChangesOpen; }
			inline ProjectFile* CurProj() { return m_CurrentProj; }
			inline ProjectFile* CacheProj() { return m_CacheProj; }
			inline bool ShouldClose() { return m_ShouldClose; }
			inline UUID Theme() { return m_Theme; }
			inline void SetTheme(UUID theme) { m_Theme = theme; }
			inline void SetSFXEnabled(bool enabled) { m_SFXEnabled = enabled; }
			inline void SetPrevEnabled(bool enabled) { m_PrevEnabled = enabled; }
			inline bool PrevEnabled() { return m_PrevEnabled; }
			inline bool SFXEnabled() { return m_SFXEnabled; }
			void DropCallback(GLFWwindow* win, int count, const char** paths);
			void RenderEditProj();
			void RenderNewProj();
			void RenderMainMenuBar(char* newProjShortcut, char* openShortcut,
								   char* saveShortcut, char* exportShortcut,
								   char* quitShortcut, char* prefShortcut,
								   char* themeEditorShortcut);
			void RenderEditPrefs();
			void RenderExport();
			void RenderKeybinds();

		private:
			char* m_FilePath;
			bool m_NewProjOpen = false;
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
			UUID m_Theme;
			bool m_ShouldClose = false;
			bool m_SFXEnabled = true;
			bool m_PrevEnabled = false;
			float* m_CacheBGCol;
			std::filesystem::path m_ThemesPath;
	};
}  // namespace FuncDoodle
