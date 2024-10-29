#pragma once

#include "Gui.h"

#include "Tool.h"

namespace FuncDoodle {
    class ToolManager {
        public:
            ToolManager();
            ~ToolManager();
            void RenderTools();
            const int SelectedTool() { return m_SelectedTool; }
            const float* Col() { return m_Col; }
        private:
            int m_SelectedTool = 0;
            float m_Col[3] = {0.0f, 0.0f, 0.0f};
    };
}
