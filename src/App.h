#pragma once

#include "AssetLoader.h"
#include "EditorController.h"
#include "Keybinds.h"
#include "Manager.h"
#include "Project.h"
#include "Ptr.h"
#include "UUID.h"

#include <functional>
#include <string>

#include "Ptr.h"

namespace FuncDoodle {
	class Application {
		public:
			Application(GLFWwindow* win, AssetLoader* assetLoader,
				std::filesystem::path themesPath,
				std::filesystem::path rootPath);
			~Application();
			void RenderImGui();
			void OpenFileDialog(std::function<void()> done);
			void SaveFileDialog(std::function<void()> done);
			void ReadProjectFile();
			void SaveProjectFile();
			void CheckKeybinds();
			void RenderOptions();
			void SaveChangesDialog();
			void OpenSaveChangesDialog();
			bool SaveChangesDialogOpen() { return m_SaveChangesOpen; }
			inline SharedPtr<ProjectFile> CurProj() { return m_CurrentProj; }
			inline SharedPtr<ProjectFile> CacheProj() { return m_CacheProj; }
			inline bool ShouldClose() { return m_ShouldClose; }
			inline UUID Theme() { return m_Theme; }
			inline void SetTheme(UUID theme) { m_Theme = theme; }
			inline void SetSFXEnabled(bool enabled) { m_SFXEnabled = enabled; }
			inline void SetPrevEnabled(bool enabled) {
				m_PrevEnabled = enabled;
			}
			inline void SetUndoByStroke(bool enabled) {
				m_UndoByStroke = enabled;
				if (m_Manager) {
					m_Manager->SetUndoByStroke(enabled);
				}
			}
			inline bool PrevEnabled() { return m_PrevEnabled; }
			inline bool SFXEnabled() { return m_SFXEnabled; }
			inline bool UndoByStroke() { return m_UndoByStroke; }
			void DropCallback(GLFWwindow* win, int count, const char** paths);
			void RenderEditProj();
			void RenderNewProj();
			void RenderMainMenuBar();
			void RenderEditPrefs();
			void RenderRotate();
			void RenderExport();
			void RenderKeybinds();
			void Rotate(int32_t deg);

		private:
			std::string m_FilePath;
			int m_Deg = 90;
			bool m_NewProjOpen = false;
			SharedPtr<KeybindsRegistry> m_Keybinds =
				std::make_shared<KeybindsRegistry>();
			SharedPtr<ProjectFile> m_CurrentProj;
			SharedPtr<ProjectFile> m_CacheProj;
			UniquePtr<AnimationManager> m_Manager;
			GLFWwindow* m_Window;
			AssetLoader* m_AssetLoader;
			SharedPtr<EditorController> m_EditorController;
			int m_ExportFormat = 0;
			bool m_EditProjOpen = false;
			bool m_ExportOpen = false;
			bool m_EditPrefsOpen = false;
			bool m_ShowKeybindsOpen = false;
			bool m_SaveChangesOpen = false;
			bool m_RotateOpen = false;
			bool m_ShortcutsOpen = false;
			UUID m_Theme;
			bool m_ShouldClose = false;
			bool m_SFXEnabled = true;
			bool m_PrevEnabled = false;
			bool m_UndoByStroke = false;
			std::array<float, 3> m_CacheBGCol;
			std::filesystem::path m_ThemesPath;
	};
}  // namespace FuncDoodle
