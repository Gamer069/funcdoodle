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

            std::size_t plteLen = uniqueVec.size();
            WRITEB(plteLen);

            for (int i = 0; i < uniqueVec.size(); i++) {
                int r = uniqueVec[i].r;
                int g = uniqueVec[i].g;
                int b = uniqueVec[i].b;
                WRITEB(r);
                WRITEB(g);
                WRITEB(b);

                std::cout << r << "," << g << "," << b << std::endl;
            }

            for (long i = 0; i < AnimFrameCount(); i++) {
                std::cout << "I SAVING: " << i << std::endl;
                //WRITEB(i);
                for (int y = 0; y < AnimFrames()->get(i).Pixels()->getWidth(); y++) {
                    for (int x = 0; x < AnimFrames()->get(i).Pixels()->getHeight(); x++) {
                        Col px = AnimFrames()->get(i).Pixels()->get(x,y);
                        auto it = uniqueSet.find(px);

                        if (it != uniqueSet.end()) {
                            int index = std::distance(uniqueSet.begin(), it);
                            std::cout << "SAV INDEX: " << index << std::endl;
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

        // TODO: read palette

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

        // TODO: if this code is wrong fix it
        std::cout << "FRAME COUNT:";
        std::cout << frameCount << std::endl;
        for (long i = 0; i < frameCount; i++) {
            ImageArray* img = new ImageArray(animWidth, animHeight);

            // read colorarr: OOPS
            for (int x = 0; x < animWidth; x++) {
                for (int y = 0; y < animHeight; y++) {
                    int index = 0;
                    file.read(reinterpret_cast<char*>(&index), sizeof(index)); // read the index

                    if (index > plteLen) {
                        brokenIndexC++;
                        lastBrokenFrame = i;
                    }
                    std::cout << "Index: " << index << std::endl;

                    // plte.get(index)
                    img->set(x, y, Col());
                }
            }
            m_Frames.push_back(Frame(img));
        }
        std::cerr << "BROKEN INDEX COUNT: " << brokenIndexC << std::endl;
        std::cerr << "LAST BROKEN FRAME: " << lastBrokenFrame << std::endl;
        m_Width = animWidth;
        m_Height = animHeight;
        m_FPS = animFPS;
        // TODO: color space
        
        char eop[4];
        file.getline(eop, sizeof(eop), '\0');

        if (eop == "EOP") {
            std::cout << "SUCCESS!" << std::endl;
        }

        if (!file) {
            std::cerr << "Failed to read from file." << std::endl;
            std::exit(-1);
        }

        file.close();
    }
}
