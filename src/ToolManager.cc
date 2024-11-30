#include "ToolManager.h"

#include "Gui.h"
#include "Tool.h"

#include "LoadedImages.h"

namespace FuncDoodle {
    ToolManager::ToolManager(AssetLoader* assetLoader) : m_SelectedTool(0), m_AssetLoader(assetLoader) {
    } 
    ToolManager::~ToolManager() {
    }
    void ToolManager::RenderTools() {
        ToolType types[] = { ToolType::Pencil, ToolType::Eraser, ToolType::Bucket, ToolType::Picker };
        char* typeName[] = { "Pencil", "Eraser", "Bucket", "Picker" };
        ImGui::Begin("Tools");

        if (!ImGui::IsAnyItemActive()) {
            if (ImGui::IsKeyPressed(ImGuiKey_1)) {
                m_SelectedTool = 0;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_2)) {
                m_SelectedTool = 1;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_3)) {
                m_SelectedTool = 2;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_4)) {
                m_SelectedTool = 3;
            }
        }
        for (int i = 0; i < (sizeof(types) / sizeof(ToolType)); i++) {
            uint32_t btnTexId = 0;
            switch (i) {
                case 0:
                    btnTexId = s_PencilTexId;
                    break;
                case 1:
                    btnTexId = s_EraserTexId;
                    break;
                case 2:
                    btnTexId = s_BucketTexId;
                    break;
                case 3:
                    btnTexId = s_PickerTexId;
                    break;
                case 4:
                    btnTexId = s_TextTexId;
                    break;
            }
            if (ImGui::ImageButton(typeName[i], (void*)(intptr_t)btnTexId, ImVec2(32,32), ImVec2(0,0), ImVec2(1,1), ImVec4(0,0,0,0), m_SelectedTool == i ? ImVec4(1,1,1,1) : ImVec4(0,0,0,1))) {
                m_SelectedTool = i;
            }
            if (i == 0) {
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Pencil (1)");
                    ImGui::EndTooltip();
                }
            } else if (i == 1) {
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Eraser (2)");
                    ImGui::EndTooltip();
                }
            } else if (i == 2) {
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Bucket (3)");
                    ImGui::EndTooltip();
                }
            } else if (i == 3) {
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Picker (4)");
                    ImGui::EndTooltip();
                }
            }
        }

        if (m_SelectedTool < 2) {
            ImGui::SliderInt("##Size", &m_Size, 1, 100, "%dpx");
            ImGui::SameLine();
            ImGui::InputInt("##SizeText", &m_Size, 1, 100, 0);
        }

        if (m_SelectedTool != 1) {
            ImGui::ColorPicker3("Col", m_Col);
        }

        if (m_Size < 1) m_Size = 1;

        if (ImGui::IsKeyPressed(ImGuiKey_Semicolon, true)) {
            m_Size--;
        } else if (ImGui::IsKeyPressed(ImGuiKey_Apostrophe, true)) {
            m_Size++;
        }
        ImGui::End();
    }
}
