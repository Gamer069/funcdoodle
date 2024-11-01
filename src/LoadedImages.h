#pragma once

#include <iostream>

namespace FuncDoodle {
    static uint s_PlayTexId;
    static uint s_PauseTexId;
    static uint s_RewindTexId;
    static uint s_EndTexId;
    static void GlobalLoadImages(AssetLoader* assetLoader) {
        std::cout << "LOAD images" << std::endl;
        std::cout << (int)(assetLoader == nullptr) << std::endl;
        s_PlayTexId = assetLoader->LoadImage("play.png");
        s_PauseTexId = assetLoader->LoadImage("pause.png");
        s_RewindTexId = assetLoader->LoadImage("rewind.png");
        s_EndTexId = assetLoader->LoadImage("end.png");
    }
}
