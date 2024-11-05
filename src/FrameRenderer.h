#pragma once

#include "Frame.h"

#include "ToolManager.h"
#include "Grid.h"

namespace FuncDoodle {
    class FrameRenderer {
        public:
            FrameRenderer(Frame* frame, ToolManager* manager) : m_Frame(frame), m_ToolManager(manager), m_Grid(nullptr) {}
            ~FrameRenderer() { CleanupFrame(); }
            void RenderFrame();
            void InitPixels();

            __inline__ const Frame* AnimFrame() const { return m_Frame; }
            void SetFrame(Frame* frame) { m_Frame = frame; }
            __inline__ const ToolManager* GetToolManager() const { return m_ToolManager; }
            void SetToolManager(ToolManager* toolManager) { m_ToolManager = toolManager; }
            const int Index() const { return m_Index; }
            void SetIndex(int index) { m_Index = index; }
            void SetPreviousFrame(Frame* frame) { m_PreviousFrame = frame; }
            const Frame* PreviousFrame() const { return m_PreviousFrame; }

            void CleanupFrame() {
                if (m_Frame != nullptr) {
                    delete m_Frame;
                    m_Frame = nullptr;
                }
            }

            void FloodFill(int x, int y, Col targetCol, Col fillCol);
        private:
            Frame* m_Frame;
            Frame* m_PreviousFrame;
            int m_Index;
            ToolManager* m_ToolManager;
            int m_PixelScale = 8;
            ImVec2 m_LastMousePos = ImVec2(-1, -1);
            Grid* m_Grid;
    };
}
