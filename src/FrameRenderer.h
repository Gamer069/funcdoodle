#pragma once

#include "Frame.h"

namespace FuncDoodle {
    class FrameRenderer {
        public:
            FrameRenderer(Frame frame) : m_Frame(frame) {}
            ~FrameRenderer() {}
            void RenderFrame();
            void InitPixels();
        private:
            Frame m_Frame;
    };
}
