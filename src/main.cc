#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <portaudio.h>
#include <stdio.h>

#include <chrono>
#include <cmath>

#include "App.h"
#include "AssetLoader.h"

#include "LoadedImages.h"

#include <stb_image.h>

float SAMPLE_RATE = 44100.0;

std::vector<double> notes = {
	261.63,	 // C4
	293.66,	 // D4
	329.63,	 // E4
	349.23,	 // F4
	392.00,	 // G4
	440.00,	 // A4
	493.88,	 // B4
	523.25,	 // C5
};

enum Note { C4, D4, E4, F4, G4, A4, B4, C5 };

std::vector<std::pair<Note, double>> melody = {};

void GLFWErrorCallback(int error, const char* desc) {
	std::cerr << "GLFW ERROR (" << error << "): " << desc << std::endl;
}

void GlobalAppTick(GLFWwindow* win, auto lastFrameTime,
				   FuncDoodle::Application* application, ImGuiIO& io) {
	auto currentFrameTime = std::chrono::high_resolution_clock::now();
	auto deltaTime =
		std::chrono::duration<double>(currentFrameTime - lastFrameTime).count();
	constexpr double FRAME_TIME = 1.0 / 1000.0;

	if (deltaTime >= FRAME_TIME) {
		lastFrameTime = currentFrameTime;
		glfwPollEvents();

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport(),
									 ImGuiDockNodeFlags_PassthruCentralNode);

		application->RenderImGui();

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(win, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}

		glfwSwapBuffers(win);
	}
}

GLFWimage* GlobalLoadWinImage(const std::filesystem::path& assetsPath) {
	std::filesystem::path icon = assetsPath / "icon.png";
	//std::cout << icon.string().c_str() << std::endl;
	int width, height, chan;
	unsigned char* data =
		stbi_load(icon.string().c_str(), &width, &height, &chan, 0);
	if (data) {
		GLFWimage* icon = (GLFWimage*)malloc(sizeof(GLFWimage*));
		icon->width = width;
		icon->height = height;
		icon->pixels = data;
		return icon;
	} else {
		std::cerr << "Failed to read image data from assets/icon.png"
				  << std::endl;
		return nullptr;
	}
}

static int AudioCB(const void* inputBuffer, void* outputBuffer,
				   unsigned long framesPerBuffer,
				   const PaStreamCallbackTimeInfo* timeInfo,
				   PaStreamCallbackFlags statusFlags, void* userData) {
	float* out = static_cast<float*>(outputBuffer);
	static double phase = 0.0;
	static size_t noteIndex = 0;
	static double phaseIncrement = 0.0;
	static double noteDuration = 0.0;
	static double noteTimer = 0.0;

	if (noteIndex >= melody.size()) {
		// End of melody
		return paComplete;
	}

	double frequency = notes[melody[noteIndex].first];
	phaseIncrement = 2.0 * 3.141592653589793238462643383279502884719 *
					 frequency / SAMPLE_RATE;
	noteDuration = melody[noteIndex].second * SAMPLE_RATE;
	noteTimer += framesPerBuffer;

	for (unsigned long i = 0; i < framesPerBuffer; ++i) {
		if (noteTimer >= noteDuration) {
			noteTimer = 0.0;
			noteIndex++;
			if (noteIndex < melody.size()) {
				frequency = notes[melody[noteIndex].first];
				phaseIncrement = 2.0 *
								 3.141592653589793238462643383279502884719 *
								 frequency / SAMPLE_RATE;
				noteDuration = melody[noteIndex].second * SAMPLE_RATE;
			}
		}
		out[i] = static_cast<float>(std::sin(phase));
		phase += phaseIncrement;
		if (phase >= 2.0 * 3.141592653589793238462643383279502884719) {
			phase -= 2.0 * 3.141592653589793238462643383279502884719;
		}
	}

	return paContinue;
}

int main(int argc, char** argv) {
	const char* path = argv[0];
	const char* lastSlash = strrchr(path, '/');
	if (!lastSlash) {
		lastSlash = strrchr(path, '\\');
	}
	char dirPath[1024];
	if (lastSlash) {
		size_t len = lastSlash - path;
		strncpy(dirPath, path, len);
		dirPath[len] = '\0';
	} else {
		strncpy(dirPath, path, sizeof(dirPath) - 1);
		dirPath[sizeof(dirPath) - 1] = '\0';
	}

	std::filesystem::path assetsPath(dirPath);
	assetsPath /= "assets";

	// glfwSetErrorCallback(GLFWErrorCallback);
	

	if (!glfwInit()) {
		const char* description;
		int error = glfwGetError(&description);
		fprintf(stderr, "Failed to initialize GLFW: %d -- %s\n", error,
				description);
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // try to use older OpenGL version, because some laptops dont support 4.1, i don't want users to have to patch something out
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow* win = glfwCreateWindow(900, 900, "FuncDoodle", NULL, NULL);
	if (!win) {
		const char* desc;
		int error = glfwGetError(&desc);
		fprintf(stderr, "Failed to initialize GLFW window -- %s\n", desc);
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(win);

	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#ifndef TILING
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.Fonts->AddFontFromFileTTF(
		(assetsPath / "Roboto" / "Roboto-Medium.ttf").string().c_str(), 16.0f);
	io.Fonts->Build();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// In your style setup
	FuncDoodle::Application::CustomStyle();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init("#version 140");

	PaStream* stream;

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		std::cerr << "Failed to initialize port audio: " << Pa_GetErrorText(err)
				  << std::endl;
		free(stream);
		exit(-1);
	}
	err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, AudioCB,
							   nullptr);
	if (err != paNoError) {
		std::cerr << "Failed to open default stream: " << Pa_GetErrorText(err)
				  << std::endl;
		free(stream);
		exit(-1);
	}
	err = Pa_StartStream(stream);
	if (err != paNoError) {
		std::cerr << "Failed to start stream: " << Pa_GetErrorText(err)
				  << std::endl;
		free(stream);
		exit(-1);
	}

	FuncDoodle::AssetLoader assetLoader(assetsPath);

	FuncDoodle::GlobalLoadImages(&assetLoader);

	FuncDoodle::Application* application =
		new FuncDoodle::Application(win, &assetLoader);

	auto lastFrameTime = std::chrono::high_resolution_clock::now();

	GLFWimage* icon = GlobalLoadWinImage(assetsPath);

	glfwSetWindowIcon(win, 1, icon);

	stbi_image_free(icon->pixels);

	while (!glfwWindowShouldClose(win)) {
		GlobalAppTick(win, lastFrameTime, application, io);
	}
	delete application;

	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(win);
	glfwTerminate();

	return 0;
}
