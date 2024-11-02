#pragma once

#include "Gui.h"

#include "Tool.h"

#include "Frame.h"

namespace FuncDoodle {
    class ToolManager {
        public:
            ToolManager();
            ~ToolManager();
            void RenderTools();
            const int SelectedTool() { return m_SelectedTool; }
            const float* Col() { return m_Col; }
            void SetCol(struct Col col) { 
                unsigned char colArr[3] = {col.r, col.g, col.b};
                for (int j = 0; j < 3; j++) {
                    m_Col[j] = static_cast<unsigned char>(colArr[j] * 255.0f + 0.5f);
                }   
            }
        private:
            int m_SelectedTool = 0;
            float m_Col[3] = {0.0f, 0.0f, 0.0f};
    };
}
