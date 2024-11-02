#pragma once

#include <iostream>

namespace FuncDoodle {
    static uint s_PlayTexId = 255;
    static uint s_PauseTexId = 255;
    static uint s_RewindTexId = 255;
    static uint s_EndTexId = 255;
    static void GlobalLoadImages(AssetLoader* assetLoader) {
        s_PlayTexId = assetLoader->LoadImage("play.png");
        s_PauseTexId = assetLoader->LoadImage("pause.png");
        s_RewindTexId = assetLoader->LoadImage("rewind.png");
        s_EndTexId = assetLoader->LoadImage("end.png");
    }
}
