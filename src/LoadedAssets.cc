#include "LoadedAssets.h"

#include <cstdint>

namespace FuncDoodle {
	uint32_t s_PlayTexId = 0;
	uint32_t s_PauseTexId = 0;
	uint32_t s_RewindTexId = 0;
	uint32_t s_EndTexId = 0;
	uint32_t s_PencilTexId = 0;
	uint32_t s_PickerTexId = 0;
	uint32_t s_EraserTexId = 0;
	uint32_t s_BucketTexId = 0;
	uint32_t s_TextTexId = 0;
	uint32_t s_AddTexId = 0;
	uint32_t s_OpenTexId = 0;
	AudioData s_ProjCreateSound;
	AudioData s_ProjSaveSound;
	AudioData s_ExportSound;
	AudioData s_ProjSaveEndSound;
	void GlobalLoadAssets(AssetLoader* assetLoader) {
		// images
		s_PlayTexId = assetLoader->LoadImage("play.png");
		s_PauseTexId = assetLoader->LoadImage("pause.png");
		s_RewindTexId = assetLoader->LoadImage("rewind.png");
		s_EndTexId = assetLoader->LoadImage("end.png");
		s_PencilTexId = assetLoader->LoadImage("pencil.png");
		s_PickerTexId = assetLoader->LoadImage("picker.png");
		s_EraserTexId = assetLoader->LoadImage("eraser.png");
		s_BucketTexId = assetLoader->LoadImage("bucket.png");
		s_TextTexId = assetLoader->LoadImage("text.png");
		s_AddTexId = assetLoader->LoadImage("add.png");
		s_OpenTexId = assetLoader->LoadImage("folder.png");

		// audio
		s_ProjCreateSound = assetLoader->ParseSound("create.wav");
		s_ProjSaveSound = assetLoader->ParseSound("save.wav");
		s_ExportSound = assetLoader->ParseSound("export.wav");
		s_ProjSaveEndSound = assetLoader->ParseSound("saveend.wav");
	}
}  // namespace FuncDoodle
