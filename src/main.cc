#include "Themes.h"
#include "UUID.h"
#include "imgui_internal.h"
#include <cstdio>
#include <cstdlib>
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
	FUNC_ERR("GLFW ERROR (" << error << "): " << desc);
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

	std::filesystem::path themesPath(dirPath);
	themesPath /= "themes";

	// glfwSetErrorCallback(GLFWErrorCallback);

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

	io.Fonts->AddFontFromFileTTF(
		(assetsPath / "Roboto" / "Roboto-Medium.ttf").string().c_str(), 16.0,
		&fontConfig);
	io.Fonts->Build();

	ImGui::GetStyle().ScaleAllSizes(1.0f / dpiScale);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	glfwSetWindowCloseCallback(win, [](GLFWwindow* win) {});

	FuncDoodle::AssetLoader assetLoader(assetsPath);

	FuncDoodle::Themes::LoadThemes(themesPath);

	FuncDoodle::Application* application =
		new FuncDoodle::Application(win, &assetLoader, themesPath);

	ImGuiSettingsHandler handler;
	handler.TypeName = "UserData";
	handler.TypeHash = ImHashStr(handler.TypeName);
	handler.UserData = application;
	handler.ReadOpenFn = [](ImGuiContext*, ImGuiSettingsHandler* handler,
							const char* val) -> void* {
		if (std::strcmp(val, "Preferences") == 0) {
			return handler->UserData;
		}
		return nullptr;
	};
	handler.ReadLineFn = [](ImGuiContext*, ImGuiSettingsHandler*, void* entry,
							const char* line) {
		char* sel = (char*)malloc(37);
		if (std::sscanf(line, "Theme=\"%36s\"", sel) == 1) {
			static_cast<FuncDoodle::Application*>(entry)->SetTheme(FuncDoodle::UUID::FromString(sel));
		}
		int sfxEnabled;
		if (std::sscanf(line, "Sfx=%d", &sfxEnabled) == 1) {
			bool sfxEnabledBool = false;
			if (sfxEnabled >= 1) {
				sfxEnabledBool = true;
			}
			static_cast<FuncDoodle::Application*>(entry)->SetSFXEnabled(sfxEnabledBool);
		}
		int prevEnabled;
		if (std::sscanf(line, "Prev=%d", &prevEnabled) == 1) {
			bool prevEnabledBool = false;
			if (prevEnabled >= 1)
				prevEnabledBool = true;
			static_cast<FuncDoodle::Application*>(entry)->SetPrevEnabled(prevEnabledBool);
		}
		FuncDoodle::UUID uuid = static_cast<FuncDoodle::Application*>(entry)->Theme();
		if (!FuncDoodle::Themes::g_Themes.contains(uuid)) {
			ImGui::GetStyle() = FuncDoodle::Themes::g_Themes[FuncDoodle::UUID::FromString("d0c1a009-d09c-4fe6-84f8-eddcb2da38f9")].Style;
			ImGui::GetStyle().Alpha = 1.0f;  // Fully opaque
			ImGui::GetStyle().WindowRounding = 10.0f;
			ImGui::GetStyle().FrameRounding = 5.0f;
			ImGui::GetStyle().PopupRounding = 12.0f;
			ImGui::GetStyle().ScrollbarRounding = 10.0f;
			ImGui::GetStyle().GrabRounding = 6.0f;
			ImGui::GetStyle().TabRounding = 12.0f;
			ImGui::GetStyle().ChildRounding = 12.0f;
			ImGui::GetStyle().WindowPadding = ImVec2(10, 10);
			ImGui::GetStyle().FramePadding = ImVec2(8, 8);
			ImGui::GetStyle().ItemSpacing = ImVec2(10, 10);
			ImGui::GetStyle().IndentSpacing = 20.0f;
			ImGui::GetStyle().ScrollbarSize = 16.0f;
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				ImGui::GetStyle().WindowRounding = 1.0f;
			}
			return;
		}
		ImGui::GetStyle() = FuncDoodle::Themes::g_Themes[uuid].Style;
		ImGui::GetStyle().Alpha = 1.0f;  // Fully opaque
		ImGui::GetStyle().WindowRounding = 10.0f;
		ImGui::GetStyle().FrameRounding = 5.0f;
		ImGui::GetStyle().PopupRounding = 12.0f;
		ImGui::GetStyle().ScrollbarRounding = 10.0f;
		ImGui::GetStyle().GrabRounding = 6.0f;
		ImGui::GetStyle().TabRounding = 12.0f;
		ImGui::GetStyle().ChildRounding = 12.0f;
		ImGui::GetStyle().WindowPadding = ImVec2(10, 10);
		ImGui::GetStyle().FramePadding = ImVec2(8, 8);
		ImGui::GetStyle().ItemSpacing = ImVec2(10, 10);
		ImGui::GetStyle().IndentSpacing = 20.0f;
		ImGui::GetStyle().ScrollbarSize = 16.0f;
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::GetStyle().WindowRounding = 1.0f;
		}
	};
	handler.WriteAllFn = [](ImGuiContext*, ImGuiSettingsHandler* handler,
			ImGuiTextBuffer* buf) {
		FuncDoodle::Application* application =
			static_cast<FuncDoodle::Application*>(handler->UserData);
		if (!application) {
			FUNC_INF("???");
			return;
		}
		FuncDoodle::UUID theme = application->Theme();
		buf->reserve(buf->size() + strlen(theme.ToString()));
		buf->append("[UserData][Preferences]\n");
		buf->appendf("Theme=\"%s\"", theme.ToString());
		buf->append("\n");
		buf->appendf("Sfx=%d", application->SFXEnabled() ? 1 : 0);
		buf->append("\n");
		buf->appendf("Prev=%d", application->PrevEnabled() ? 1 : 0);
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
		FUNC_WARN("Failed to initialize port audio: " +
				(std::string)Pa_GetErrorText(err));
		free(stream);
		exit(-1);
	}
	err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, paCB, nullptr);
	if (err != paNoError) {
		FUNC_WARN("Failed to open default stream: " << Pa_GetErrorText(err));
		free(stream);
		exit(-1);
	}
	err = Pa_StartStream(stream);

	if (err != paNoError) {
		FUNC_WARN("Failed to start audio stream:" +
				(std::string)Pa_GetErrorText(err));
		free(stream);
		exit(-1);
	}

	FuncDoodle::GlobalLoadAssets(&assetLoader);

	auto lastFrameTime = std::chrono::high_resolution_clock::now();

	GLFWimage* icon = GlobalLoadWinImage(assetsPath);

	glfwSetWindowUserPointer(win, application);
	glfwSetWindowIcon(win, 1, icon);
	glfwSetDropCallback(win, [](GLFWwindow* win, int count, const char** paths) {
			((FuncDoodle::Application*)(glfwGetWindowUserPointer(win)))->DropCallback(win, count, paths);
			});

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
