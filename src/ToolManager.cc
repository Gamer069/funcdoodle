#include "ToolManager.h"

#include "Gui.h"
#include "Tool.h"

namespace FuncDoodle {
    ToolManager::ToolManager() : m_SelectedTool(0) {
    } 
    ToolManager::~ToolManager() {
    }
    void ToolManager::RenderTools() {
        ToolType types[] = { ToolType::Pencil, ToolType::Eraser, ToolType::Bucket, ToolType::Picker };
        char* typeName[] = { "Pencil", "Eraser", "Bucket", "Picker" };
        ImGui::Begin("Tools");
        for (int i = 0; i < (sizeof(types) / sizeof(ToolType)); i++) {
            if (ImGui::RadioButton(typeName[i], &m_SelectedTool, i)) {
                std::cout << typeName[i] << std::endl;
                std::cout << i << std::endl;
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
        
        // if (m_Size < 1) m_Size = 1;

        if (ImGui::IsKeyPressed(ImGuiKey_Semicolon, true)) {
            m_Size--;
        } else if (ImGui::IsKeyPressed(ImGuiKey_Apostrophe, true)) {
            m_Size++;
        }
        ImGui::End();
    }
}
