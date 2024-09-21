#pragma once

#include "Project.h"

#include "Frame.h"

#include "Macro.h"

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
        if (outFile.is_open()) {
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
                int r = uniqueVec[i].r;
                int g = uniqueVec[i].g;
                int b = uniqueVec[i].b;
                WRITEB(r);
                WRITEB(g);
                WRITEB(b);
            }

            for (long i = 0; i < AnimFrameCount(); i++) {
                // WRITEB(i);
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

        for (int i = 0; i < strlen(m_Name); i++) {
            m_Name[i] = animName[i];
        }

        for (int i = 0; i < strlen(m_Desc); i++) {
            m_Desc[i] = animDesc[i];
        }

        for (int i = 0; i < strlen(m_Author); i++) {
            m_Author[i] = animAuthor[i];
        }

        // TODO: read palette

        LongIndexArray<Col> plte;

        for (long i = 0; i < frameCount; i++) {
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

        for (int i = 0; i < plte.getSize(); i++) {
            std::cout << i << std::endl;
            std::cout << (unsigned int)(plte[i].r) << ";" << (unsigned int)(plte[i].g) << ";" << (unsigned int)(plte[i].b) << ";" << std::endl;
        }

        m_Frames = LongIndexArray<Frame>();

        // TODO: if this code is wrong fix it
        for (long i = 0; i < frameCount; i++) {
            ImageArray* img = new ImageArray(animWidth, animHeight);
            int xVal = 0;
            int yVal = 0;

            // read colorarr: OOPS
            for (long c = 0; c < (animWidth * animHeight); c++) {
                long index = 0;
                file.read(reinterpret_cast<char*>(&index), sizeof(index)); // read the index

                std::cout << xVal << std::endl;
                std::cout << yVal << std::endl;

                std::cout << "Index: " << (int)(index) << std::endl;

                // plte.get(index)
                img->set(xVal, yVal, Col());
                if (xVal < animWidth) {
                    xVal++;
                } else {
                    yVal++;
                    xVal = 0;
                }
            }
            m_Frames.push_back(Frame(img));
        }
        m_Width = animWidth;
        m_Height = animHeight;
        m_FPS = animFPS;
        // TODO: color space

        char eop[3];
        for (int i = 0; i < 3; i++) {
            file.get(cur);
            eop[i] = cur;
        }

        std::cout << "This is the only occurrence of the ASSERT_EQ macro in this entire code. This is terrible but i mean whatever" << std::endl;

        if (!file) {
            std::cerr << "Failed to read from file." << std::endl;
            std::exit(-1);
        }

        file.close();
    }
}
