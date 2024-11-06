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
        // remove later
        GlobalLoadImages(m_AssetLoader);

        if (ImGui::IsKeyPressed(ImGuiKey_6)) {
            m_SelectedTool = 0;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_7)) {
            m_SelectedTool = 1;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_8)) {
            m_SelectedTool = 2;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_9)) {
            m_SelectedTool = 3;
        }
        for (int i = 0; i < (sizeof(types) / sizeof(ToolType)); i++) {
            uint btnTexId = 0;
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
            }
            if (ImGui::ImageButton(typeName[i], (void*)(intptr_t)btnTexId, ImVec2(32,32), ImVec2(0,0), ImVec2(1,1), ImVec4(0,0,0,0), m_SelectedTool == i ? ImVec4(1,1,1,1) : ImVec4(0,0,0,1))) {
                m_SelectedTool = i;
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
