#include "FrameRenderer.h"

#include "Gui.h"

#include "Frame.h"

#include <iostream>

namespace FuncDoodle {
    void FrameRenderer::RenderFrame() {
        ImGui::Begin("Frame");
//        const auto col = m_Frame.Pixels()->get(1,1);

        InitPixels();

        // set has rendered to false if changing the actual pixel data

        ImGui::End();
    }
    void FrameRenderer::InitPixels() {
        const ImageArray* pixels = m_Frame->Pixels();

        const int pixelWidth = 2;
        const int pixelHeight = 2;
        // get the imgui draw list
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImVec2 topLeft = ImGui::GetCursorScreenPos();
        ImVec2 bottomRight = ImVec2(topLeft.x + pixelWidth, topLeft.y + pixelHeight);

        for (int y = 0; y < pixels->getHeight(); y++) {
            for (int x = 0; x < pixels->getWidth(); x++) {
                const Col col = pixels->get(x,y);
                // std::cout << "r:" << (int)col.r << "g:" << (int)col.g << "b:" << (int)col.b << std::endl;
                // Hover check hehe
                if (ImGui::IsMouseHoveringRect(ImVec2(topLeft.x+x, topLeft.y+y), ImVec2(bottomRight.x+x, bottomRight.y+y)) && ImGui::IsMouseDown(0)) {
                    if (m_ToolManager->SelectedTool() == 0) {
                        const float* colOld = m_ToolManager->Col();
                        unsigned char colNew[3] = { (unsigned char)0, (unsigned char)0, (unsigned char)0 };

                        for (int i = 0; i < 3; i++) {
                            colNew[i] = static_cast<unsigned char>(colOld[i] * 255.0f + 0.5f);
                        }

                        m_Frame->SetPixel(x, y, Col { .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                    } else if (m_ToolManager->SelectedTool() == 1) {
                        m_Frame->SetPixel(x, y, Col());
                    };
                }

                drawList->AddRectFilled(ImVec2(topLeft.x+x, topLeft.y+y), ImVec2(bottomRight.x+x, bottomRight.y+y), IM_COL32(col.r, col.g, col.b, 255));

            }
        }
    }
}
