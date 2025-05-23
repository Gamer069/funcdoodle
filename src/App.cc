#include "Gui.h"
#include "Manager.h"

#include "App.h"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <imgui.h>
#include <iostream>
#include <stdint.h>
#include <string.h>

#include <fstream>

#include "LoadedAssets.h"

#include "MacroUtils.h"

#include "Themes.h"
#include "nfd.h"

namespace FuncDoodle {
	Application::Application(GLFWwindow* win, AssetLoader* assetLoader,
							 std::filesystem::path themesPath)
		: m_FilePath(nullptr), m_NewProjOpen(false), m_CurrentProj(nullptr),
		  m_CacheProj(nullptr),
		  m_Manager(new AnimationManager(nullptr, assetLoader)), m_Window(win),
		  m_AssetLoader(assetLoader), m_CacheBGCol(new float[3]{255, 255, 255}),
		  m_ThemesPath(themesPath), m_Theme(UUID::FromString("d0c1a009-d09c-4fe6-84f8-eddcb2da38f9")) {}
	Application::~Application() {
		delete m_Manager;
		delete m_CurrentProj;
		delete[] m_CacheBGCol;
	}
	char* GlobalGetShortcut(const char* key, bool shift, bool super) {
		int maxLen = 11 + strlen(key);

		char* shortcut = (char*)malloc(maxLen);

#if defined(_WIN32) || defined(__linux__)
		strcpy(shortcut, "Ctrl");
#else
		strcpy(shortcut, "Cmd");
#endif

		if (shift) {
			strcat(shortcut, "+Shift");
		}

		if (super) {
			strcat(shortcut, "+Super");
		}

		strcat(shortcut, "+");
		strcat(shortcut, key);

		return shortcut;
	}

	void Application::CheckKeybinds(char* newProj, char* open, char* save,
									char* exportShortcut, char* quit,
									char* pref, char* themeEditorShortcut) {
		ImGuiIO& io = ImGui::GetIO();

		struct Shortcut {
				bool requiresCtrl;
				bool requiresShift;
				bool requiresSuper;
				ImGuiKey key;
		};

		auto parseShortcut = [](const char* shortcut) -> Shortcut {
			Shortcut result = {false, false, false, ImGuiKey_None};

			// Determine platform-specific "Ctrl" vs "Cmd" modifier
#if defined(_WIN32) || defined(__linux__)
			result.requiresCtrl = std::strstr(shortcut, "Ctrl") != nullptr;
#else
			result.requiresCtrl = std::strstr(shortcut, "Cmd") != nullptr;
#endif
			result.requiresShift = std::strstr(shortcut, "Shift") != nullptr;
			result.requiresSuper = std::strstr(shortcut, "Super") != nullptr;

			// Find the key portion after the last '+'
			const char* key = std::strrchr(shortcut, '+');
			if (key) {
				key++;	// Skip the '+'
			} else {
				key = shortcut;	 // No '+', assume it's just a single key
			}

			// Map of string keys to ImGuiKey values
			if (std::strcmp(key, "N") == 0)
				result.key = ImGuiKey_N;
			else if (std::strcmp(key, "O") == 0)
				result.key = ImGuiKey_O;
			else if (std::strcmp(key, "S") == 0)
				result.key = ImGuiKey_S;
			else if (std::strcmp(key, "E") == 0)
				result.key = ImGuiKey_E;
			else if (std::strcmp(key, "Q") == 0)
				result.key = ImGuiKey_Q;
			else if (std::strcmp(key, "T") == 0)
				result.key = ImGuiKey_T;
			else if (std::strcmp(key, ",") == 0)
				result.key = ImGuiKey_Comma;
			// Add more key mappings as needed

			return result;
		};

		// Parse each provided shortcut once
		Shortcut newProjShortcut = parseShortcut(newProj);
		Shortcut openShortcut = parseShortcut(open);
		Shortcut saveShortcut = parseShortcut(save);
		Shortcut exportShortcutShortcut = parseShortcut(exportShortcut);
		Shortcut quitShortcut = parseShortcut(quit);
		Shortcut prefShortcut = parseShortcut(pref);
		Shortcut themeEditorShortcutShortcut =
			parseShortcut(themeEditorShortcut);

		// Inline lambda to check if a given shortcut is pressed
		auto isShortcutPressed = [&](const Shortcut& shortcut) {
			return (shortcut.requiresCtrl == io.KeyCtrl) &&
				   (shortcut.requiresShift == io.KeyShift) &&
				   (shortcut.requiresSuper == io.KeySuper) &&
				   ImGui::IsKeyPressed(shortcut.key);
		};

		// Check if each shortcut is pressed and perform the appropriate action
		if (isShortcutPressed(newProjShortcut)) {
			if (m_SFXEnabled)
				m_AssetLoader->PlaySound(s_ProjCreateSound);
			m_NewProjOpen = true;
		}
		if (isShortcutPressed(openShortcut)) {
			OpenFileDialog([&]() { this->ReadProjectFile(); });
		}
		if (isShortcutPressed(saveShortcut)) {
			if (m_SFXEnabled)
				m_AssetLoader->PlaySound(s_ProjSaveSound);
			SaveFileDialog([&]() { this->SaveProjectFile(); });
			if (m_SFXEnabled)
				m_AssetLoader->PlaySound(s_ProjSaveEndSound);
		}
		if (isShortcutPressed(quitShortcut)) {
			glfwSetWindowShouldClose(m_Window, true);
		}
		if (isShortcutPressed(prefShortcut)) {
			m_EditPrefsOpen = true;
		}
		if (isShortcutPressed(themeEditorShortcutShortcut)) {
			Themes::g_ThemeEditorOpen = true;
		}
		if (m_CurrentProj) {
			if (isShortcutPressed(exportShortcutShortcut)) {
				m_ExportOpen = true;
			}
		}
	}

