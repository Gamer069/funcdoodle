#include "TextRenderer.h"
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <imgui.h>

namespace FuncDoodle {
    GLuint shaderProgram = 0;  // Global shader program, could be moved to class members if necessary

    TextRenderer::TextRenderer() {
        // Simple shader initialization (vertex and fragment shader)
        const char* vertexShaderSource = R"(
            #version 330 core
            layout(location = 0) in vec2 aPos;
            layout(location = 1) in vec2 aTexCoord;
            out vec2 TexCoord;
            uniform mat4 model;
            void main() {
                gl_Position = model * vec4(aPos, 0.0, 1.0);
                TexCoord = aTexCoord;
            })";
        
        const char* fragmentShaderSource = R"(
            #version 330 core
            in vec2 TexCoord;
            out vec4 FragColor;
            uniform sampler2D text;
            uniform vec3 textColor;
            void main() {
                vec4 sampled = texture(text, TexCoord);
                FragColor = vec4(textColor, 1.0) * sampled.a;
            })";
        
        // Compile and link the shader program (vertex + fragment)
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Clean up shaders after linking
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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

        glUseProgram(shaderProgram);  // Use the shader program

        // Set the color in the shader
        GLint colorLoc = glGetUniformLocation(shaderProgram, "textColor");
        glUniform3f(colorLoc, txtCol.r / 255.0f, txtCol.g / 255.0f, txtCol.b / 255.0f);

        // Set the transformation matrix (center the text)
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        float offsetX = -0.5f;  // Adjust for centering horizontally
        float offsetY = 0.0f;   // Adjust for vertical centering
        float scaleX = 0.005f;
        float scaleY = 0.005f;

        float modelMatrix[16] = {
            scaleX, 0.0f, 0.0f, offsetX,
            0.0f, scaleY, 0.0f, offsetY,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        glUniformMatrix4fv(modelLoc, 1, GL_TRUE, modelMatrix);

        // Set OpenGL state for rendering text
        glBindTexture(GL_TEXTURE_2D, textureId);  // Bind the texture
        glEnable(GL_BLEND);  // Enable blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set blend function
        glDisable(GL_DEPTH_TEST);  // Disable depth test to ensure text appears on top

        // Start position for centering
        float x = offsetX;
        float y = offsetY;

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

        // Restore OpenGL state
        glEnable(GL_DEPTH_TEST);  // Re-enable depth test
        glDisable(GL_BLEND);  // Disable blending after rendering text
    }
}

