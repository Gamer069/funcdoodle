#include "AudioManager.h"
#include "Audio.h"

#include <af/AudioFile.h>
#include <chrono>
#include <filesystem>
#include <thread>

namespace FuncDoodle {
	// Plays the provided AudioData asynchronously.
	void AudioManager::PlayWav(AudioData wav) {
		// Allocate a heap copy so that the callback has a valid pointer
		AudioData* audioData = new AudioData(std::move(wav));

		// Initialize PortAudio
		PaError err = Pa_Initialize();
		if (err != paNoError) {
			std::cerr << "PortAudio initialization error: "
					  << Pa_GetErrorText(err) << std::endl;
			delete audioData;
			return;
		}

		PaStream* stream = nullptr;
		err = Pa_OpenDefaultStream(&stream,
								   0,						// no input channels
								   audioData->numChannels,	// output channels
								   paFloat32,				// sample format
								   audioData->sampleRate,
								   paFramesPerBufferUnspecified,
								   paCB,		// callback function
								   audioData);	// user data
		if (err != paNoError) {
			std::cerr << "Error opening default stream: "
					  << Pa_GetErrorText(err) << std::endl;
			Pa_Terminate();
			delete audioData;
			return;
		}

		err = Pa_StartStream(stream);
		if (err != paNoError) {
			std::cerr << "Error starting stream: " << Pa_GetErrorText(err)
					  << std::endl;
			Pa_CloseStream(stream);
			Pa_Terminate();
			delete audioData;
			return;
		}

		// Launch a detached thread to monitor playback completion
		std::thread([stream, audioData]() {
			// Wait until playback is finished
			while (Pa_IsStreamActive(stream) == 1) {
				Pa_Sleep(100);
			}
			// Clean up PortAudio
			Pa_StopStream(stream);
			Pa_CloseStream(stream);
			Pa_Terminate();

			// Free the audio data allocated in PlayWav
			delete audioData;
		}).detach();
	}

	// Parses a WAV file from the given path, then plays it.
	AudioData AudioManager::ParseWav(std::filesystem::path wavPath) {
		AudioFile<float> audioFile;
		if (!audioFile.load(wavPath.string())) {
			std::cerr << "Failed to load wav file: " << wavPath << std::endl;
			std::exit(-1);
		}

		AudioData audioData;
		audioData.sampleRate = audioFile.getSampleRate();
		audioData.numChannels = audioFile.getNumChannels();
		audioData.total = audioFile.getNumSamplesPerChannel();
		audioData.cur = 0;	// Start at beginning

		// Resize to hold all interleaved samples:
		audioData.samples.resize(audioData.total * audioData.numChannels);

		// AudioFile stores samples per channel (samples[channel][sampleIndex])
		// Interleave the channels into a single vector.
		for (int ch = 0; ch < audioData.numChannels; ++ch) {
			for (size_t i = 0; i < audioData.total; ++i) {
				audioData.samples[i * audioData.numChannels + ch] =
					audioFile.samples[ch][i];
			}
		}
		return audioData;
	}

}  // namespace FuncDoodle
