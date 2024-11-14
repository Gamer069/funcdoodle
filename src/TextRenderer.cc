#include "TextRenderer.h"
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <imgui.h>

namespace FuncDoodle {
    TextRenderer::TextRenderer() {
        // No shader initialization here, using the font texture directly
    }

    TextRenderer::~TextRenderer() {
        // No cleanup needed for shaders
    }

    void TextRenderer::RenderText(const char* txt, const Col& txtCol) {
        if (!ImGui::GetFont()) {
            std::cout << "FONT TEXT RENDERER NULLPTR?" << std::endl;
            return;
        }

        ImFont* font = ImGui::GetFont();
        ImFontAtlas* atlas = font->ContainerAtlas;
        void* texId = atlas->TexID;

        if (!texId) {
            std::cout << "Atlas texture ID is NULL!" << std::endl;
            return;
        }

        GLuint textureId = static_cast<GLuint>(reinterpret_cast<intptr_t>(texId));

        if (textureId == 0) {
            std::cout << "Invalid texture ID!" << std::endl;
            return;
        }

        // Set OpenGL state for rendering text
        glBindTexture(GL_TEXTURE_2D, textureId);  // Bind the texture
        glEnable(GL_BLEND);  // Enable blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set blend function
        glDisable(GL_DEPTH_TEST);  // Disable depth test to ensure text appears on top

        // Set text color (since A is always 1.0, we only use RGB values)
        glColor3f(txtCol.r / 255.0f, txtCol.g / 255.0f, txtCol.b / 255.0f);

        // Start position
        float x = 0.0f;
        float y = 0.0f;

        // Loop through each character in the text string
        for (const char* c = txt; *c; c++) {
            const ImFontGlyph* glyph = font->FindGlyph(*c);
            if (!glyph) continue;  // Skip characters that don't have a glyph

            // Calculate the position and size based on the glyph data
            float xpos = x + glyph->X0;
            float ypos = y + glyph->Y0;
            float w = glyph->X1 - glyph->X0;
            float h = glyph->Y1 - glyph->Y0;

            // Render the character at the calculated position and size
            glBegin(GL_QUADS);
                glTexCoord2f(glyph->U0, glyph->V1); glVertex2f(xpos, ypos + h);
                glTexCoord2f(glyph->U0, glyph->V0); glVertex2f(xpos, ypos);
                glTexCoord2f(glyph->U1, glyph->V0); glVertex2f(xpos + w, ypos);
                glTexCoord2f(glyph->U1, glyph->V1); glVertex2f(xpos + w, ypos + h);
            glEnd();

            // Increment x position for the next character
            x += glyph->AdvanceX;
        }

        // Restore OpenGL state if needed
        glEnable(GL_DEPTH_TEST);  // Re-enable depth test (if required)
        glDisable(GL_BLEND);  // Disable blending after rendering text
    }
}

