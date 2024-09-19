#pragma once

#include "Project.h"

#include "Frame.h"

#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <set>

#define WRITEB(b) do { outFile.write(reinterpret_cast<const char*>(&(b)), sizeof((b))); } while (0)

namespace FuncDoodle {
    ProjectFile::ProjectFile(char name[], int width, int height, char author[], int fps, char desc[]) {
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

    const int ProjectFile::AnimWidth() const {
        return m_Width;
    }
    void ProjectFile::SetAnimWidth(int width) {
        for (long i = 0; i < AnimFrameCount(); ++i) {
            m_Frames.get(i).SetWidth(width);
            std::cout << "Wat" << std::endl;
        }
        m_Width = width;
    }

    const int ProjectFile::AnimHeight() const {
        return m_Height;
    }
    void ProjectFile::SetAnimHeight(int height) {
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
        std::cout << "ProjectFile::AnimFrameCount()" << std::endl;
        std::cout << "END" << std::endl;
        return m_Frames.getSize();
    }
    LongIndexArray<Frame>* ProjectFile::AnimFrames() {
        return &m_Frames;
    }

    
    void ProjectFile::Write(char* fileName) {
        std::ofstream outFile(fileName, std::ios::binary);
        if (outFile.is_open()) {
            std::cout << "Writing file header" << std::endl;
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
            outFile << "sRGB."; // color space
            outFile << m_Name; // animation name
            WRITEB(null);
            outFile << m_Desc; // animation description
            WRITEB(null);
            outFile << m_Author; // animation author
            WRITEB(null);

            LongIndexArray<Frame>* frameData = AnimFrames();

            std::vector<Col> colors = std::vector<Col>();
            
            for (long i = 0; i < AnimFrameCount(); i++) {
                auto pixels = frameData->get(i).Pixels();
                for (int x = 0; x < pixels->getWidth(); x++) {
                    for (int y = 0; y < pixels->getHeight(); y++) {
                        colors.push_back(pixels->get(x,y));
                    }
                }
            }

            std::set<Col> uniqueSet(colors.begin(), colors.end()); // Set automatically handles uniqueness
            std::vector<Col> uniqueVec(uniqueSet.begin(), uniqueSet.end());

            for (int i = 0; i < uniqueVec.size(); i++) {
                std::cout << "Color" << std::endl;
                int r = uniqueVec[i].r;
                int g = uniqueVec[i].g;
                int b = uniqueVec[i].b;
                WRITEB(r);
                WRITEB(g);
                WRITEB(b);
            }

            outFile << "PALEND";

            std::cout << "E" << std::endl;
            std::cout << outFile.tellp() << std::endl;

            for (long i = 0; i < AnimFrameCount(); i++) {
                WRITEB(i);
                for (int y = 0; y < AnimFrames()->get(i).Pixels()->getWidth(); y++) {
                    for (int x = 0; x < AnimFrames()->get(i).Pixels()->getHeight(); x++) {
                        Col px = AnimFrames()->get(i).Pixels()->get(x,y);
                        auto it = uniqueSet.find(px);
            
                        if (it != uniqueSet.end()) {
                            int index = std::distance(uniqueSet.begin(), it);
                            WRITEB(index);
                        } else {
                            std::cerr << "Failed to find color in the palette: Terminating..." << std::endl;
                            exit(-1);
                        }
            
                       outFile << "COLOREND";
                    }
                }
                WRITEB(null);
            }

            outFile << "EOP";

            outFile.close(); // close the out file
        } else {
            std::cerr << "Failed to write project file" << std::endl;
        }
    }
    void ProjectFile::ReadAndPopulate(char* filePath) {
        if (this == nullptr) {
            std::cout << "Nullptr..?" << std::endl;
            std::exit(-1);
        }

        std::cout << "READ?" << std::endl;

        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Failed to open file" << std::endl;
            std::exit(-1);
        }

        const int numBytes = 6;
        std::string str(numBytes, '\0');

        file.read(&str[0], numBytes);


        if (str == "FDProj") {
            std::cout << "Yep" << std::endl;
        } else {
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

        char* colorSpace = "";
        char cur;
        
        while (file.get(cur)) {  // Read one character at a time
            colorSpace += cur;
            if (cur == '.') {  // Stop if we encounter the null terminator
                break;
            }
        }

        char* animName;

        while (file.get(cur)) {  // Read one character at a time
            animName += cur;
            if (cur == '\0') {  // Stop if we encounter the null terminator
                break;
            }
        }

        char* animDesc;

        while (file.get(cur)) {  // Read one character at a time
            animDesc += cur;
            if (cur == '\0') {  // Stop if we encounter the null terminator
                break;
            }
        }

        char* animAuthor;

        while (file.get(cur)) {  // Read one character at a time
            animAuthor += cur;
            if (cur == '\0') {  // Stop if we encounter the null terminator
                break;
            }
        }

        std::cout << "verMajor: " << verMajor << std::endl;
        std::cout << "verMinor: " << verMinor << std::endl;
        std::cout << "frameCount: " << frameCount << std::endl;
        std::cout << "animWidth: " << animWidth << std::endl;
        std::cout << "animHeight: " << animHeight << std::endl;
        std::cout << "animFPS: " << animFPS << std::endl;

        for (int i = 0; i < strlen(m_Name); i++) {
            m_Name[i] = animName[i];
        }

        for (int i = 0; i < strlen(m_Desc); i++) {
            m_Desc[i] = animDesc[i];
        }

        for (int i = 0; i < strlen(m_Author); i++) {
            m_Author[i] = animAuthor[i];
        }

        m_Frames = LongIndexArray<Frame>();

        std::cout << "Size: " << m_Frames.getSize() << std::endl;

        // TODO: change frames
        m_Width = animWidth;
        m_Height = animHeight;
        m_FPS = animFPS;
        // TODO: color space

        if (!file) {
            std::cerr << "Failed to read from file." << std::endl;
            std::exit(-1);
        }

        file.close();
    }
}
