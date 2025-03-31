#include "Themes.h"
#include "imgui_internal.h"
#include <cstdio>
#include <cstring>
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
#include "Audio.h"

#include "LoadedAssets.h"

#include "MacroUtils.h"

#include <stb_image.h>

#include "exepath.h"

float SAMPLE_RATE = 44100.0;

void GLFWErrorCallback(int error, const char* desc) {
	FUNC_WARN("GLFW ERROR (" << error << "): " << desc);
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
	FUNC_INF(assetsPath / "icon.png");
	std::filesystem::path icon = assetsPath / "icon.png";
	int width, height, chan;
	unsigned char* data =
		stbi_load(icon.string().c_str(), &width, &height, &chan, 0);
	if (data) {
		GLFWimage* icon = (GLFWimage*)malloc(sizeof(GLFWimage));
		icon->width = width;
		icon->height = height;
		icon->pixels = data;
		return icon;
	} else {
		FUNC_WARN("Failed to read image data from window icon");
		return nullptr;
	}
}

int main(int argc, char** argv) {
	const char* path = exepath::get();
	FUNC_DBG("Starting funcdoodle with exe path: " << path);
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

	//glfwSetErrorCallback(GLFWErrorCallback);

	if (!glfwInit()) {
		const char* description;
		int error = glfwGetError(&description);
		fprintf(stderr, "Failed to initialize GLFW: %d -- %s\n", error,
				description);
		return -1;
	}

	glfwWindowHint(GLFW_SCALE_TO_MONITOR, 1);
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
	float xScale, yScale;
	glfwGetWindowContentScale(win, &xScale, &yScale);
	float dpiScale = xScale;
	ImFontConfig fontConfig;
	fontConfig.OversampleH = 4;
	fontConfig.OversampleV = 4;
	fontConfig.PixelSnapH = true;

	io.Fonts->AddFontFromFileTTF((assetsPath / "Roboto" / "Roboto-Medium.ttf").string().c_str(), 16.0, &fontConfig);
	io.Fonts->Build();

    ImGui::GetStyle().ScaleAllSizes(1.0f / dpiScale);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	glfwSetWindowCloseCallback(win, [](GLFWwindow* win){});

	FuncDoodle::AssetLoader assetLoader(assetsPath);
	FuncDoodle::Application* application = new FuncDoodle::Application(win, &assetLoader);

	ImGui::GetStyle() = FuncDoodle::Themes::FuncDoodleStyle();

	FuncDoodle::Themes::InitThemes();

	ImGuiSettingsHandler handler;
	handler.TypeName = "UserData";
	handler.TypeHash = ImHashStr(handler.TypeName);
	handler.UserData = application;
	FUNC_WARN("stupid application: " << application);
	FUNC_WARN("stupid userdata: " << handler.UserData);
	handler.ReadOpenFn = [](ImGuiContext*, ImGuiSettingsHandler* handler, const char* val) -> void* {
		if (std::strcmp(val, "Preferences") == 0) {
			return handler->UserData;
		}
		return nullptr;
	};
	handler.ReadLineFn = [](ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line) {
		FUNC_WARN("ReadLineFn called with line: " << line);  // Debugging
		int sel;
		if (std::sscanf(line, "Theme=%i", &sel) == 1) {
			static_cast<FuncDoodle::Application*>(entry)->SetTheme(sel);
		}
		FUNC_WARN("sel: " << sel);
		switch (static_cast<FuncDoodle::Application*>(entry)->Theme()) {
			case 0: { 
				ImGui::GetStyle() = FuncDoodle::Themes::FuncDoodleStyle();
				break;
			};
			case 1: { 
				ImGui::StyleColorsDark();
				break;
			};
			case 2: { 
				ImGui::StyleColorsLight();
				break;
			};
			case 3: { 
				ImGui::StyleColorsClassic();
				break;
			};
			case 4: { 
				ImGui::GetStyle() = FuncDoodle::Themes::CatppuccinMochaStyle();
				break;
			};
			case 5: { 
				ImGui::GetStyle() = FuncDoodle::Themes::CatppuccinMacchiatoStyle();
				break;
			};
			case 6: { 
				ImGui::GetStyle() = FuncDoodle::Themes::CatppuccinFrappeStyle();
				break;
			}
			case 7: { 
				ImGui::GetStyle() = FuncDoodle::Themes::CatppuccinLatteStyle();
				break;
			};
		}
	};
	handler.WriteAllFn = [](ImGuiContext*, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf) {
		FuncDoodle::Application* application = static_cast<FuncDoodle::Application*>(handler->UserData);
		if (!application) {
			FUNC_INF("???");
			return;
		}
		int theme = application->Theme();
		buf->reserve(buf->size() + sizeof(int));
		buf->append("[UserData][Preferences]\n");
		buf->appendf("Theme=%d", theme);
		buf->append("\n");
	};
	ImGui::AddSettingsHandler(&handler);

    ImGui::LoadIniSettingsFromDisk(ImGui::GetIO().IniFilename);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init("#version 140");

	PaStream* stream;

	PaError err = Pa_Initialize();
	if (err != paNoError) {
		FUNC_WARN("Failed to initialize port audio: " + (std::string)Pa_GetErrorText(err));
		free(stream);
		exit(-1);
	}
	err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, paCB,
							   nullptr);
	if (err != paNoError) {
		FUNC_WARN("Failed to open default stream: " << Pa_GetErrorText(err));
		free(stream);
		exit(-1);
	}
	err = Pa_StartStream(stream);

	if (err != paNoError) {
		FUNC_WARN("Failed to start audio stream:" + (std::string)Pa_GetErrorText(err));
		free(stream);
		exit(-1);
	}

	FuncDoodle::GlobalLoadAssets(&assetLoader);

	auto lastFrameTime = std::chrono::high_resolution_clock::now();

	GLFWimage* icon = GlobalLoadWinImage(assetsPath);

	glfwSetWindowUserPointer(win, application);
	glfwSetWindowIcon(win, 1, icon);
	glfwSetDropCallback(win, [](GLFWwindow* win, int count, const char** paths){((FuncDoodle::Application*)(glfwGetWindowUserPointer(win)))->DropCallback(win, count, paths);});

	stbi_image_free(icon->pixels);

	while (!glfwWindowShouldClose(win)) {
		GlobalAppTick(win, lastFrameTime, application, io);
		if (application->ShouldClose()) {
			break;
		}
		if (glfwWindowShouldClose(win)) {
			if (application->CurProj() && !application->CurProj()->Saved()) {
				glfwSetWindowShouldClose(win, false);
				application->OpenSaveChangesDialog();
			}
		}
	}
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	// cleanup code
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(win);
	glfwTerminate();

	delete application;

	return 0;
}