	void Application::RenderImGui() {
		if (!m_CurrentProj)
			RenderOptions();

		char* newProjShortcut = GlobalGetShortcut("N", false, false);
		char* openShortcut = GlobalGetShortcut("O", false, false);
		char* saveShortcut = GlobalGetShortcut("S", false, false);
		char* exportShortcut = GlobalGetShortcut("E", false, false);
		char* quitShortcut = GlobalGetShortcut("Q", false, false);
		char* prefShortcut = GlobalGetShortcut(",", false, false);
		char* themeEditorShortcut = GlobalGetShortcut("T", false, false);
		CheckKeybinds(newProjShortcut, openShortcut, saveShortcut,
					  exportShortcut, quitShortcut, prefShortcut,
					  themeEditorShortcut);
		RenderMainMenuBar(newProjShortcut, openShortcut, saveShortcut,
						  exportShortcut, quitShortcut, prefShortcut,
						  themeEditorShortcut);
		RenderEditPrefs();
		SaveChangesDialog();
		RenderExport();
		RenderEditProj();
		RenderKeybinds();
		RenderNewProj();

		Themes::ThemeEditor();
		Themes::SaveCurrentTheme();

		if (m_CurrentProj) {
			m_Manager->RenderTimeline(m_PrevEnabled);
			m_Manager->RenderControls();
			m_Manager->Player()->Play();
			m_CurrentProj->DisplayFPS();
		} else {
			char* title = (char*)malloc(35);
			if (title != 0) {
				snprintf(title, 35, "FuncDoodle -- %s -- %d FPS", FUNCVER,
						 (int)ImGui::GetIO().Framerate);
				glfwSetWindowTitle(m_Window, title);
				free(title);
			} else {
				FUNC_WARN(
					"Failed to allocate title -- perhaps you ran out of RAM?");
			}
		}

		free(newProjShortcut);
		free(openShortcut);
		free(saveShortcut);
		free(exportShortcut);
		free(quitShortcut);
	}
	void Application::OpenFileDialog(std::function<void()> done) {
		nfdchar_t* outPath = 0;
		nfdresult_t result = NFD_OpenDialog("fdp", 0, &outPath);

		if (result == NFD_OKAY) {
			m_FilePath = outPath;
			done();
			free(outPath);
		} else if (result == NFD_CANCEL) {
			FUNC_DBG("Cancelled");
		} else {
			FUNC_WARN("Failed to open open file dialog -- " +
					  (std::string)NFD_GetError());
		}
		// free(outPath);
	}
	void Application::SaveFileDialog(std::function<void()> done) {
		if (m_CurrentProj == nullptr) {
			FUNC_INF("No project to save");
			return;
		}
		nfdchar_t* outPath = 0;
		nfdresult_t result = NFD_SaveDialog("fdp", 0, &outPath);

		if (result == NFD_OKAY) {
			m_FilePath = outPath;
			done();
			free(outPath);
		} else if (result == NFD_CANCEL) {
			FUNC_DBG("Cancelled");
		} else {
			FUNC_WARN("Failed to open save file dialog -- " +
					  (std::string)NFD_GetError());
		}
	}
	void Application::ReadProjectFile() {
		// m_FilePath is the actual file that we're going to read
		if (m_FilePath == nullptr) {
			FUNC_DBG("@Application::ReadProjectFile -- m_FilePath is nullptr");
			return;
		}

		if (m_CurrentProj == nullptr) {
			m_CurrentProj =
				new ProjectFile((char*)"", 1, 1, (char*)"", 0, (char*)"",
								m_Window, Col{.r = 0, .g = 0, .b = 0});
		}

		m_CurrentProj->ReadAndPopulate(m_FilePath);

		m_Manager->SetProj(m_CurrentProj);
	}
	void Application::SaveProjectFile() {
		if (m_FilePath == nullptr) {
			FUNC_DBG("@Application::SaveProjectFile -- m_FilePath is nullptr");
			return;
		}
		m_CurrentProj->Write(m_FilePath);
	}
	void Application::RenderOptions() {
		// Get the viewport dimensions
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 viewportPos = viewport->WorkPos;
		ImVec2 viewportSize = viewport->WorkSize;
		float menuBarHeight = ImGui::GetFrameHeight();

		// Calculate center of viewport
		ImVec2 center(viewportPos.x + (viewportSize.x * 0.5f),
					  viewportPos.y + (viewportSize.y * 0.5f));

		ImVec4 btnNewCol = ImVec4(1, 1, 1, 1);
		ImVec4 btnOpenCol = ImVec4(1, 1, 1, 1);
		ImVec4 tintNew = ImVec4(0, 0, 0, 1);
		ImVec4 tintOpen = ImVec4(0, 0, 0, 1);

		// Get the draw list for the viewport
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		auto invertColor = [](ImVec4& col) {
			col.x = 1.0f - col.x;
			col.y = 1.0f - col.y;
			col.z = 1.0f - col.z;
			col.w = 1.0f;
		};

		int btnWidth = 50;
		int btnHeight = 50;

		ImVec2 safePosAdd = ImVec2(viewportPos.x, viewportPos.y);
		ImVec2 safePosAddMax =
			ImVec2(safePosAdd.x + btnWidth, safePosAdd.y + btnHeight);

		ImVec2 safePosOpen = ImVec2(viewportPos.x, viewportPos.y + 55);
		ImVec2 safePosOpenMax =
			ImVec2(safePosOpen.x + btnWidth, safePosOpen.y + btnHeight);

		drawList->AddRectFilled(viewportPos,
								ImVec2(viewportPos.x + viewportSize.x,
									   viewportPos.y + viewportSize.y),
								IM_COL32(50, 50, 50, 255));

		ImVec2 mousePos = ImGui::GetMousePos();
		if (!ImGui::IsAnyItemHovered()) {
			if (mousePos.x >= safePosAdd.x && mousePos.x <= safePosAddMax.x &&
				mousePos.y >= safePosAdd.y && mousePos.y <= safePosAddMax.y) {
				invertColor(btnNewCol);
				invertColor(tintNew);
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					if (m_SFXEnabled)
						m_AssetLoader->PlaySound(s_ProjCreateSound);
					m_NewProjOpen = true;
				}
			}

			if (mousePos.x >= safePosOpen.x && mousePos.x <= safePosOpenMax.x &&
				mousePos.y >= safePosOpen.y && mousePos.y <= safePosOpenMax.y) {
				invertColor(btnOpenCol);
				invertColor(tintOpen);
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					OpenFileDialog([&]() { this->ReadProjectFile(); });
				}
			}
		}

		drawList->AddRectFilled(safePosAdd, safePosAddMax,
								ImGui::ColorConvertFloat4ToU32(btnNewCol),
								10.0f);
		drawList->AddImage(
			(ImTextureID)(intptr_t)s_AddTexId, safePosAdd,
			ImVec2(safePosAdd.x + btnWidth, safePosAdd.y + btnHeight),
			ImVec2(0, 0), ImVec2(1, 1),
			ImGui::ColorConvertFloat4ToU32(tintNew));
		drawList->AddRectFilled(safePosOpen, safePosOpenMax,
								ImGui::ColorConvertFloat4ToU32(btnOpenCol),
								10.0f);
		drawList->AddImage(
			(ImTextureID)(intptr_t)s_OpenTexId, safePosOpen,
			ImVec2(safePosOpen.x + btnWidth, safePosOpen.y + btnHeight),
			ImVec2(0, 0), ImVec2(1, 1),
			ImGui::ColorConvertFloat4ToU32(tintOpen));
	}
	void Application::SaveChangesDialog() {
		if (m_SaveChangesOpen) {
			ImGui::OpenPopup("SaveChanges");
			m_SaveChangesOpen = false;
		}

		if (ImGui::BeginPopupModal("SaveChanges")) {
			ImGui::Text("Save changes?");
			if (ImGui::Button("Yes")) {
				SaveFileDialog([&] { SaveProjectFile(); });
				// glfwSetWindowShouldClose(m_Window, true);
				m_ShouldClose = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No")) {
				// glfwSetWindowShouldClose(m_Window, true);
				m_ShouldClose = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				m_ShouldClose = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	void Application::OpenSaveChangesDialog() {
		FUNC_DBG("Saved?: " << m_CurrentProj->Saved());
		FUNC_DBG("m_SaveChangesOpen is getting set to true");
		m_SaveChangesOpen = true;
		FUNC_DBG("set m_SaveChangesOpen");
	}
	void Application::DropCallback(GLFWwindow* win, int count,
								   const char** paths) {
		if (count == 0)
			return;
		if (count > 1) {
			FUNC_WARN("Attempted to drag and drop multiple items when 1 is "
					  "expected: Attempting to use first item");
		}
		m_FilePath = const_cast<char*>(paths[0]);
		this->ReadProjectFile();
	}
	void Application::RenderEditProj() {
		if (m_EditProjOpen) {
			ImGui::OpenPopup("EditProj");
		}

		if (ImGui::IsPopupOpen("EditProj")) {
			ImGui::SetNextWindowFocus();
			ImGui::SetNextWindowPos(ImVec2(485, 384), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(309, 312), ImGuiCond_FirstUseEver);
		}
		if (ImGui::BeginPopupModal("EditProj", &m_EditProjOpen,
								   ImGuiWindowFlags_AlwaysAutoResize) &&
			m_CurrentProj) {
			char name[256];
			strcpy(name, m_CurrentProj->AnimName());
			int width = m_CurrentProj->AnimWidth();
			int height = m_CurrentProj->AnimHeight();
			char author[100];
			strcpy(author, m_CurrentProj->AnimAuthor());
			int fps = m_CurrentProj->AnimFPS();
			char desc[512];
			strcpy(desc, m_CurrentProj->AnimDesc());
			if (m_CacheProj) {
				strcpy(name, m_CacheProj->AnimName());
				width = m_CacheProj->AnimWidth();
				height = m_CacheProj->AnimHeight();
				strcpy(author, m_CacheProj->AnimAuthor());
				fps = m_CacheProj->AnimFPS();
				strcpy(desc, m_CacheProj->AnimDesc());
			} else {
				strcpy(name, (char*)"Untitled Animation");
				width = 32;
				height = 32;
				char* username =
					std::getenv("USER");  // Common on Linux and macOS
				if (!username) {
					username =
						std::getenv("LOGNAME");	 // Fallback for Linux and macOS
				}
				if (!username) {
					username = std::getenv("USERNAME");	 // Common on Windows
				}
				strcpy(author, username);
				free(username);
				fps = 10;
				strcpy(desc, "Simple test project");
			}
			if (ImGui::InputText("Name", name, sizeof(name))) {
				m_CacheProj->SetAnimName(name);
			}
			if (ImGui::InputInt("Width", &width)) {
				m_CacheProj->SetAnimWidth(width);
			}
			if (ImGui::InputInt("Height", &height)) {
				m_CacheProj->SetAnimHeight(height);
			}
			if (ImGui::InputText("Author", author, sizeof(author))) {
				m_CacheProj->SetAnimAuthor(author);
			}
			if (ImGui::InputInt("FPS", &fps)) {
				m_CacheProj->SetAnimFPS(fps);
			}
			if (ImGui::InputText("Description", desc, sizeof(desc))) {
				m_CacheProj->SetAnimDesc(desc);
			}

			if (ImGui::Button("Close") ||
				ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
				m_EditProjOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("OK") ||
				ImGui::IsKeyPressed(ImGuiKey_Enter, false) ||
				ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false)) {
				m_CurrentProj = m_CacheProj;
				m_EditProjOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	void Application::RenderNewProj() {
		if (m_NewProjOpen) {
			ImGui::OpenPopup("NewProj");
		}
		if (ImGui::IsPopupOpen("NewProj")) {
			ImGui::SetNextWindowFocus();
			ImGui::SetNextWindowPos(ImVec2(376, 436), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(350, 336), ImGuiCond_FirstUseEver);
		}
		if (ImGui::BeginPopupModal("NewProj", &m_NewProjOpen,
								   ImGuiWindowFlags_AlwaysAutoResize)) {
			char name[256] = "";
			int width = 32;
			int height = 32;
			char author[100] = "";
			int fps = 0;
			char desc[512] = "";

			if (!m_CacheProj) {
				strcpy(name, (char*)"testproj");
				width = 32;
				height = 32;
				char* username = std::getenv("USER");
				if (!username) {
					username = std::getenv("LOGNAME");
				}
				if (!username) {
					username = std::getenv("USERNAME");
				}
				strcpy(author, username);
				fps = 10;
				strcpy(desc, "Simple test project");
				m_CacheProj = new ProjectFile(name, width, height, username,
											  fps, desc, m_Window, Col());
			} else {
				strcpy(name, m_CacheProj->AnimName());
				width = m_CacheProj->AnimWidth();
				height = m_CacheProj->AnimHeight();
				strcpy(author, m_CacheProj->AnimAuthor());
				fps = m_CacheProj->AnimFPS();
				strcpy(desc, m_CacheProj->AnimDesc());

				if (m_CacheBGCol) {
					float r = (float)(m_CacheProj->BgCol().r) / 255;
					float g = (float)(m_CacheProj->BgCol().g) / 255;
					float b = (float)(m_CacheProj->BgCol().b) / 255;
					m_CacheBGCol[0] = r;
					m_CacheBGCol[1] = g;
					m_CacheBGCol[2] = b;
				}
			}

			// GUI inputs for project properties
			if (ImGui::InputText("Name", name, sizeof(name))) {
				m_CacheProj->SetAnimName(name);
			}
			if (ImGui::InputInt("Width", &width)) {
				if (m_CurrentProj)
					m_CacheProj->SetAnimWidth(width, false);
				else {
					if (m_CacheProj) {
						m_CacheProj->SetAnimWidth(width, true);
					}
				}
			}
			if (ImGui::InputInt("Height", &height)) {
				if (m_CurrentProj)
					m_CacheProj->SetAnimHeight(height, false);
				else
					m_CacheProj->SetAnimHeight(height, true);
			}
			if (ImGui::InputText("Author", author, sizeof(name))) {
				m_CacheProj->SetAnimAuthor(author);
			}
			if (ImGui::InputInt("FPS", &fps)) {
				m_CacheProj->SetAnimFPS(fps);
			}
			if (ImGui::InputText("Description", desc, sizeof(name))) {
				m_CacheProj->SetAnimDesc(desc);
			}
			if (ImGui::ColorPicker3("BG", m_CacheBGCol)) {
				if (m_CacheProj)
					m_CacheProj->SetBgCol(m_CacheBGCol);
			}

			if (ImGui::Button("Close") ||
				ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
				m_NewProjOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("OK") ||
				ImGui::IsKeyPressed(ImGuiKey_Enter, false) ||
				ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false)) {
				m_CurrentProj = m_CacheProj;
				m_Manager = new AnimationManager(m_CurrentProj, m_AssetLoader);
				m_NewProjOpen = false;
			}

			ImGui::EndPopup();
		}
	}
	void Application::RenderMainMenuBar(char* newProjShortcut,
										char* openShortcut, char* saveShortcut,
										char* exportShortcut,
										char* quitShortcut, char* prefShortcut,
										char* themeEditorShortcut) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File", true)) {
				if (ImGui::MenuItem("New project", newProjShortcut)) {
					if (m_SFXEnabled)
						m_AssetLoader->PlaySound(s_ProjCreateSound);
					m_NewProjOpen = true;
				}

				if (ImGui::MenuItem("Open", openShortcut)) {
					this->OpenFileDialog([&]() { this->ReadProjectFile(); });
				}
				if (ImGui::MenuItem("Save", saveShortcut)) {
					if (m_SFXEnabled)
						m_AssetLoader->PlaySound(s_ProjSaveSound);
					this->SaveFileDialog([&]() { this->SaveProjectFile(); });
				}
				if (m_CurrentProj) {
					if (ImGui::MenuItem("Close")) {
						m_CurrentProj = nullptr;
						delete m_CurrentProj;
					}
					if (ImGui::MenuItem("Edit project")) {
						m_EditProjOpen = true;
					}
					if (ImGui::MenuItem("Export", exportShortcut)) {
						m_ExportOpen = true;
					}
				}
				if (ImGui::MenuItem("Exit", quitShortcut)) {
					m_EditProjOpen = false;
					m_ExportOpen = false;
					m_NewProjOpen = false;
					glfwSetWindowShouldClose(m_Window, true);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit", true)) {
				if (ImGui::MenuItem("Preferences", prefShortcut)) {
					m_EditPrefsOpen = true;
				}
				if (ImGui::MenuItem("Theme editor", themeEditorShortcut)) {
					Themes::g_ThemeEditorOpen = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help", true)) {
				if (ImGui::MenuItem("Show keybinds")) {
					m_ShowKeybindsOpen = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	void Application::RenderEditPrefs() {
		if (m_EditPrefsOpen) {
			ImGui::OpenPopup("EditPrefs");
			m_EditPrefsOpen = false;	// Reset flag since OpenPopup only sets visibility
		}
		if (ImGui::BeginPopup("EditPrefs")) {
			if (ImGui::BeginCombo("Theme", Themes::g_Themes[m_Theme].Name)) {
				for (auto& [uuid, theme] : Themes::g_Themes) {
					bool is_selected = (m_Theme == uuid);
					if (ImGui::Selectable(theme.Name, is_selected)) {
						m_Theme = uuid;
						ImGui::GetStyle() = theme.Style;
						ImGui::GetStyle().Alpha = 1.0f;	 // Fully opaque
						ImGui::GetStyle().WindowRounding = 10.0f;
						ImGui::GetStyle().FrameRounding = 5.0f;
						ImGui::GetStyle().PopupRounding = 12.0f;
						ImGui::GetStyle().ScrollbarRounding = 10.0f;
						ImGui::GetStyle().GrabRounding = 6.0f;
						ImGui::GetStyle().TabRounding = 12.0f;
						ImGui::GetStyle().ChildRounding = 12.0f;
						ImGui::GetStyle().WindowPadding = ImVec2(10, 10);
						ImGui::GetStyle().FramePadding = ImVec2(8, 8);
						ImGui::GetStyle().ItemSpacing = ImVec2(10, 10);
						ImGui::GetStyle().IndentSpacing = 20.0f;
						ImGui::GetStyle().ScrollbarSize = 16.0f;
						if (ImGui::GetIO().ConfigFlags &
							ImGuiConfigFlags_ViewportsEnable) {
							ImGui::GetStyle().WindowRounding = 1.0f;
						}
					}
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("by %s", theme.Author);
						ImGui::EndTooltip();
					}
					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::Button("Save current")) {
				Themes::g_SaveThemeOpen = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Add temporary from file")) {
				static Themes::CustomTheme* style;
				nfdpathset_t pathset;
				nfdresult_t res = NFD_OpenDialogMultiple("toml", "", &pathset);
				if (res == NFD_OKAY) {
					for (size_t i = 0; i < NFD_PathSet_GetCount(&pathset);
							i++) {
						nfdchar_t* path = NFD_PathSet_GetPath(&pathset, i);
						style = Themes::LoadThemeFromFile(path);
						if (style) {
							Themes::g_Themes.emplace(style->Uuid, *style);
						}
					}
				} else if (res == NFD_ERROR) {
					FUNC_ERR(
							"Failed to open save theme dialog: " << NFD_GetError());
				}
			}
			if (ImGui::Button("Open themes directory")) {
				OPEN_FILE_EXPLORER(m_ThemesPath);
			}
			ImGui::SameLine();
			if (ImGui::Button("Refresh")) {
				Themes::g_Themes.clear();
				Themes::LoadThemes(m_ThemesPath);
			}

			ImGui::Checkbox("SFX", &m_SFXEnabled);
			ImGui::SameLine();
			ImGui::Checkbox("Preview", &m_PrevEnabled);
			ImGui::SameLine();

			if (ImGui::IsKeyPressed(ImGuiKey_Escape) ||
					ImGui::IsKeyPressed(ImGuiKey_Enter) ||
					ImGui::IsKeyPressed(ImGuiKey_KeypadEnter) ||
					ImGui::Button("OK")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	void Application::RenderExport() {
		if (m_ExportOpen) {
			ImGui::OpenPopup("Export##export");
			m_ExportOpen = false;
		}

		if (ImGui::BeginPopup("Export##export")) {
			const char* formats[] = {"PNGs", "MP4"};
			ImGui::Combo("Export Format", &m_ExportFormat, formats,
					IM_ARRAYSIZE(formats));
			if (ImGui::Button("Export") ||
					ImGui::IsKeyPressed(ImGuiKey_Enter, false) ||
					ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false)) {
				nfdchar_t* outPath = 0;
				nfdresult_t result = NFD_PickFolder(0, &outPath);

				if (result == NFD_OKAY) {
					if (m_SFXEnabled)
						m_AssetLoader->PlaySound(s_ExportSound);
					FUNC_INF("Exporting to " << outPath);
					m_CurrentProj->Export(outPath, m_ExportFormat);
					free(outPath);
				} else if (result == NFD_CANCEL) {
					FUNC_DBG("Cancelled");
					free(outPath);
				} else {
					FUNC_DBG("Failed to open file dialog" +
							(std::string)NFD_GetError());
					free(outPath);
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close") ||
					ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	void Application::RenderKeybinds() {
		if (m_ShowKeybindsOpen) {
			ImGui::OpenPopup("Keybinds");
		}
		if (ImGui::BeginPopupModal("Keybinds", &m_ShowKeybindsOpen,
					ImGuiWindowFlags_AlwaysAutoResize)) {
			std::filesystem::path keysPath =
				m_AssetLoader->GetPath() / "keys.txt";
			std::ifstream keysIn(keysPath);
			if (!keysIn) {
				FUNC_WARN("Failed to open file keys.txt");
				ImGui::EndPopup();
			}

			keysIn.seekg(0, std::ios::end);
			std::streamsize fileSize = keysIn.tellg();
			keysIn.seekg(0, std::ios::beg);

			char* buf = new char[fileSize + 1];

			if (keysIn.read(buf, fileSize)) {
				buf[fileSize] = '\0';
				ImGui::Text("%s", buf);
			} else {
				FUNC_WARN("Failed to read file keys.txt");
				delete[] buf;
				ImGui::EndPopup();
			}

			ImGui::EndPopup();
		}
	}
}  // namespace FuncDoodle
