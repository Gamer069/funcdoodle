#include "App.h"

#include "Gui.h"
#include "Manager.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cmath>
#include <imgui.h>
#include <cstring>
#include <unordered_map>
#include <stdint.h>

#include "LoadedImages.h"

namespace FuncDoodle
{
    Application::Application(GLFWwindow* win, AssetLoader* assetLoader) : m_FilePath("???"), m_NewProjOpen(false), m_CurrentProj(nullptr), m_CacheProj(new ProjectFile("asdf", 1, 1, "asdf", 5, "asdf", win)), m_Manager(new AnimationManager(nullptr, assetLoader)), m_Window(win), m_AssetLoader(assetLoader) {}
    Application::~Application() {}
    char* GlobalGetShortcut(const char* key, bool shift, bool super)
    {
        // Calculate the maximum possible length of the shortcut string
        int maxLen = 10 + strlen(key); // Assuming "Ctrl+" or "Cmd+" and "+Shift" and "+Super"

        // Allocate memory for the shortcut string
        char* shortcut = new char[maxLen];

        // Copy the platform-specific modifier
#ifdef _WIN32
        strcpy(shortcut, "Ctrl");
#else
        strcpy(shortcut, "Cmd");
#endif

        // Append modifiers if necessary
        if (shift)
        {
            strcat(shortcut, "+Shift");
        }

        if (super)
        {
            strcat(shortcut, "+Super");
        }

        // Append the key
        strcat(shortcut, "+");
        strcat(shortcut, key);

        return shortcut;
    }

    void Application::CheckKeybinds(char* newProj, char* open, char* save) {
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
            Shortcut result = { false, false, false, ImGuiKey_None };

            // Determine platform-specific "Ctrl" vs "Cmd" modifier
#ifdef _WIN32
            result.requiresCtrl = std::strstr(shortcut, "Ctrl") != nullptr;
#else
            result.requiresCtrl = std::strstr(shortcut, "Cmd") != nullptr;
#endif
            result.requiresShift = std::strstr(shortcut, "Shift") != nullptr;
            result.requiresSuper = std::strstr(shortcut, "Super") != nullptr;

            // Find the key portion after the last '+'
            const char* key = std::strrchr(shortcut, '+');
            if (key) {
                key++;  // Skip the '+'
            }
            else {
                key = shortcut;  // No '+', assume it's just a single key
            }

            // Map of string keys to ImGuiKey values
            if (std::strcmp(key, "N") == 0) result.key = ImGuiKey_N;
            else if (std::strcmp(key, "O") == 0) result.key = ImGuiKey_O;
            else if (std::strcmp(key, "S") == 0) result.key = ImGuiKey_S;
            // Add more key mappings as needed

            return result;
            };

