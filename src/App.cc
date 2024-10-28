#include "App.h"

#include "Gui.h"
#include "Manager.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>

namespace FuncDoodle {
    // TODO: temporary m_FilePath in application
    // ;.............mnv,.
    Application::Application(GLFWwindow* win) : m_FilePath("???"), m_NewProjOpen(false), m_CurrentProj(nullptr), m_CacheProj(new ProjectFile("asdf", 1, 1, "asdf", 5, "asdf", win)), m_Manager(new AnimationManager(nullptr)), m_Window(win) {}
    Application::~Application() {}
    void Application::RenderImGui() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New project")) {
                    m_NewProjOpen = true;
                }
                
                if (ImGui::MenuItem("Open")) {
                    this->OpenFileDialog();
                }
                if (ImGui::MenuItem("Save")) {
                    this->SaveFileDialog();
                }
                if (ImGui::MenuItem("Exit")) {
                    std::exit(0);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (m_NewProjOpen) {
            ImGui::OpenPopup("NewProj");
        }

        if (ImGui::IsPopupOpen("NewProj")) {
            ImGui::SetNextWindowFocus();
        }

        if (ImGui::BeginPopupModal("NewProj", &m_NewProjOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
            char name[256] = "";
            int width = 32;
            int height = 32;
            char author[100] = "";
            int fps = 0;
            char desc[512] = "";
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
            if (ImGui::InputText("Author", author, sizeof(name))) {
                m_CacheProj->SetAnimAuthor(author);
            }
            if (ImGui::InputInt("FPS", &fps)) {
                m_CacheProj->SetAnimFPS(fps);
            }
            if (ImGui::InputText("Description", desc, sizeof(name))) {
                m_CacheProj->SetAnimDesc(desc);
            }

            if (ImGui::Button("Close")) { m_NewProjOpen = false; ImGui::CloseCurrentPopup(); }
            if (ImGui::Button("OK")) {
                m_CurrentProj = m_CacheProj;
                m_Manager->SetProj(m_CurrentProj);
                m_NewProjOpen = false;
            }
            ImGui::EndPopup();
        }

        if (m_CurrentProj) {
            m_Manager->RenderTimeline();
            m_Manager->RenderControls();

            m_Manager->Player()->Play();
            m_CurrentProj->DisplayFPS();
        }
    }
    void Application::OpenFileDialog() {
        nfdchar_t* outPath = 0;
        nfdresult_t result = NFD_OpenDialog("fdp", 0, &outPath);

        if (result == NFD_OKAY) {
            m_FilePath = outPath;
            ReadProjectFile();
            free(outPath);
        } else if (result == NFD_CANCEL) {
            std::cout << "Cancelled" << std::endl;
        } else {
            std::cout << "Error: " << NFD_GetError() << std::endl;
        }
    }
    void Application::SaveFileDialog() {
        nfdchar_t* outPath = 0;
        nfdresult_t result = NFD_SaveDialog("fdp", 0, &outPath);

        if (result == NFD_OKAY) {
            m_FilePath = outPath;
            SaveProjectFile();
            free(outPath);
        } else if (result == NFD_CANCEL) {
            std::cout << "Cancelled" << std::endl;
        } else {
            std::cout << "Error: " << NFD_GetError() << std::endl;
        }
    }
    void Application::ReadProjectFile() {
        // m_FilePath is the auctual file that we're going to read
        if (m_FilePath == "???") {
            std::cout << "Congratulations! You've found a weird bug that i've never seen before! Please screen record urself and make a github issue on this project. I rlly wanna fix this." << std::endl;
            return;
        }
        
        if (m_CurrentProj == nullptr) {
            m_CurrentProj = new ProjectFile("", 1, 1, "", 0, "", m_Window);
        }

        m_CurrentProj->ReadAndPopulate(m_FilePath);

        m_Manager->SetProj(m_CurrentProj);
    }
    void Application::SaveProjectFile() {
        if (m_FilePath == "???") {
            std::cout << "Congratulations! You've found a weird bug that i've never seen before! Please screen record urself and make a github issue on this project. I rlly wanna fix this." << std::endl;
            return;
        }
        m_CurrentProj->Write(m_FilePath);
    }
}
