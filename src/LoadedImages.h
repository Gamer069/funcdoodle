#pragma once

#include "LoadedImages.h"

#include <iostream>

#include "AssetLoader.h"

namespace FuncDoodle {
	extern uint32_t s_PlayTexId;
	extern uint32_t s_PauseTexId;
	extern uint32_t s_RewindTexId;
	extern uint32_t s_EndTexId;
	extern uint32_t s_BucketTexId;
	extern uint32_t s_PencilTexId;
	extern uint32_t s_PickerTexId;
	extern uint32_t s_EraserTexId;
	extern uint32_t s_TextTexId;
	extern uint32_t s_AddTexId;
	extern uint32_t s_OpenTexId;
	void GlobalLoadImages(AssetLoader* assetLoader);
}  // namespace FuncDoodle
