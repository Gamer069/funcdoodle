#pragma once

#include "Project.h"

#include "Frame.h"

#include "Macro.h"

#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#include <GLFW/glfw3.h>

#include "MacroUtils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WRITEB(b) do { outFile.write(reinterpret_cast<const char*>(&(b)), sizeof((b))); } while (0)

namespace FuncDoodle {
    ProjectFile::ProjectFile(char name[], int width, int height, char author[], int fps, char desc[], GLFWwindow* win) : m_Window(win) {
        strcpy(m_Name, name);
        m_Width = width;
        m_Height = height;
        strcpy(m_Author, author);
        m_FPS = fps;
        strcpy(m_Desc, desc);
        m_Frames = LongIndexArray<Frame>();
        m_Frames.push_back(Frame(m_Width, m_Height));
        m_Frames.push_back(Frame(m_Width, m_Height));
    }
    ProjectFile::~ProjectFile() {}

    const char* ProjectFile::AnimName() const {
        return m_Name;
    }
    void ProjectFile::SetAnimName(char name[]) {
        strcpy(m_Name, name);
    }

    void ProjectFile::Export(char* filePath) {
        std::cout << "Exporting to " << filePath << std::endl;

        LongIndexArray<Frame>* frames = AnimFrames();

        char curFilePath[512];

        for (long i = 0; i < AnimFrameCount(); i++) {
            sprintf(curFilePath, "%s/frame_%d.png", filePath, i);
            frames->get(i).Export(curFilePath);
        }
    }

    const int ProjectFile::AnimWidth() const {
        return m_Width;
    }
    void ProjectFile::SetAnimWidth(int width) {
        // no
        for (long i = 0; i < AnimFrameCount(); ++i) {
            m_Frames.get(i).SetWidth(width);
        }
        m_Width = width;
    }

    const int ProjectFile::AnimHeight() const {
        return m_Height;
    }
    void ProjectFile::SetAnimHeight(int height) {
        // no
        for (long i = 0; i < AnimFrameCount(); ++i) {
            m_Frames.get(i).SetHeight(height);
        }
        m_Height = height;
    }

    const char* ProjectFile::AnimAuthor() const {
        return m_Author;
    }
    void ProjectFile::SetAnimAuthor(char* author) {
        strcpy(m_Author, author);
    }

    const int ProjectFile::AnimFPS() const {
        if (this == nullptr) {
            std::cerr << "ProjectFile is nullptr" << std::endl;
            std::exit(-1);
        }
        return m_FPS;
    }
    void ProjectFile::SetAnimFPS(int FPS) {
        m_FPS = FPS;
    }

    const char* ProjectFile::AnimDesc() const {
        return m_Desc;
    }
    void ProjectFile::SetAnimDesc(char* desc) {
        strcpy(m_Desc, desc);
    }

    const long ProjectFile::AnimFrameCount() const {
        return m_Frames.getSize();
    }
    LongIndexArray<Frame>* ProjectFile::AnimFrames() {
        return &m_Frames;
    }


    void ProjectFile::Write(char* fileName) {
        std::ofstream outFile(fileName, std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file for writing" << std::endl;
            return;
        }

        outFile << "FDProj";
        int major = 0;
        int minor = 1;
        long frames = m_Frames.getSize();
        WRITEB(major); // version major
        WRITEB(minor); // version minor
        WRITEB(frames); // frame count (default)
        WRITEB(m_Width); // animation width
        WRITEB(m_Height); // animation height
        WRITEB(m_FPS); // animation fps
        unsigned char null = 0;
        outFile << m_Name; // animation name
        WRITEB(null);
        outFile << m_Desc; // animation description
        WRITEB(null);
        outFile << m_Author; // animation author
        WRITEB(null);

        LongIndexArray<Frame>* frameData = AnimFrames();

        // Use a vector and maintain stable indices
        std::vector<Col> uniqueColors;
        std::map<Col, int> colorToIndex;  // Map each color to its stable index

        // First pass: collect unique colors with stable ordering
        for (long i = 0; i < AnimFrameCount(); i++) {
            auto pixels = frameData->get(i).Pixels();
            for (int x = 0; x < pixels->getWidth(); x++) {
                for (int y = 0; y < pixels->getHeight(); y++) {
                    Col px = pixels->get(x,y);
                    if (colorToIndex.find(px) == colorToIndex.end()) {
                        colorToIndex[px] = uniqueColors.size();
                        uniqueColors.push_back(px);
                    }
                }
            }
        }

        // Write palette size
        std::size_t plteLen = uniqueColors.size();
        WRITEB(plteLen);

        // Write palette colors
        for (const Col& col : uniqueColors) {
            WRITEB(col.r);
            WRITEB(col.g);
            WRITEB(col.b);
        }

        // Write frame data using stable indices
        for (long i = 0; i < AnimFrameCount(); i++) {
            auto pixels = frameData->get(i).Pixels();
            for (int y = 0; y < pixels->getHeight(); y++) {
                for (int x = 0; x < pixels->getWidth(); x++) {
                    Col px = pixels->get(x,y);
                    int index = colorToIndex[px];  // Get stable index
                    WRITEB(index);
                }
            }
            unsigned char null = 0;
            WRITEB(null);
        }

        outFile << "EOP";
        outFile.close();
    }
    void ProjectFile::ReadAndPopulate(char* filePath) {
        if (this == nullptr) {
            std::exit(-1);
        }

        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Failed to open file" << std::endl;
            std::exit(-1);
        }

