#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>

#include "Gui.h"

#include "stb_image_write.h"

namespace FuncDoodle {
    struct Col {
        unsigned char r = 255, g = 255, b = 255;

        bool operator==(const Col& other) const {
            return r == other.r && g == other.g && b == other.b;
        }
        bool operator<(const Col& other) const {
            if (r != other.r) return r < other.r;
            if (g != other.g) return g < other.g;
            return b < other.b;
        }
    };
    class ImageArray {
        public:
            ImageArray(int width, int height)
                : width(width), height(height), data(width * height) {
                    if (width < 1 || height < 1) {
                        throw std::invalid_argument("Width and height must be greater than 0");
                    }

                    for (int x = 0; x < width; x++) {
                        for (int y = 0; y < height; y++) {
                            set(x,y,Col());
                        }
                    }
                }

            void RedoColorAdjustment() {
                Resize();

                for (int x = 0; x < width; x++) {
                    for (int y = 0; y < height; y++) {
                        set(x,y,Col());
                    }
                }
            }
            void Resize() {
                data.resize(width * height);
            }

            // Set the color at the specified (x, y) position
            void set(int x, int y, const Col& color) {
                if (x < 0 || x >= width || y < 0 || y >= height) {
                    // no
                    throw std::out_of_range("Index out of range");
                }
                data[y * width + x] = color;
            }

            // Get the color at the specified (x, y) position
            Col get(int x, int y) const {
                if (x < 0 || x >= width || y < 0 || y >= height) {
                    // no
                    throw std::out_of_range("Index out of range");
                }
                return data[y * width + x];
            }

            int getWidth() const {
                return width;
            }

            void setWidth(int width) {
                this->width = width;
            }

            int getHeight() const {
                return height;
            }

            void setHeight(int height) {
                this->height = height;
            }

            void SetData(const std::vector<Col>& data) {
                this->data = data;
            }

            const std::vector<Col>& Data() const {
                return data;
            }

        private:
            int width;
            int height;
            std::vector<Col> data;
    };
    class Frame {
        public:
            Frame() : m_Pixels(nullptr) { };
            Frame(int width, int height) : m_Pixels(new ImageArray(width, height)) {};
            Frame(ImageArray* arr) : m_Pixels(arr) {};
            ~Frame() {};
            inline const ImageArray* Pixels() const {
                return m_Pixels;
            }
            void SetPixel(int x, int y, Col px) {
                m_Pixels->set(x, y, px);
            }
            inline const int Width() {
                return m_Pixels->getWidth();
            }
            void SetWidth(int width, bool clear = false) {
                if (m_Pixels == nullptr) {
                    m_Pixels = new ImageArray(width, 1);
                    std::cout << "NULLPTR SETWIDTH CALL!!!" << std::endl;
                } else {
                    if (!clear) {
                        const std::vector<Col>& oldData = m_Pixels->Data();
                        int oldWidth = m_Pixels->getWidth();
                        int oldHeight = m_Pixels->getHeight();
                        m_Pixels->setWidth(width);
                        m_Pixels->Resize();
                        std::vector<Col> newData(width * oldHeight);
                        int xOffset = (width - oldWidth) / 2;
                        for (int y = 0; y < oldHeight; y++) {
                            for (int x = 0; x < oldWidth; x++) {
                                if (newData.size() >= oldData.size()) {
                                    newData[y * width + (x + xOffset)] = oldData[y * oldWidth + x];
                                } else {
                                    newData[y * oldWidth + x] = oldData[y * oldWidth + x];
                                }
                            }
                        }
                        m_Pixels->SetData(newData);
                    } else {
                        m_Pixels->setWidth(width);
                        m_Pixels->Resize();
                        m_Pixels->RedoColorAdjustment();
                    }
                }
            }
            inline const int Height() {
                return m_Pixels->getHeight();
            }
            void SetHeight(int height, bool clear = false) {
                if (m_Pixels == nullptr) {
                    m_Pixels = new ImageArray(1, height);
                } else {
                    if (!clear) {
                        std::vector<Col> oldData = m_Pixels->Data();
                        int oldWidth = m_Pixels->getWidth();
                        int oldHeight = m_Pixels->getHeight();
                        m_Pixels->setHeight(height);
                        m_Pixels->Resize();
                        std::vector<Col> newData(oldWidth * height);
                        int yOffset = (height - oldHeight) / 2;
                        for (int y = 0; y < oldHeight; y++) {
                            for (int x = 0; x < oldWidth; x++) {
                                if (newData.size() >= oldData.size()) {
                                    newData[(y + yOffset) * oldWidth + x] = oldData[y * oldWidth + x];
                                } else {
                                    newData[y * oldWidth + x] = oldData[y * oldWidth + x];
                                }
                            }
                        }
                        m_Pixels->SetData(newData);
                    } else {
                        m_Pixels->setHeight(height);
                        m_Pixels->Resize();
                        m_Pixels->RedoColorAdjustment();
                    }
                }
            }

