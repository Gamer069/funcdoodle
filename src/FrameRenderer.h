#pragma once

#include "Frame.h"

namespace FuncDoodle {
    class FrameRenderer {
        public:
            FrameRenderer(Frame* frame, bool render = true) : m_Frame(frame) {
                m_HasRendered = !render;
            }
            ~FrameRenderer() {}
            void RenderFrame();
            void InitPixels();

            __inline__ const Frame* AnimFrame() const { return m_Frame; }
            void SetFrame(Frame* frame) { m_Frame = frame; }
            __inline__ const bool HasRendered() const { return m_HasRendered; }
            void SetHasRendered(bool hasRendered) { m_HasRendered = hasRendered; }
        private:
            Frame* m_Frame;
            bool m_HasRendered;
    };
}
