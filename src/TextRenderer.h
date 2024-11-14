#pragma once

#include <vector>
#include "Frame.h"
#include "Gui.h"

namespace FuncDoodle {
    class TextRenderer {
        public:
            TextRenderer(ImFont* fnt);
            ~TextRenderer();
            const std::vector<Col>& RenderText(const char* text, const Col& txtColor);
        private:
            ImFont* m_Font;
    };
}
