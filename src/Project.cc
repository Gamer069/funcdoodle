#pragma once

#include "Project.h"

#include "Frame.h"

#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>

#define WRITEB(b) do { outFile.write(reinterpret_cast<const char*>(&(b)), sizeof((b))); } while (0)

//int width;
//int height;
//char author[100] = "";
//int fps;
//char desc[512] = "";



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
        m_Width = width;
    }

    const int ProjectFile::AnimHeight() const {
        return m_Height;
    }
    void ProjectFile::SetAnimHeight(int height) {
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
            outFile.close(); // close the out file
        } else {
            std::cerr << "Failed to write project file" << std::endl;
        }
    }
}