        // Parse each provided shortcut once
        Shortcut newProjShortcut = parseShortcut(newProj);
        Shortcut openShortcut = parseShortcut(open);
        Shortcut saveShortcut = parseShortcut(save);

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
            OpenFileDialog();
        }
        if (isShortcutPressed(saveShortcut)) {
            SaveFileDialog();
        }
    }

    void Application::RenderImGui()
    {
        char* newProjShortcut = GlobalGetShortcut("N", false, false);
        char* openShortcut = GlobalGetShortcut("O", false, false);
        char* saveShortcut = GlobalGetShortcut("S", false, false);
        CheckKeybinds(newProjShortcut, openShortcut, saveShortcut);

        if (!m_CurrentProj) RenderOptions();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New project", newProjShortcut))
                {
                    m_NewProjOpen = true;
                }

                if (ImGui::MenuItem("Open", openShortcut))
                {
                    this->OpenFileDialog();
                }
                if (ImGui::MenuItem("Save", saveShortcut))
                {
                    this->SaveFileDialog();
                }
                if (m_CurrentProj) {
                    if (ImGui::MenuItem("Close")) {
                        m_CurrentProj = nullptr;
                        m_Manager = new AnimationManager(nullptr, m_AssetLoader);
                    }
                    if (ImGui::MenuItem("Edit project")) {
                        m_EditProjOpen = true;
                    }
                }
                if (ImGui::MenuItem("Exit", GlobalGetShortcut("Q", false, false)))
                {
                    std::exit(0);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (m_EditProjOpen) {
            ImGui::OpenPopup("EditProj");
        }

        if (ImGui::IsPopupOpen("EditProj")) {
            ImGui::SetNextWindowFocus();
        }

        if (ImGui::BeginPopupModal("EditProj", &m_EditProjOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
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

            if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
                m_EditProjOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGuiKey_Enter, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false)) {
                m_CurrentProj = m_CacheProj;
                m_EditProjOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (m_NewProjOpen)
        {
            ImGui::OpenPopup("NewProj");
        }

        if (ImGui::IsPopupOpen("NewProj"))
        {
            ImGui::SetNextWindowFocus();
        }

        if (ImGui::BeginPopupModal("NewProj", &m_NewProjOpen, ImGuiWindowFlags_AlwaysAutoResize))
        {
            char name[256] = "";
            int width = 32;
            int height = 32;
            char author[100] = "";
            int fps = 0;
            char desc[512] = "";
            if (m_CacheProj)
            {
                strcpy(name, m_CacheProj->AnimName());
                width = m_CacheProj->AnimWidth();
                height = m_CacheProj->AnimHeight();
                strcpy(author, m_CacheProj->AnimAuthor());
                fps = m_CacheProj->AnimFPS();
                strcpy(desc, m_CacheProj->AnimDesc());
            }
            if (ImGui::InputText("Name", name, sizeof(name)))
            {
                m_CacheProj->SetAnimName(name);
            }
            if (ImGui::InputInt("Width", &width))
            {
                m_CacheProj->SetAnimWidth(width);
            }
            if (ImGui::InputInt("Height", &height))
            {
                m_CacheProj->SetAnimHeight(height);
            }
            if (ImGui::InputText("Author", author, sizeof(name)))
            {
                m_CacheProj->SetAnimAuthor(author);
            }
            if (ImGui::InputInt("FPS", &fps))
            {
                m_CacheProj->SetAnimFPS(fps);
            }
            if (ImGui::InputText("Description", desc, sizeof(name)))
            {
                m_CacheProj->SetAnimDesc(desc);
            }

            if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
            {
                m_NewProjOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGuiKey_Enter, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, false))
            {
                m_CurrentProj = m_CacheProj;
                std::cout << "CREATE NEW PROJ" << std::endl;
                m_Manager = new AnimationManager(m_CurrentProj, m_AssetLoader);
                m_NewProjOpen = false;
            }
            ImGui::EndPopup();
        }

        if (m_CurrentProj)
        {
            m_Manager->RenderTimeline();
            m_Manager->RenderControls();
            m_Manager->Player()->Play();
            m_CurrentProj->DisplayFPS();
        }
    }
    void Application::OpenFileDialog()
    {
        nfdchar_t* outPath = 0;
        nfdresult_t result = NFD_OpenDialog("fdp", 0, &outPath);

        if (result == NFD_OKAY)
        {
            m_FilePath = outPath;
            ReadProjectFile();
            free(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            std::cout << "Cancelled" << std::endl;
        }
        else
        {
            std::cout << "Error: " << NFD_GetError() << std::endl;
        }
    }
    void Application::SaveFileDialog()
    {
        if (m_CurrentProj == nullptr)
        {
            std::cout << "No project to save!" << std::endl;
            return;
        }
        nfdchar_t* outPath = 0;
        nfdresult_t result = NFD_SaveDialog("fdp", 0, &outPath);

        if (result == NFD_OKAY)
        {
            m_FilePath = outPath;
            SaveProjectFile();
            free(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            std::cout << "Cancelled" << std::endl;
        }
        else
        {
            std::cout << "Error: " << NFD_GetError() << std::endl;
        }
    }
    void Application::ReadProjectFile()
    {
        // m_FilePath is the auctual file that we're going to read
        if (m_FilePath == "???")
        {
            std::cout << "Congratulations! You've found a weird bug that i've never seen before! Please screen record urself and make a github issue on this project. I rlly wanna fix this." << std::endl;
            return;
        }

        if (m_CurrentProj == nullptr)
        {
            m_CurrentProj = new ProjectFile("", 1, 1, "", 0, "", m_Window);
        }

        m_CurrentProj->ReadAndPopulate(m_FilePath);

        m_Manager->SetProj(m_CurrentProj);
    }
    void Application::SaveProjectFile()
    {
        if (m_FilePath == "???")
        {
            std::cout << "Congratulations! You've found a weird bug that i've never seen before! Please screen record urself and make a github issue on this project. I rlly wanna fix this." << std::endl;
            return;
        }
        m_CurrentProj->Write(m_FilePath);
    }
    void Application::RenderOptions()
    {
        // Get the viewport dimensions
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 viewportPos = viewport->WorkPos;
        ImVec2 viewportSize = viewport->WorkSize;
        float menuBarHeight = ImGui::GetFrameHeight();

        // Calculate center of viewport
        ImVec2 center(
            viewportPos.x + (viewportSize.x * 0.5f),
            viewportPos.y + (viewportSize.y * 0.5f)
        );

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
        ImVec2 safePosAddMax = ImVec2(safePosAdd.x + btnWidth, safePosAdd.y + btnHeight);

        ImVec2 safePosOpen = ImVec2(viewportPos.x, viewportPos.y + 55);
        ImVec2 safePosOpenMax = ImVec2(safePosOpen.x + btnWidth, safePosOpen.y + btnHeight);

        drawList->AddRectFilled(viewportPos, ImVec2(viewportPos.x + viewportSize.x, viewportPos.y + viewportSize.y), IM_COL32(50, 50, 50, 255));

        ImVec2 mousePos = ImGui::GetMousePos();
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
                OpenFileDialog();
            }
        }

        drawList->AddRectFilled(safePosAdd, safePosAddMax, ImGui::ColorConvertFloat4ToU32(btnNewCol), 10.0f);
        m_AssetLoader->RenderImage("add.png", drawList, safePosAdd, ImVec2(btnWidth, btnHeight), tintNew);
        drawList->AddRectFilled(safePosOpen, safePosOpenMax, ImGui::ColorConvertFloat4ToU32(btnOpenCol), 10.0f);
        m_AssetLoader->RenderImage("folder.png", drawList, safePosOpen, ImVec2(btnWidth, btnHeight), tintOpen);
    }
}
