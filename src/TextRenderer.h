#pragma once

#include <vector>
#include "Frame.h"
#include "Gui.h"

namespace FuncDoodle {
    class TextRenderer {
        public:
            TextRenderer();
            ~TextRenderer();
            void RenderText(const char* text, const Col& txtColor);
        private:
            ImFont* m_Font = nullptr;
    };
}
