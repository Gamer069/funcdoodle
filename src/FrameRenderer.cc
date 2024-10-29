#include "FrameRenderer.h"

#include "Gui.h"

#include "Frame.h"

#include <iostream>

namespace FuncDoodle {
    void FrameRenderer::RenderFrame() {
        ImGui::Begin("Frame", nullptr, ImGuiWindowFlags_NoMove);
        // Add a custom title bar area that allows movement
        if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            ImVec2 pos = ImGui::GetWindowPos();
            ImGui::SetWindowPos(ImVec2(pos.x + delta.x, pos.y + delta.y));
        }

        if (!m_Frame || !m_ToolManager) return;
        InitPixels();

        // set has rendered to false if changing the actual pixel data

        ImGui::End();
    }
    void FrameRenderer::InitPixels() {
        const ImageArray* pixels = m_Frame->Pixels();

        // Get window dimensions before handling zoom
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();

        // Handle zoom input with window size constraints
        if (ImGui::IsWindowFocused()) {
            if (ImGui::IsKeyPressed(ImGuiKey_Equal)) { // = key for zoom in
                // Calculate max scale that would fit in window
                int maxScaleWidth = static_cast<int>(contentRegion.x / pixels->getWidth());
                int maxScaleHeight = static_cast<int>(contentRegion.y / pixels->getHeight());
                int maxAllowedScale = std::min(maxScaleWidth, maxScaleHeight);
                
                m_PixelScale += 1;
                m_PixelScale = std::min(maxAllowedScale, m_PixelScale);
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Minus)) { // - key for zoom out  
                m_PixelScale -= 1;
                m_PixelScale = std::max(1, m_PixelScale); // Minimum of 1
            }
        }

        // Calculate total frame dimensions
        float frameWidth = pixels->getWidth() * m_PixelScale;
        float frameHeight = pixels->getHeight() * m_PixelScale;

        // Calculate starting position to center the frame
        float startX = windowPos.x + (contentRegion.x - frameWidth) * 0.5f;
        float startY = windowPos.y + (contentRegion.y - frameHeight) * 0.5f;

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (int y = 0; y < pixels->getHeight(); y++) {
            for (int x = 0; x < pixels->getWidth(); x++) {
                const Col col = pixels->get(x, y);
                
                // Calculate pixel positions
                ImVec2 topLeft(startX + x * m_PixelScale, startY + y * m_PixelScale);
                ImVec2 bottomRight(topLeft.x + m_PixelScale, topLeft.y + m_PixelScale);

                // Handle mouse input
                if (ImGui::IsMouseHoveringRect(topLeft, bottomRight) && ImGui::IsMouseDown(0)) {
                    if (m_ToolManager == nullptr) {
                        std::cerr << "???????" << std::endl;
                        std::exit(-1);
                    }

                    if (m_ToolManager->SelectedTool() == 0) {
                        const float* colOld = m_ToolManager->Col();
                        unsigned char colNew[3] = { (unsigned char)0, (unsigned char)0, (unsigned char)0 };

                        for (int i = 0; i < 3; i++) {
                            colNew[i] = static_cast<unsigned char>(colOld[i] * 255.0f + 0.5f);
                        }

                        m_Frame->SetPixel(x, y, Col { .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                    } else if (m_ToolManager->SelectedTool() == 1) {
                        m_Frame->SetPixel(x, y, Col { .r = 255, .g = 255, .b = 255 });
                    }
                }

                // Draw pixel
                drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, 255));
            }
        }
    }
}
