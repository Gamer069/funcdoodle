#pragma once

#include "Gui.h"

#include "Tool.h"

namespace FuncDoodle {
    class ToolManager {
        public:
            ToolManager();
            ~ToolManager();
            void RenderTools();
            __inline__ const int SelectedTool() const { return m_SelectedTool; }
            __inline__ const float* Col() const { return m_Col; }
        private:
            int m_SelectedTool;
            float m_Col[3] = {0.0f, 0.0f, 0.0f};
    };
}
