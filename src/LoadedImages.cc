#include "LoadedImages.h"

namespace FuncDoodle {
    uint s_PlayTexId = 255;
    uint s_PauseTexId = 255;
    uint s_RewindTexId = 255;
    uint s_EndTexId = 255;
    uint s_PencilTexId = 255;
    uint s_PickerTexId = 255;
    uint s_EraserTexId = 255;
    uint s_BucketTexId = 255;
    void GlobalLoadImages(AssetLoader* assetLoader) {
        s_PlayTexId = assetLoader->LoadImage("play.png");
        s_PauseTexId = assetLoader->LoadImage("pause.png");
        s_RewindTexId = assetLoader->LoadImage("rewind.png");
        s_EndTexId = assetLoader->LoadImage("end.png");
        s_PencilTexId = assetLoader->LoadImage("pencil.png");
        s_PickerTexId = assetLoader->LoadImage("picker.png");
        s_EraserTexId = assetLoader->LoadImage("eraser.png");
        s_BucketTexId = assetLoader->LoadImage("bucket.png");
    }
}
