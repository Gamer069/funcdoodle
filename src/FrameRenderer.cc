#include "FrameRenderer.h"

#include "Gui.h"

#include "Frame.h"

#include <iostream>

namespace FuncDoodle {
    void FrameRenderer::RenderFrame() {
        ImGui::Begin("Frame");
//        const auto col = m_Frame.Pixels()->get(1,1);
        const ImageArray* pixels = m_Frame.Pixels();

        InitPixels();

        ImGui::End();
    }
    void FrameRenderer::InitPixels() {
        const int pixelWidth = 1;
        const int pixelHeight = 1;
        // get the imgui draw list
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImVec2 topLeft = ImGui::GetCursorScreenPos();
        ImVec2 bottomRight = ImVec2(topLeft.x + pixelWidth, topLeft.y + pixelHeight);

        for (int y = 0; y < pixels->getHeight(); y++) {
            for (int x = 0; x < pixels->getWidth(); x++) {
                const Col col = pixels->get(x,y);
                drawList->AddRectFilled(ImVec2(topLeft.x+x, topLeft.y+x), ImVec2(bottomRight.x+y, bottomRight.y+y), IM_COL32(col.r, col.g, col.b, 255));
            }
        }
    }
}
