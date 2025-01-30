#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Gui.h"

#include "stb_image_write.h"

namespace FuncDoodle {
	struct Col {
		public:
			unsigned char r = 255, g = 255, b = 255;

			bool operator==(const Col& other) const {
				return r == other.r && g == other.g && b == other.b;
			}
			bool operator!=(const Col& other) const {
				return !(r == other.r && g == other.g && b == other.b);
			}
			bool operator<(const Col& other) const {
				if (r != other.r)
					return r < other.r;
				if (g != other.g)
					return g < other.g;
				return b < other.b;
			}
			friend std::ostream& operator<<(std::ostream& stream,
											const Col& col) {
				stream << "Col{" << (unsigned int)col.r << ", "
					   << (unsigned int)col.g << ", " << (unsigned int)col.b
					   << "}";
				return stream;
			}
	};
	class ImageArray {
		public:
			ImageArray(int width, int height, Col bgCol);
			~ImageArray();

			void RedoColorAdjustment(Col bgCol);
			void Resize();
			void Set(int x, int y, const Col& color);
			Col Get(int x, int y) const;

			inline int Width() const { return m_Width; }
			inline void SetWidth(int width, bool clear = false) {
				m_Width = width;
			}
			inline int Height() const { return m_Height; }
			inline void SetHeight(int height, bool clear = false) {
				m_Height = height;
			}
			inline void SetData(const std::vector<Col>& data) {
				this->m_Data = data;
			}
			inline const std::vector<Col>& Data() const { return m_Data; }
			inline const Col BgCol() const { return m_BG; }
			inline void SetBG(const Col bgCol) { m_BG = bgCol; }

		private:
			int m_Width = 32;
			int m_Height = 32;
			std::vector<Col> m_Data;
			Col m_BG;
	};
	class Frame {
		public:
			Frame(const Frame& other) {
				m_Pixels = new ImageArray(*other.Pixels());
			}
			Frame(int width, int height, Col bgCol) {
				m_Pixels = new ImageArray(width, height, bgCol);
			};
			Frame(ImageArray* arr) : m_Pixels(arr) {};
			~Frame() {};
			void ReInit(int width, int height, Col bgCol) {
				m_Pixels = new ImageArray(width, height, bgCol);
			}
			Frame& operator=(const Frame& other);
			void SetWidth(int width, bool clear = false);
			void SetHeight(int height, bool clear = false);

			void CopyToClipboard();
			static Frame* PastedFrame();
			void Export(char* filePath);

			inline const ImageArray* Pixels() const { return m_Pixels; }
			inline ImageArray* PixelsMut() { return m_Pixels; }
			inline void SetPixel(int x, int y, Col px) {
				m_Pixels->Set(x, y, px);
			}
			inline const int Width() { return m_Pixels->Width(); }
			inline const int Height() { return m_Pixels->Height(); }
			inline std::vector<Col> Data() const { return m_Pixels->Data(); }
		private:
			ImageArray* m_Pixels;
	};
}  // namespace FuncDoodle
