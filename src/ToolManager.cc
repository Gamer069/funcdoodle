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
        if (m_SelectedTool == 0 || m_SelectedTool == 2 || m_SelectedTool == 3) {
            ImGui::ColorPicker3("Col", m_Col);
        }
        ImGui::End();
    }
}