        const int numBytes = 6;
        std::string str(numBytes, '\0');

        file.read(&str[0], numBytes);


        if (str != "FDProj") {
            std::cerr << "This isn't a funcdooodle project" << std::endl;
            std::exit(-1);
        }

        int verMajor = 0;
        file.read(reinterpret_cast<char*>(&verMajor), sizeof(verMajor));
        int verMinor = 0;
        file.read(reinterpret_cast<char*>(&verMinor), sizeof(verMinor));
        long frameCount = 0;
        file.read(reinterpret_cast<char*>(&frameCount), sizeof(frameCount));
        int animWidth = 0;
        file.read(reinterpret_cast<char*>(&animWidth), sizeof(animWidth));
        int animHeight = 0;
        file.read(reinterpret_cast<char*>(&animHeight), sizeof(animHeight));
        int animFPS = 0;
        file.read(reinterpret_cast<char*>(&animFPS), sizeof(animFPS));

        std::cout << "SHOULD BE" << animWidth*animHeight << "px" << std::endl;

        file.getline(m_Name, sizeof(m_Name), '\0');

        if (file.fail()) {
            std::cerr << file.tellg();
            std::cerr << "IM DISSAPOINTED THAT THE DAMN FILE HAS JUST FAILED" << std::endl;
        }

        file.getline(m_Desc, sizeof(m_Desc), '\0');

        file.getline(m_Author, sizeof(m_Author), '\0');

        std::cout << m_Name << " -- NAME" << std::endl;
        std::cout << m_Desc << " -- DESC" << std::endl;
        std::cout << m_Author << " -- AUTH" << std::endl;

        std::vector<Col> plte;

        std::size_t plteLen = 0;

        if (file.fail()) {
            std::cerr << file.tellg();
            std::cerr << "IM DISSAPOINTED THAT THE DAMN FILE HAS JUST FAILED" << std::endl;
        }

        file.read(reinterpret_cast<char*>(&plteLen), sizeof(plteLen));

        if (file.fail()) {
            std::cerr << file.tellg();
            std::cerr << "IM DISSAPOINTED THAT THE DAMN FILE HAS JUST FAILED" << std::endl;
        }

        std::cout << "PLTE LEN: " << plteLen << std::endl;

        for (std::size_t i = 0; i < plteLen; i++) {
            // long value = 0;
            // file.read(reinterpret_cast<char*>(&value), sizeof(long));
            // std::cout << value << std::endl;
            // ASSERT_EQ(value, i);

            // read the rgb
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
            file.read(reinterpret_cast<char*>(&r), sizeof(r));
            file.read(reinterpret_cast<char*>(&g), sizeof(g));
            file.read(reinterpret_cast<char*>(&b), sizeof(b));

            plte.push_back(Col{.r = r, .g = g, .b = b});
        }

        for (int i = 0; i < plte.size(); i++) {
            std::cout << i << std::endl;
            std::cout << (unsigned int)(plte[i].r) << ";" << (unsigned int)(plte[i].g) << ";" << (unsigned int)(plte[i].b) << ";" << std::endl;
        }

        m_Frames = LongIndexArray<Frame>();

        long brokenIndexC = 0;
        long lastBrokenFrame = 0;

        for (long i = 0; i < frameCount; i++) {
            ImageArray* img = new ImageArray(animWidth, animHeight);

            // read colorarr: OOPS
            for (int y = 0; y < animHeight; y++) {
                for (int x = 0; x < animWidth; x++) {
                    // Read raw bytes first
                    unsigned char bytes[sizeof(int)];
                    file.read(reinterpret_cast<char*>(bytes), sizeof(int));
                    
                    // Convert to index
                    int index = *reinterpret_cast<int*>(bytes);

                    if (index > plteLen) {
                        std::cerr << "ERROR: INDEX OUT OF BOUNDS" << std::endl;
                        std::exit(-1);
                    }
                    
                    for (size_t b = 0; b < sizeof(int); b++) {
                        std::cout << std::hex << (int)bytes[b] << " ";
                    }

                    img->set(x, y, plte[index]);
                }
            }
            m_Frames.push_back(Frame(img));

            unsigned char null;
            file.read(reinterpret_cast<char*>(&null), 1);
        }
        m_Width = animWidth;
        m_Height = animHeight;
        m_FPS = animFPS;
        // maybe decide if the color space is needed cos everything is sRGB nowadays.
        
        char eop[4];
        file.getline(eop, sizeof(eop), '\0');

        if (strcmp(eop, "EOP") == 0) {
            std::cout << "SUCCESS!" << std::endl;
        }

        if (!file) {
            std::cerr << "Failed to read from file." << std::endl;
            std::exit(-1);
        }

        file.close();
    }

    void ProjectFile::DisplayFPS() {
        static double lastTime = glfwGetTime();
        static int frameCount = 0;
        
        double currentTime = glfwGetTime();
        frameCount++;

        // Update FPS counter every second
        if (currentTime - lastTime >= 1.0) {
            std::stringstream title;
            title << "FuncDoodle -- " << FUNCVER << " -- " << AnimName() << " (" << frameCount << " FPS)";
            glfwSetWindowTitle(m_Window, title.str().c_str());
            
            frameCount = 0;
            lastTime = currentTime;
        }
    }
}
