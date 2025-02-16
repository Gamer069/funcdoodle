#include "Frame.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Gui.h"

#include "MacroUtils.h"

#include "stb_image_write.h"

namespace FuncDoodle {
	ImageArray::ImageArray(int width, int height, Col bgCol)
		: m_Width(width), m_Height(height), m_Data(width * height) {
		if (m_Width < 1 || m_Height < 1) {
			throw std::invalid_argument(
				"Width and m_Height must be greater than 0");
		}

		RedoColorAdjustment(bgCol);
		m_BG = bgCol;
	}
	ImageArray::~ImageArray() {
		//m_Data.clear();
		//m_Data.shrink_to_fit();
	}

	void ImageArray::RedoColorAdjustment(Col bgCol) {
		Resize();

		for (int x = 0; x < m_Width; x++) {
			for (int y = 0; y < m_Height; y++) {
				Set(x, y, bgCol);
			}
		}
	}
	void ImageArray::Resize() {
		m_Data.resize(m_Width * m_Height);
	}

	// Set the color at the specified (x, y) position
	void ImageArray::Set(int x, int y, const Col& color) {
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) {
			// no
			throw std::out_of_range("Index out of range");
		}
		m_Data[y * m_Width + x] = color;
	}

	// Get the color at the specified (x, y) position
	Col ImageArray::Get(int x, int y) const {
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) {
			// no
			throw std::out_of_range("Index out of range");
		}
		return m_Data[y * m_Width + x];
	}

	void Frame::SetWidth(int width, bool clear) {
		if (m_Pixels == nullptr) {
			m_Pixels = new ImageArray(width, 1, Col());  // default height of 1 bc we dont
										   // know required height yet
		} else {
			if (!clear) {
				const std::vector<Col>& oldData = m_Pixels->Data();
				int oldWidth = m_Pixels->Width();
				int oldHeight = m_Pixels->Height();
				m_Pixels->SetWidth(width);
				m_Pixels->Resize();
				std::vector<Col> newData(width * oldHeight);
				int xOffset = (width - oldWidth) / 2;
				for (int y = 0; y < oldHeight; y++) {
					for (int x = 0; x < oldWidth; x++) {
						if (newData.size() >= oldData.size()) {
							newData[y * width + (x + xOffset)] =
								oldData[y * oldWidth + x];
						} else {
							newData[y * oldWidth + x] =
								oldData[y * oldWidth + x];
						}
					}
				}
				m_Pixels->SetData(newData);
			} else {
				ReInit(width, m_Pixels->Height(), m_Pixels->BgCol());
			}
		}
	}
	void Frame::SetHeight(int height, bool clear) {
		if (m_Pixels == nullptr) {
			m_Pixels = new ImageArray(1, height, Col());
		} else {
			if (!clear) {
				std::vector<Col> oldData = m_Pixels->Data();
				int oldWidth = m_Pixels->Width();
				int oldHeight = m_Pixels->Height();
				m_Pixels->SetHeight(height);
				m_Pixels->Resize();
				std::vector<Col> newData(oldWidth * height);
				int yOffset = (height - oldHeight) / 2;
				for (int y = 0; y < oldHeight; y++) {
					for (int x = 0; x < oldWidth; x++) {
						if (newData.size() >= oldData.size()) {
							newData[(y + yOffset) * oldWidth + x] =
								oldData[y * oldWidth + x];
						} else {
							newData[y * oldWidth + x] =
								oldData[y * oldWidth + x];
						}
					}
				}
				m_Pixels->SetData(newData);
			} else {
				m_Pixels->SetHeight(height);
				m_Pixels->Resize();
				m_Pixels->RedoColorAdjustment(m_Pixels->BgCol());
			}
		}
	}

	void Frame::CopyToClipboard() {
		size_t bufferSize = 32;	 // Start with space for dimensions
		for (int y = 0; y < Height(); y++) {
			for (int x = 0; x < Width(); x++) {
				bufferSize += 32;  // Space for r g b values and spaces/newline
			}
		}

		char* fdata = (char*)malloc(bufferSize);
		if (!fdata)
			return;

		int written = snprintf(fdata, bufferSize, "%dx%d\n", Width(), Height());
		char* curr = fdata + written;

		// horizontal
		for (int y = 0; y < Height(); y++) {
			for (int x = 0; x < Width(); x++) {
				Col pixel = Pixels()->Get(x, y);
				written = snprintf(curr, bufferSize - (curr - fdata),
								   "%d %d %d\n", pixel.r, pixel.g, pixel.b);
				curr += written;
			}
		}

		written = snprintf(curr, bufferSize - (curr - fdata), "%d %d %d", m_Pixels->BgCol().r, m_Pixels->BgCol().g, m_Pixels->BgCol().b);
		curr += written;

		ImGui::SetClipboardText(fdata);
		free(fdata);
	}
	Frame* Frame::PastedFrame() {
		char* pasted = const_cast<char*>(ImGui::GetClipboardText());
		if (!pasted)
			return nullptr;

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
		while (*ptr && *ptr != 'x')
			ptr++;
		if (*ptr == 'x') {
			ptr++;
			height = atoi(ptr);
		}

		if (width <= 0 || height <= 0)
			return nullptr;

		Frame* frame = new Frame(width, height, Col{.r = 255, .g = 255, .b = 255});

		ptr = pasted;
		// Skip first line
		while (*ptr && *ptr != '\n')
			ptr++;
		if (*ptr)
			ptr++;	// Skip the \n

		// Parse pixel m_Data
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Col pixel;
				pixel.r = atoi(ptr);
				while (*ptr && *ptr != ' ')
					ptr++;	// Skip to next number
				if (*ptr)
					ptr++;

				pixel.g = atoi(ptr);
				while (*ptr && *ptr != ' ')
					ptr++;
				if (*ptr)
					ptr++;

				pixel.b = atoi(ptr);
				while (*ptr && *ptr != '\n')
					ptr++;
				if (*ptr)
					ptr++;

				frame->SetPixel(x, y, pixel);
			}
		}

		Col bgCol = Col{.r = 255, .g = 255, .b = 255};

		bgCol.r = atoi(ptr);
		while (*ptr && *ptr != ' ')
			ptr++;
		if(*ptr)
			ptr++;
		bgCol.g = atoi(ptr);
		while (*ptr && *ptr != ' ')
			ptr++;
		if(*ptr)
			ptr++;
		bgCol.b = atoi(ptr);
		frame->PixelsMut()->SetBG(bgCol);

		return frame;
	}
	void Frame::Export(char* filePath) {
		stbi_write_png(filePath, Width(), Height(), 3, Data().data(),
					   Width() * 3);
	}
	Frame& Frame::operator=(const Frame& other) {
		if (this != &other) {
			//delete m_Pixels;
			m_Pixels = new ImageArray(*other.Pixels());
		}
		return *this;
	}
}  // namespace FuncDoodle
