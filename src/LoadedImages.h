#pragma once

#include "LoadedImages.h"

#include <iostream>

#include "AssetLoader.h"

namespace FuncDoodle {
    extern uint s_PlayTexId;
    extern uint s_PauseTexId;
    extern uint s_RewindTexId;
    extern uint s_EndTexId;
    extern uint s_BucketTexId;
    extern uint s_PencilTexId;
    extern uint s_PickerTexId;
    extern uint s_EraserTexId;
    extern uint s_TextTexId;
    void GlobalLoadImages(AssetLoader* assetLoader);
}