            void CopyToClipboard() {
                size_t bufferSize = 32; // Start with space for dimensions
                for (int y = 0; y < Height(); y++) {
                    for (int x = 0; x < Width(); x++) {
                        bufferSize += 32; // Space for r g b values and spaces/newline
                    }
                }
                
                char* fdata = (char*)malloc(bufferSize);
                if (!fdata) return;
                
                int written = snprintf(fdata, bufferSize, "%dx%d\n", Width(), Height());
                char* curr = fdata + written;
                
                // horizontal
                for (int y = 0; y < Height(); y++) {
                    for (int x = 0; x < Width(); x++) {
                        Col pixel = Pixels()->get(x, y);
                        written = snprintf(curr, bufferSize - (curr - fdata), 
                            "%d %d %d\n", pixel.r, pixel.g, pixel.b);
                        curr += written;
                    }
                }
                
                ImGui::SetClipboardText(fdata);
                free(fdata);
            }
            static Frame* PastedFrame() {
                char* pasted = const_cast<char*>(ImGui::GetClipboardText());
                if (!pasted) return nullptr;

                // Buffer for dimensions line (e.g. "1920x1080\0")
                char first[32];
                int i = 0;
                
                // Copy first line into buffer
                while (pasted[i] && pasted[i] != '\n' && pasted[i] != '\r' && i < 31) {
                    first[i] = pasted[i];
                    i++;
                }
                first[i] = '\0';

                // Parse dimensions
                int width = 0;
                int height = 0;
                char* ptr = first;
                width = atoi(ptr);
                while (*ptr && *ptr != 'x') ptr++;
                if (*ptr == 'x') {
                    ptr++;
                    height = atoi(ptr);
                }

                if (width <= 0 || height <= 0) return nullptr;

                Frame* frame = new Frame(width, height);

                ptr = pasted;
                // Skip first line
                while (*ptr && *ptr != '\n') ptr++;
                if (*ptr) ptr++;  // Skip the \n

                // Parse pixel data
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        Col pixel;
                        pixel.r = atoi(ptr);
                        while (*ptr && *ptr != ' ') ptr++;  // Skip to next number
                        if (*ptr) ptr++;

                        pixel.g = atoi(ptr);
                        while (*ptr && *ptr != ' ') ptr++;
                        if (*ptr) ptr++;

                        pixel.b = atoi(ptr);
                        while (*ptr && *ptr != '\n') ptr++;
                        if (*ptr) ptr++;

                        frame->SetPixel(x, y, pixel);
                    }
                }

                return frame;
            }
            void Export(char* filePath) {
                stbi_write_png(filePath, Width(), Height(), 3, Data().data(), Width() * 3);
            }
            std::vector<Col> Data() const {
                return m_Pixels->Data();
            }
    private:
        ImageArray* m_Pixels;
    };
}
