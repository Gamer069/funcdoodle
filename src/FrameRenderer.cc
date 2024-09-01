#include "FrameRenderer.h"

#include "Gui.h"

#include "Frame.h"

#include <iostream>

namespace FuncDoodle {
    void FrameRenderer::RenderFrame() {
        ImGui::Begin("Frame");
        const auto col = m_Frame.Pixels()->get(1,1);
        std::cout << "Color: RGB(" << col.r << "," << col.g << "," << col.b << ")" << std::endl;
        std::cout << "Render frame" << std::endl;
        ImGui::End();
    }
}
