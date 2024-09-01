#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>

namespace FuncDoodle {
    typedef struct {
        unsigned char r = 255, g = 255, b = 255;
    } Col;
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

            // Set the color at the specified (x, y) position
            void set(int x, int y, const Col& color) {
                if (x < 0 || x >= width || y < 0 || y >= height) {
                    throw std::out_of_range("Index out of range");
                }
                data[y * width + x] = color;
            }

            // Get the color at the specified (x, y) position
            Col get(int x, int y) const {
                if (x < 0 || x >= width || y < 0 || y >= height) {
                    throw std::out_of_range("Index out of range");
                }
                return data[y * width + x];
            }

            int getWidth() const {
                return width;
            }

            int getHeight() const {
                return height;
            }

        private:
            int width;
            int height;
            std::vector<Col> data; // Internal storage for pixel data
    };
    class Frame {
        public:
            Frame() : m_Pixels(nullptr) {};
            Frame(int width, int height) : m_Pixels(new ImageArray(width, height)) {};
            ~Frame() {};
            const ImageArray* Pixels() const {
                return m_Pixels;
            }
            void SetPixel(int x, int y, Col px) {
                m_Pixels->set(x, y, px);
            }
        private:
            ImageArray* m_Pixels;
    };
}
