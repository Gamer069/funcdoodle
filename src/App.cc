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
#include <vector>

#include <fstream>

#include "LoadedImages.h"

#include "MacroUtils.h"


namespace FuncDoodle {
	Application::Application(GLFWwindow* win, AssetLoader* assetLoader)
		: m_FilePath(nullptr), m_NewProjOpen(false), m_CurrentProj(nullptr),
		m_CacheProj(nullptr),
		m_Manager(new AnimationManager(nullptr, assetLoader)), m_Window(win),
		m_AssetLoader(assetLoader) {}
	Application::~Application() {
		delete m_Manager;
		delete m_FilePath;
		delete m_CurrentProj;
	}
	char* GlobalGetShortcut(const char* key, bool shift, bool super) {
		// Calculate the maximum possible length of the shortcut string
		int maxLen =
			11 +
			strlen(
					key);  // Assuming "Ctrl+" or "Cmd+" and "+Shift" and "+Super"

		// Allocate memory for the shortcut string
		char* shortcut = (char*)malloc(maxLen);

		// std::memset(shortcut, 0, sizeof(shortcut));

		// Copy the platform-specific modifier
#if defined(_WIN32) || defined(__linux__)
		strcpy(shortcut, "Ctrl");
#else
		strcpy(shortcut, "Cmd");
#endif

		// Append modifiers if necessary
		if (shift) {
			strcat(shortcut, "+Shift");
		}

		if (super) {
			strcat(shortcut, "+Super");
		}

		// Append the key
		strcat(shortcut, "+");
		strcat(shortcut, key);

		return shortcut;
	}

	void Application::CheckKeybinds(char* newProj, char* open, char* save,
			char* exportShortcut, char* quit) {
		ImGuiIO& io = ImGui::GetIO();

		// Inline struct to store each shortcut's parsed values
		struct Shortcut {
			bool requiresCtrl;
			bool requiresShift;
			bool requiresSuper;
			ImGuiKey key;
		};

		// Parse a shortcut string into a Shortcut struct
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
			// Add more key mappings as needed

			return result;
		};

		// Parse each provided shortcut once
		Shortcut newProjShortcut = parseShortcut(newProj);
		Shortcut openShortcut = parseShortcut(open);
		Shortcut saveShortcut = parseShortcut(save);
		Shortcut exportShortcutShortcut = parseShortcut(exportShortcut);
		Shortcut quitShortcut = parseShortcut(quit);

		// Inline lambda to check if a given shortcut is pressed
		auto isShortcutPressed = [&](const Shortcut& shortcut) {
			return (shortcut.requiresCtrl == io.KeyCtrl) &&
				(shortcut.requiresShift == io.KeyShift) &&
				(shortcut.requiresSuper == io.KeySuper) &&
				ImGui::IsKeyPressed(shortcut.key);
		};

