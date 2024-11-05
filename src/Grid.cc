#include "Grid.h"

#include "Gui.h"

#include <iostream>

namespace FuncDoodle
{
    void Grid::RenderWithDrawList(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos) {
        if (!m_ShowGrid) return;

        // Calculate cell sizes with rounding to avoid floating point imprecision
        float cellWidth = std::round((endPos.x - startPos.x) / m_GridWidth);
        float cellHeight = std::round((endPos.y - startPos.y) / m_GridHeight);

        // Draw vertical lines with rounded positions
        for (int x = 0; x <= m_GridWidth; x++) {
            float xPos = std::round(startPos.x + x * cellWidth) - 0.7f;
            drawList->AddLine(
                ImVec2(xPos, startPos.y),
                ImVec2(xPos, endPos.y),
                IM_COL32(0, 0, 0, 255)  // Gray color
            );
        }

        // Draw horizontal lines with rounded positions
        for (int y = 0; y <= m_GridHeight; y++) {
            float yPos = std::round(startPos.y + y * cellHeight) - 0.7f;
            drawList->AddLine(
                ImVec2(startPos.x, yPos),
                ImVec2(endPos.x, yPos),
                IM_COL32(0, 0, 0, 255)  // Gray color
            );
        }
    }
    Grid::Grid(int gridWidth, int gridHeight) : m_GridWidth(gridWidth), m_GridHeight(gridHeight), m_ShowGrid(false) {}
    Grid::~Grid() {}
}