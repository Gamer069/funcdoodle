#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <portaudio.h>
#include <stdio.h>

#include <cmath>

#include "App.h"

float SAMPLE_RATE = 44100.0;

std::vector<double> notes = {
    261.63, // C4
    293.66, // D4
    329.63, // E4
    349.23, // F4
    392.00, // G4
    440.00, // A4
    493.88,  // B4
    523.25, // C5
};

enum Note {
    C4, D4, E4, F4, G4, A4, B4, C5
};

std::vector<std::pair<Note, double>> melody = {
};

static int AudioCB(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData) {
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
    phaseIncrement = 2.0 * 3.141592653589793238462643383279502884719 * frequency / SAMPLE_RATE;
    noteDuration = melody[noteIndex].second * SAMPLE_RATE;
    noteTimer += framesPerBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        if (noteTimer >= noteDuration) {
            noteTimer = 0.0;
            noteIndex++;
            if (noteIndex < melody.size()) {
                frequency = notes[melody[noteIndex].first];
                phaseIncrement = 2.0 * 3.141592653589793238462643383279502884719 * frequency / SAMPLE_RATE;
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
    if (!glfwInit()) {
        const char* description;
        int error = glfwGetError(&description);
        fprintf(stderr, "Failed to initialize GLFW: %d -- %s\n", error, description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif

    GLFWwindow* win = glfwCreateWindow(1280, 1080, "FuncDoodle", NULL, NULL);
    if (!win) {
        fprintf(stderr, "Failed to initialize GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    PaStream* stream;

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "Failed to initialize port audio: " << Pa_GetErrorText(err) << std::endl;
        exit(-1);
    }
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, AudioCB, nullptr);
    if (err != paNoError) {
        std::cerr << "Failed to open default stream: " << Pa_GetErrorText(err) << std::endl;
        exit(-1);
    }
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        exit(-1);
    }

    FuncDoodle::Application* application = new FuncDoodle::Application();

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0U, ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

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

