#pragma once

#include "Frame.h"

#include "DynArr.h"

#include <iostream>

namespace FuncDoodle {
    class ProjectFile {
        public:
            ProjectFile(char name[256], int width, int height, char author[100], int fps, char desc[512]);
            ~ProjectFile();
            const char* AnimName() const;
            void SetAnimName(char name[256]);
            const int AnimWidth() const;
            void SetAnimWidth(int width);
            const int AnimHeight() const;
            void SetAnimHeight(int height);
            const char* AnimAuthor() const;
            void SetAnimAuthor(char* author);
            const int AnimFPS() const;
            void SetAnimFPS(int FPS);
            const char* AnimDesc() const;
            void SetAnimDesc(char* desc);
            const long AnimFrameCount() const;
            void SetAnimFrameCount(long count);
            LongIndexArray<Frame>* AnimFrames();
            void Write(char* filePath);
            void ReadAndPopulate(char* filePath);
        private:
            char m_Name[256]; // 256
            int m_Width = 0;
            int m_Height = 0;
            char m_Author[100]; // 100
            int m_FPS = 0;
            char m_Desc[512]; // 512
            LongIndexArray<Frame> m_Frames;
    };
}
