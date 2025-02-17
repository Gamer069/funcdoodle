#pragma once

#include <filesystem>

#include "Audio.h"

namespace FuncDoodle {
	class AudioManager {
		public:
			AudioManager() {}
			~AudioManager() {}
			void PlayWav(AudioData wavPath);
			AudioData ParseWav(std::filesystem::path wav);
	};
};
