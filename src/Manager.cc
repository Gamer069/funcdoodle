#include "Manager.h"

#include "Gui.h"

#include "Project.h"

#include "DynArr.h"

#include "FrameRenderer.h"

#include "ToolManager.h"

#include <string>
#include <iostream>
#include <random>

namespace FuncDoodle {
    AnimationManager::AnimationManager(ProjectFile* proj) : m_Proj(proj), m_SelectedFrame(0), m_ToolManager(new ToolManager()) { m_FrameRenderer = new FrameRenderer(nullptr, m_ToolManager); }
    AnimationManager::~AnimationManager() {}
    void AnimationManager::RenderTimeline() {
        // Set scrollbar size (thickness)
        ImGui::GetStyle().ScrollbarSize = 20.0f;  // Increase the thickness of the scrollbars

        // Lock window height but allow horizontal resizing
        float fixedHeight = 160.0f;
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, fixedHeight), ImVec2(FLT_MAX, fixedHeight));

        // Begin the window with horizontal scrollbar enabled
        ImGui::Begin("Timeline", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Define frame size and padding
        float frameWidth = 200.0f;
        float frameHeight = 100.0f;
        float padding = 25.0f;

        // Calculate total width required for all frames
        float totalWidth = m_Proj->AnimFrameCount() * (frameWidth + padding);

        // Create a scrollable region
        ImGui::BeginChild("FrameScrollRegion", ImVec2(ImGui::GetContentRegionAvail().x, frameHeight + padding), false, ImGuiWindowFlags_HorizontalScrollbar);

        // Get the initial top-left position
        ImVec2 topLeft = ImGui::GetCursorScreenPos();
        ImVec2 bottomRight = ImVec2(topLeft.x + frameWidth, topLeft.y + frameHeight);

        ImFont* font = ImGui::GetFont();
        float fontSize = ImGui::GetFontSize();

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftBracket))) {
            if (m_SelectedFrame > 0) m_SelectedFrame--;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightBracket))) {
            if (m_SelectedFrame < m_Proj->AnimFrameCount()-1) ++m_SelectedFrame;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_P))) {
            Frame frame = Frame(m_Proj->AnimWidth(), m_Proj->AnimHeight());
            m_Proj->AnimFrames()->insertBefore(m_SelectedFrame, frame);
            m_SelectedFrame++;
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O))) {
            Frame frame = Frame(m_Proj->AnimWidth(), m_Proj->AnimHeight());
            m_Proj->AnimFrames()->insertBefore(m_SelectedFrame, frame);
        }

        // Render frames
        for (long i = 0; i < m_Proj->AnimFrameCount(); i++) {
            drawList->AddText(font, fontSize, m_SelectedFrame == i ? ImVec2(topLeft.x + frameWidth/2, bottomRight.y + 10) : ImVec2(topLeft.x + frameWidth/2, bottomRight.y), IM_COL32(255, 255, 255, 255), std::to_string(i).c_str());

            drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(255, 255, 255, 255));
            if (m_SelectedFrame == i) {
                drawList->AddRect(ImVec2(topLeft.x - 3, topLeft.y - 3), ImVec2(bottomRight.x + 3, bottomRight.y + 3), IM_COL32(255, 0, 0, 255), 0.0f, ImDrawFlags_None, 5.0f);
                const auto frames = m_Proj->AnimFrames();
                auto frame = frames->get(i);
                m_FrameRenderer->SetFrame(&frame);
                m_FrameRenderer->RenderFrame();
            }
            topLeft.x += frameWidth + padding;
            bottomRight.x += frameWidth + padding;
        }

        // Ensure the scroll region size is based on total width of all frames
        ImGui::Dummy(ImVec2(totalWidth - 25, frameHeight));

        ImGui::EndChild();
        ImGui::End();

        m_ToolManager->RenderTools();
    }
}
