#include "FrameRenderer.h"

#include "Gui.h"

#include "Frame.h"

#include <iostream>

namespace FuncDoodle
{
    void FrameRenderer::RenderFrame()
    {
        ImGui::SetNextWindowPos(ImVec2(0, 32));
        ImGui::SetNextWindowSize(ImVec2(1073, 886));
        ImGui::Begin("Frame");

        if (!m_Frame || !m_ToolManager)
            return;

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Zoom out", "-")) {
                m_PixelScale--;
            }
            if (ImGui::MenuItem("Zoom equal", "0")) {
                m_PixelScale = 1;
            }
            if (ImGui::MenuItem("Zoom in", "=")) {
                m_PixelScale++;
            }
            ImGui::EndPopup();
        }
        InitPixels();

        ImGui::End();
    }
    void FrameRenderer::InitPixels()
    {
        const ImageArray* pixels = m_Frame->Pixels();

        // Get window dimensions before handling zoom
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();

        // Handle zoom input with window size constraints
        if (ImGui::IsWindowFocused())
        {
            if (ImGui::IsKeyPressed(ImGuiKey_Equal))
            { // = key for zoom in
                m_PixelScale += 1;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Minus))
            { // - key for zoom out
                m_PixelScale -= 1;
                m_PixelScale = std::max(1, m_PixelScale); // Minimum of 1
            }

            if (ImGui::IsKeyPressed(ImGuiKey_0)) {
                m_PixelScale = 1;
            }
        }

        // Calculate total frame dimensions
        float frameWidth = pixels->getWidth() * m_PixelScale;
        float frameHeight = pixels->getHeight() * m_PixelScale;

        // Calculate starting position to center the frame
        float startX = windowPos.x + (contentRegion.x - frameWidth) * 0.5f;
        float startY = windowPos.y + (contentRegion.y - frameHeight) * 0.5f;

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Handle mouse input first
        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 frameMin(startX, startY);
        ImVec2 frameMax(startX + frameWidth, startY + frameHeight);

        // Check if mouse is within frame bounds and mouse button is down
        if (ImGui::IsMouseHoveringRect(frameMin, frameMax) && ImGui::IsMouseDown(0))
        {
            if (m_ToolManager == nullptr)
            {
                std::cerr << "???????" << std::endl;
                std::exit(-1);
            }

            // Calculate current pixel coordinates
            ImVec2 currentPixel(
                (mousePos.x - startX) / m_PixelScale,
                (mousePos.y - startY) / m_PixelScale);

            // Draw at current position
            if (currentPixel.x >= 0 && currentPixel.x < pixels->getWidth() &&
                currentPixel.y >= 0 && currentPixel.y < pixels->getHeight())
            {
                int selectedTool = m_ToolManager->SelectedTool();

                // If we have a valid last position, interpolate
                if (m_LastMousePos.x >= 0 && m_LastMousePos.y >= 0)
                {
                    float dx = currentPixel.x - m_LastMousePos.x;
                    float dy = currentPixel.y - m_LastMousePos.y;
                    int steps = std::max(1, std::max(abs(static_cast<int>(dx)), abs(static_cast<int>(dy))));

                    // Pre-calculate color values outside the loop
                    unsigned char colNew[3] = { 255, 255, 255 }; // Default white for eraser
                    if (selectedTool == 0)
                    {
                        const float* colOld = m_ToolManager->Col();
                        for (int j = 0; j < 3; j++)
                        {
                            colNew[j] = static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
                        }

                        int size = m_ToolManager->Size();

                        for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
                            for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
                                int newX = currentPixel.x + offsetX;
                                int newY = currentPixel.y + offsetY;
                                if (newX >= 0 && newX < m_Frame->Pixels()->getWidth() &&
                                    newY >= 0 && newY < m_Frame->Pixels()->getHeight()) {
                                    m_Frame->SetPixel(newX, newY, Col{ .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                                }
                            }
                        }
                    }
                    else if (selectedTool == 1) {
                        int size = m_ToolManager->Size();

                        for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
                            for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
                                int newX = currentPixel.x + offsetX;
                                int newY = currentPixel.y + offsetY;
                                if (newX >= 0 && newX < m_Frame->Pixels()->getWidth() &&
                                    newY >= 0 && newY < m_Frame->Pixels()->getHeight()) {
                                    m_Frame->SetPixel(newX, newY, Col{ .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                                }
                            }
                        }
                    }
                    else if (selectedTool == 2) {
                        const float* colOld = m_ToolManager->Col();
                        unsigned char colResult[3];
                        for (int j = 0; j < 3; j++) {
                            colResult[j] = static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
                        }
                        // FLOODFILL
                        Col curPixelCol = pixels->get(currentPixel.x, currentPixel.y);
                        FloodFill(currentPixel.x, currentPixel.y, curPixelCol, Col{ .r = colResult[0], .g = colResult[1], .b = colResult[2] });
                        colNew[0] = colResult[0];
                        colNew[1] = colResult[1];
                        colNew[2] = colResult[2];
                    }
                    else if (selectedTool == 3) {
                        m_ToolManager->SetCol(m_Frame->Pixels()->get(currentPixel.x, currentPixel.y));
                        Col curPxCol = m_Frame->Pixels()->get(currentPixel.x, currentPixel.y);
                        colNew[0] = curPxCol.r;
                        colNew[1] = curPxCol.g;
                        colNew[2] = curPxCol.b;
                        std::cout << "CURRENT PIXEL: " << currentPixel.x << ", " << currentPixel.y << std::endl;
                        steps = 0;
                    }

                    for (int i = 0; i <= steps; i++)
                    {
                        float t = static_cast<float>(i) / steps;
                        int interpX = static_cast<int>(m_LastMousePos.x + dx * t);
                        int interpY = static_cast<int>(m_LastMousePos.y + dy * t);

                        if (interpX == 0 && interpY == 0)
                        {
                            std::cout << "STEPS -- " << steps << '\n';
                            std::cout << "Last mouse pos (0) -> " << m_LastMousePos.x << "," << m_LastMousePos.y << "." << '\n';
                            std::cout << "D -- " << dx << "," << dy << '\n';
                            std::cout << "T -- " << t << '\n';
                        }

                        if (interpX >= 0 && interpX < pixels->getWidth() &&
                            interpY >= 0 && interpY < pixels->getHeight())
                        {
                            m_Frame->SetPixel(interpX, interpY, Col{ .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                        }
                    }
                }
                else
                {
                    // Draw single pixel if no last position
                    unsigned char colNew[3] = { 255, 255, 255 }; // Default white for eraser
                    if (selectedTool == 0)
                    {
                        const float* colOld = m_ToolManager->Col();
                        for (int j = 0; j < 3; j++)
                        {
                            colNew[j] = static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
                        }
                        int size = m_ToolManager->Size();

                        for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
                            for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
                                int newX = currentPixel.x + offsetX;
                                int newY = currentPixel.y + offsetY;
                                if (newX >= 0 && newX < m_Frame->Pixels()->getWidth() &&
                                    newY >= 0 && newY < m_Frame->Pixels()->getHeight()) {
                                    m_Frame->SetPixel(newX, newY, Col{ .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                                }
                            }
                        }
                    }
                    else if (selectedTool == 1) {
                        int size = m_ToolManager->Size();

                        for (int offsetY = -size / 2; offsetY <= size / 2; offsetY++) {
                            for (int offsetX = -size / 2; offsetX <= size / 2; offsetX++) {
                                int newX = currentPixel.x + offsetX;
                                int newY = currentPixel.y + offsetY;
                                if (newX >= 0 && newX < m_Frame->Pixels()->getWidth() &&
                                    newY >= 0 && newY < m_Frame->Pixels()->getHeight()) {
                                    m_Frame->SetPixel(newX, newY, Col{ .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                                }
                            }
                        }
                    }
                    else if (selectedTool == 2) {
                        const float* colOld = m_ToolManager->Col();
                        unsigned char colResult[3];
                        for (int j = 0; j < 3; j++) {
                            colResult[j] = static_cast<unsigned char>(colOld[j] * 255.0f + 0.5f);
                        }
                        // FLOODFILL
                        Col curPixelCol = pixels->get(currentPixel.x, currentPixel.y);
                        FloodFill(currentPixel.x, currentPixel.y, curPixelCol, Col{ .r = colResult[0], .g = colResult[1], .b = colResult[2] });
                        colNew[0] = colResult[0];
                        colNew[1] = colResult[1];
                        colNew[2] = colResult[2];
                    }
                    else if (selectedTool == 3) {
                        // For color picker, we just pick the color - no drawing
                        m_ToolManager->SetCol(pixels->get(currentPixel.x, currentPixel.y));
                        colNew[0] = pixels->get(currentPixel.x, currentPixel.y).r;
                        colNew[1] = pixels->get(currentPixel.x, currentPixel.y).g;
                        colNew[2] = pixels->get(currentPixel.x, currentPixel.y).b;
                    }

                    if (selectedTool != 3) { // Only draw if not using color picker
                        m_Frame->SetPixel(currentPixel.x, currentPixel.y, Col{ .r = colNew[0], .g = colNew[1], .b = colNew[2] });
                    }
                }
                m_LastMousePos = currentPixel;
            }
        }
        else if (!ImGui::IsMouseDown(0))
        {
            m_LastMousePos = ImVec2(-1, -1);
        }

        for (int y = 0; y < pixels->getHeight(); y++) {
            for (int x = 0; x < pixels->getWidth(); x++) {
                Col col = pixels->get(x, y);
                ImVec2 topLeft(startX + x * m_PixelScale, startY + y * m_PixelScale);
                ImVec2 bottomRight(startX + (x + 1) * m_PixelScale, startY + (y + 1) * m_PixelScale);
                drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, 255));
            }
        }

        // Draw the previous frame with transparency if we're not on the first frame
        if (m_Index > 0 && m_PreviousFrame != nullptr) {
            const ImageArray* prevPixels = m_PreviousFrame->Pixels();
            
            // Draw each pixel of the previous frame with 50% transparency
            for (int y = 0; y < prevPixels->getHeight(); y++) {
                for (int x = 0; x < prevPixels->getWidth(); x++) {
                    Col col = prevPixels->get(x, y);
                    std::cout << (int)col.r << ", " << (int)col.g << ", " << (int)col.b << std::endl;
                    ImVec2 topLeft(startX + x * m_PixelScale, startY + y * m_PixelScale);
                    ImVec2 bottomRight(startX + (x + 1) * m_PixelScale, startY + (y + 1) * m_PixelScale);
                    drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(col.r, col.g, col.b, 255/2));
                }
            }
            // std::exit(-1);
        }
    }

    void FrameRenderer::FloodFill(int x, int y, Col targetCol, Col fillCol) {
        std::cout << "FLOODFILL TIME YE BABY" << std::endl;
        if (x < 0 || x >= m_Frame->Pixels()->getWidth() || y < 0 || y >= m_Frame->Pixels()->getHeight())
            return;

        if (m_Frame->Pixels()->get(x, y) != targetCol || m_Frame->Pixels()->get(x, y) == fillCol)
            return;

        m_Frame->SetPixel(x, y, fillCol);

        FloodFill(x + 1, y, targetCol, fillCol);
        FloodFill(x - 1, y, targetCol, fillCol);
        FloodFill(x, y + 1, targetCol, fillCol);
        FloodFill(x, y - 1, targetCol, fillCol);
    }
}
