#pragma once

#include "Frame.h"

#include "ToolManager.h"

namespace FuncDoodle {
    class FrameRenderer {
        public:
            FrameRenderer(Frame* frame, ToolManager* manager) : m_Frame(frame), m_ToolManager(manager) {}
            ~FrameRenderer() { CleanupFrame(); }
            void RenderFrame();
            void InitPixels();

            __inline__ const Frame* AnimFrame() const { return m_Frame; }
            void SetFrame(Frame* frame) { m_Frame = frame; }
            __inline__ const ToolManager* GetToolManager() const { return m_ToolManager; }
            void SetToolManager(ToolManager* toolManager) { m_ToolManager = toolManager; }

            void CleanupFrame() {
                if (m_Frame != nullptr) {
                    delete m_Frame;
                    m_Frame = nullptr;
                }
            }
        private:
            Frame* m_Frame;
            ToolManager* m_ToolManager;
            int m_PixelScale = 8;
            ImVec2 m_LastMousePos = ImVec2(-1, -1);
    };
}