		// Check if each shortcut is pressed and perform the appropriate action
		if (isShortcutPressed(newProjShortcut)) {
			m_NewProjOpen = true;
		}
		if (isShortcutPressed(openShortcut)) {
			OpenFileDialog([&](){this->ReadProjectFile();});
		}
		if (isShortcutPressed(saveShortcut)) {
			SaveFileDialog([&](){this->SaveProjectFile();});
		}
		if (isShortcutPressed(quitShortcut)) {
			glfwSetWindowShouldClose(m_Window, true);
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
		CheckKeybinds(newProjShortcut, openShortcut, saveShortcut,
				exportShortcut, quitShortcut);

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File", true)) {
				if (ImGui::MenuItem("New project", newProjShortcut)) {
					m_NewProjOpen = true;
				}

				if (ImGui::MenuItem("Open", openShortcut)) {
					this->OpenFileDialog([&](){this->ReadProjectFile();});
				}
				if (ImGui::MenuItem("Save", saveShortcut)) {
					this->SaveFileDialog([&](){this->SaveProjectFile();});
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
				if (ImGui::MenuItem("Preferences")) {
					m_EditPrefsOpen = true;
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

		if (m_EditPrefsOpen) {
			ImGui::OpenPopup("EditPrefs");
		}

		if (m_NewProjOpen) {
			ImGui::OpenPopup("NewProj");
		}

		// wasnt here before
		if (m_ShowKeybindsOpen) {
			ImGui::OpenPopup("Keybinds");
		}

		if (ImGui::IsPopupOpen("EditPrefs")) {
			ImGui::SetNextWindowFocus();
			ImGui::SetNextWindowPos(ImVec2(655, 478), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(323, 152), ImGuiCond_FirstUseEver);
		}

		// TODO: add default pos for keybinds
		if (ImGui::BeginPopupModal("EditPrefs", &m_EditPrefsOpen,
					ImGuiWindowFlags_AlwaysAutoResize)) {
			const char* themes[] = {"Custom", "Dark", "Light", "Classic"};
			if (ImGui::ListBox("Theme", &m_Theme, themes,
						IM_ARRAYSIZE(themes))) {
				switch (m_Theme) {
					case 0:
						CustomStyle();
						break;
					case 1:
						ImGui::StyleColorsDark();
						break;
					case 2:
						ImGui::StyleColorsLight();
						break;
					case 3:
						ImGui::StyleColorsClassic();
						break;
				}
			}
			ImGui::EndPopup();
		}

		if (m_ExportOpen) {
			ImGui::OpenPopup("Export##modal");
		}

		if (ImGui::IsPopupOpen("Export##modal")) {
			ImGui::SetNextWindowFocus();
			ImGui::SetNextWindowPos(ImVec2(503, 467), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(367, 168), ImGuiCond_FirstUseEver);
		}

		if (ImGui::BeginPopupModal("Export##modal", &m_ExportOpen,
					ImGuiWindowFlags_AlwaysAutoResize)) {
			const char* formats[] = {"PNGs", "MP4"};
			ImGui::ListBox("Export Format", &m_ExportFormat, formats,
					IM_ARRAYSIZE(formats));
			if (ImGui::IsItemClicked()) {
				m_ExportFormat = (m_ExportFormat + 1) % IM_ARRAYSIZE(formats);
			}
			if (ImGui::Button("Export") ||
					ImGui::IsKeyPressed(ImGuiKey_Enter, false) ||
					ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false)) {
				nfdchar_t* outPath = 0;
				nfdresult_t result = NFD_PickFolder(0, &outPath);

				if (result == NFD_OKAY) {
					FUNC_INF("Exporting to " + (std::string)outPath);
					m_CurrentProj->Export(outPath, m_ExportFormat);
					free(outPath);
				} else if (result == NFD_CANCEL) {
					FUNC_DBG("Cancelled");
				} else {
					FUNC_DBG("Failed to open file dialog" + (std::string)NFD_GetError());
				}
				m_ExportOpen = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Close") ||
					ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
				m_ExportOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (m_EditProjOpen) {
			ImGui::OpenPopup("EditProj");
		}

		if (ImGui::IsPopupOpen("EditProj")) {
			ImGui::SetNextWindowFocus();
			ImGui::SetNextWindowPos(ImVec2(485, 384), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(309, 312), ImGuiCond_FirstUseEver);
		}
		if (ImGui::BeginPopupModal("EditProj", &m_EditProjOpen,
					ImGuiWindowFlags_AlwaysAutoResize) && m_CurrentProj) {
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
				strcpy(name, (char*)"testproj");
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
		if (ImGui::BeginPopupModal("Keybinds", &m_ShowKeybindsOpen,
					ImGuiWindowFlags_AlwaysAutoResize)) {
			std::filesystem::path keysPath =
				m_AssetLoader->GetPath().parent_path() / "keys.txt";
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
			float* bgCol = (float*)std::malloc(sizeof(float)*3);
			if (!m_CacheProj) {
				strcpy(name, (char*)"testproj");
				width = 32;
				height = 32;
				char* username = std::getenv("USER");  // Common on Linux and macOS
				if (!username) {
					username = std::getenv("LOGNAME");	 // Fallback for Linux and macOS
				}
				if (!username) {
					username = std::getenv("USERNAME");	 // Common on Windows
				}
				strcpy(author, username);
				fps = 10;
				strcpy(desc, "Simple test project");
				m_CacheProj = new ProjectFile(
						(char*)"testproj", width, height, username, fps,
						(char*)"Simple test project", m_Window, Col{.r = 255, .g = 255, .b = 255});
			} else {
				strcpy(name, m_CacheProj->AnimName());
				width = m_CacheProj->AnimWidth();
				height = m_CacheProj->AnimHeight();
				strcpy(author, m_CacheProj->AnimAuthor());
				fps = m_CacheProj->AnimFPS();
				strcpy(desc, m_CacheProj->AnimDesc());
				if (bgCol) {
					float r = (float)(m_CacheProj->BgCol().r) / 255;
					float g = (float)(m_CacheProj->BgCol().g) / 255;
					float b = (float)(m_CacheProj->BgCol().b) / 255;
					bgCol = new float[3];
					bgCol[0] = r;
					bgCol[1] = g;
					bgCol[2] = b;
				}
			}
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
			if (ImGui::ColorPicker3("BG", bgCol)) {
				if (m_CacheProj)
					m_CacheProj->SetBgCol(bgCol);
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

		if (m_CurrentProj != nullptr) {
			// render timeline line is broken
			m_Manager->RenderTimeline();
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
				FUNC_WARN("Failed to allocate title -- perhaps you ran out of RAM?");
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
			FUNC_WARN("Failed to open open file dialog -- " + (std::string)NFD_GetError());
		}
		//free(outPath);
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
			FUNC_WARN("Failed to open save file dialog -- " + (std::string)NFD_GetError());
		}
	}
	void Application::ReadProjectFile() {
		// m_FilePath is the actual file that we're going to read
		if (m_FilePath == nullptr) {
			FUNC_DBG("@Application::ReadProjectFile -- m_FilePath is nullptr");
			return;
		}

		if (m_CurrentProj == nullptr) {
			m_CurrentProj = new ProjectFile((char*)"", 1, 1, (char*)"", 0,
					(char*)"", m_Window, Col{.r = 0, .g = 0, .b = 0});
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
					m_NewProjOpen = true;
				}
			}

			if (mousePos.x >= safePosOpen.x && mousePos.x <= safePosOpenMax.x &&
					mousePos.y >= safePosOpen.y && mousePos.y <= safePosOpenMax.y) {
				invertColor(btnOpenCol);
				invertColor(tintOpen);
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					OpenFileDialog([&](){this->ReadProjectFile();});
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
		ImGui::Begin("Save Changes?", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("There are unsaved changes. Do you wanna save them?");
		ImGui::Separator();

		if (ImGui::Button("Yes")) {
			SaveFileDialog([&](){this->SaveProjectFile();});
			glfwSetWindowShouldClose(m_Window, true);
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			glfwSetWindowShouldClose(m_Window, true);
		}
		ImGui::End();
	}
	void Application::CustomStyle() {
		ImGui::StyleColorsDark();

		ImGuiStyle* style = &ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		style->WindowRounding = 10.0f;
		style->FrameRounding = 5.0f;
		style->PopupRounding = 12.0f;
		style->ScrollbarRounding = 10.0f;
		style->GrabRounding = 6.0f;
		style->TabRounding = 12.0f;
		style->ChildRounding = 12.0f;
		style->WindowPadding = ImVec2(10, 10);
		style->FramePadding = ImVec2(8, 8);
		style->ItemSpacing = ImVec2(10, 10);
		style->IndentSpacing = 20.0f;
		style->ScrollbarSize = 16.0f;
		ImVec4* colors = style->Colors;
		colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_Border] = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.8f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_CheckMark] =
			ImVec4(0.0f, 1.0f, 0.0f, 1.0f);	 // Bright green
		colors[ImGuiCol_SliderGrab] =
			ImVec4(0.1f, 0.5f, 0.8f, 1.0f);	 // Bright blue
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1f, 0.6f, 0.9f, 1.0f);
		colors[ImGuiCol_Button] = ImVec4(0.1f, 0.5f, 0.8f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.6f, 0.9f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.7f, 1.0f, 1.0f);
		colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_Separator] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
		colors[ImGuiCol_Tab] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.2f, 0.3f, 0.5f, 0.7f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
		colors[ImGuiCol_ChildBg] =
			ImVec4(0.2f, 0.2f, 0.2f, 0.0f);	 // Make child background transparent
		colors[ImGuiCol_Border] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);

		// Optional: You can adjust global scaling or other parameters here if
		// needed
		style->Alpha = 1.0f;  // Fully opaque
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style->WindowRounding = 1.0f;
		}
	}
}  // namespace FuncDoodle
