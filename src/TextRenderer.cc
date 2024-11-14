#include "TextRenderer.h"

#include <vector>
#include "Frame.h"
#include "Gui.h"
#include <glad/glad.h>

#include <iostream>

#include <string>

namespace FuncDoodle {
    TextRenderer::TextRenderer(ImFont* fnt) : m_Font(fnt) {
    }
    TextRenderer::~TextRenderer() {
        free(m_Font);
    }
    const std::vector<Col>& TextRenderer::RenderText(const char* txt, const Col& txtCol) {
        // calculate text pixels
        
        // calculate font size -- eg. 16x16, etc.
        ImVec2 textSize = ImGui::CalcTextSize(txt, 0, false, -1.0f);
        std::cout << "TEXT SIZE: " << textSize.x << ", " << textSize.y << std::endl;
        const int textLen = std::strlen(txt);
        // store in bools: 1 for filled pixel, 0 for transparent / non filled
        ImFontAtlas* atlas = m_Font->ContainerAtlas;
        void* texId = atlas->TexID;
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)texId);
        float x = 0;
        float y = 0;
        for (const char* c = txt; *c; c++) {
            const ImFontGlyph* glyph = m_Font->FindGlyph(*c);
            if (!glyph) continue; // skip glyph if not found
            const int width = glyph->X1 - glyph->X0;
            const int height = glyph->Y1 - glyph->Y0;
            const int u0 = glyph->U0;
            const int v0 = glyph->V0;
            const int u1 = glyph->U1;
            const int v1 = glyph->V1;
            const int spacing = glyph->AdvanceX;
            glBegin(GL_QUADS);
            glTexCoord2f(u0, v0); glVertex2f(x, y);
            glTexCoord2f(u1, v0); glVertex2f(x + width, y);
            glTexCoord2f(u1, v1); glVertex2f(x + width, y + height);
            glTexCoord2f(u0, v1); glVertex2f(x, y + height);
            glEnd();

            x += glyph->AdvanceX; // our beautiful line
            // loop over each char
        }

        // mask the color and the bits.
    }
}
