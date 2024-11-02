#pragma once

#include "Frame.h"

#include <functional>

namespace FuncDoodle {
    enum class ToolType {
        Pencil,
        Eraser,
        Bucket,
        Picker
    };
    typedef struct {
        ToolType Type;
        int Size = 1; // pixel
        Col* Col;
        std::function<void(int, int, Frame*)> Callback;
    } Tool;
}
