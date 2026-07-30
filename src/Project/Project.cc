#include "Conf/FuncPCH.h"

#include "Project.h"

#include "Conf/Constants.h"
#include "Project/Frame.h"

#include <cstdio>
#include <cstring>
#include <memory>

#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <utility>
#include <vector>
#include <av.h>
#include <codec.h>
#include <codeccontext.h>
#include <format.h>
#include <formatcontext.h>
#include <frame.h>
#include <packet.h>
#include <pixelformat.h>
#include <rational.h>
#include <stream.h>
#include <timestamp.h>
#include <videorescaler.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cstdlib>
#include <ctime>
#include <format>

#define WRITEB(b)                                                        \
	do {                                                                 \
		outFile.write(reinterpret_cast<const char*>(&(b)), sizeof((b))); \
	} while (0)

namespace FuncDoodle {
	ProjectFile::ProjectFile(char name[], int width, int height, char author[],
		int fps, char desc[], Platform::Window* win, Col bgCol)
		: m_Window(win) {
		strcpy(m_Name, name);
		m_Width = width;
		m_Height = height;
		strcpy(m_Author, author);
		m_FPS = fps;
		strcpy(m_Desc, desc);

		m_BG = bgCol;

		m_Frames = std::make_shared<LongIndexArray>(width, height, bgCol);
		m_Frames->PushBackEmpty();

		m_UndoStack = std::stack<UniquePtr<Action>>();
		m_RedoStack = std::stack<UniquePtr<Action>>();
	}
	ProjectFile::~ProjectFile() = default;

	const char* ProjectFile::AnimName() const {
		return m_Name;
	}
	void ProjectFile::SetAnimName(char name[]) {
		strcpy(m_Name, name);
	}

	void ProjectFile::Export(const char* filePath, ExportFormat format) {
		if (format == ExportFormat::PNGSequence) {
			auto frames = AnimFrames();
			char curFilePath[g_FilePathBufferSize];
			for (uint64_t i = 0; i < AnimFrameCount(); i++) {
#ifndef _WIN32
				snprintf(curFilePath, sizeof(curFilePath), "%s/frame_%lu.png",
					filePath, i);
#else
				snprintf(curFilePath, sizeof(curFilePath), "%s\\frame_%lu.png",
					filePath, i);
#endif
				frames->Get(i)->Export(curFilePath);
			}
		} else if (format == ExportFormat::MP4) {
			bool oldSaved = m_Saved;

			// HACK: avcpp REFUSES to properly encode the last frame for some reason, so i just push an empty one then cleanup afterwards
			m_Frames->PushBackEmpty();

			av::init();

			av::OutputFormat ofrmt;
			ofrmt.setFormat({}, filePath);

			av::FormatContext octx;
			octx.setFormat(ofrmt);

			av::Codec codec = av::findEncodingCodec("libx264");

			if (codec.isNull()) {
				FUNC_ERR("libx264 codec not installed, so mp4 export is unsupported. plz install it");
				return;
			}

			av::VideoEncoderContext encoder(codec);

			int w = m_Width;
			int h = m_Height;
			encoder.setWidth(w);
			encoder.setHeight(h);
			encoder.setPixelFormat(AV_PIX_FMT_YUV420P);
			encoder.setTimeBase(av::Rational(1, m_FPS));
			encoder.setOption("preset", "veryslow");
			encoder.setOption("crf", "0");

			encoder.setGopSize(m_FPS);

			std::error_code ec;
			encoder.open(ec);
			if (ec) {
				FUNC_ERR("Failed to open encoder: {}", ec.message());
				return;
			}

			av::Stream stream = octx.addStream(encoder, ec);
			if (ec) {
				FUNC_ERR("Failed to add stream: {}", ec.message());
				return;
			}
			stream.setFrameRate(av::Rational(m_FPS, 1));

			octx.openOutput(filePath, ec);
			if (ec) {
				FUNC_ERR("Failed to open output: {}", ec.message());
				return;
			}

			octx.writeHeader(ec);

			if (ec) {
				FUNC_ERR("Failed to write header: {}", ec.message());
				return;
			}

			octx.dump();

			av::VideoRescaler rescaler(w, h, AV_PIX_FMT_YUV420P, w, h,
				AV_PIX_FMT_RGB24, SWS_BILINEAR);

			for (uint64_t i = 0; i < AnimFrameCount(); i++) {
				auto* frame = m_Frames->Get(i);
				const auto& pixelData = frame->Pixels()->Data();

				av::VideoFrame srcFrame = av::VideoFrame::wrap(
					pixelData.data(), pixelData.size() * sizeof(Col),
					AV_PIX_FMT_RGB24, w, h);

				av::VideoFrame yuvFrame = rescaler.rescale(srcFrame, ec);
				if (ec) {
					FUNC_ERR("Failed to rescale frame {}: {}", i, ec.message());
					return;
				}

				yuvFrame.setTimeBase(av::Rational(1, m_FPS));
				yuvFrame.setPts(
					av::Timestamp(static_cast<int64_t>(i), av::Rational(1, m_FPS)));

				av::Packet pkt = encoder.encode(yuvFrame, ec);
				if (ec) {
					FUNC_ERR("Failed to encode frame {}: {}", i, ec.message());
					return;
				}

				if (pkt) {
					pkt.setStreamIndex(stream.index());
					pkt.setTimeBase(av::Rational(1, m_FPS));
					pkt.setDuration(1, av::Rational(1, m_FPS));
					octx.writePacket(pkt, ec);

					if (ec) {
						FUNC_ERR("Failed to write packet: {}", ec.message());
						return;
					}
				}
			}

			// Flush encoder: drain all buffered frames
			while (true) {
				std::error_code flushEc;
				av::Packet pkt = encoder.encode(flushEc);
				if (!pkt)
					break;
				pkt.setStreamIndex(stream.index());
				octx.writePacket(pkt, flushEc);
			}

			octx.writeTrailer(ec);
			octx.close();

			FUNC_INF("mp4 export complete: {}", filePath);

			m_Frames->Remove(m_Frames->Size()-1);
			m_Saved = oldSaved;
		} else {
			FUNC_FATAL("Failed to export animation; format not supported.");
		}
	}

	int ProjectFile::AnimWidth() const {
		return m_Width;
	}

	void ProjectFile::SetAnimWidth(int width, bool clear) {
		for (uint64_t i = 0; i < AnimFrameCount(); ++i) {
			m_Frames->Get(i)->SetWidth(width, clear);
		}
		m_Width = width;
	}

	int ProjectFile::AnimHeight() const {
		return m_Height;
	}
	void ProjectFile::SetAnimHeight(int height, bool clear) {
		for (uint64_t i = 0; i < AnimFrameCount(); ++i) {
			m_Frames->Get(i)->SetHeight(height, clear);
		}
		m_Height = height;
	}

	const char* ProjectFile::AnimAuthor() const {
		return m_Author;
	}
	void ProjectFile::SetAnimAuthor(char* author) {
		strcpy(m_Author, author);
	}

	int ProjectFile::AnimFPS() const {
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

	uint64_t ProjectFile::AnimFrameCount() const {
		return m_Frames->Size();
	}
	SharedPtr<LongIndexArray> ProjectFile::AnimFrames() {
		return m_Frames;
	}

	void ProjectFile::Undo() {
		if (m_UndoStack.empty()) {
			FUNC_INF("Nothing to undo");
			return;
		}

		m_Saved = false;

		UniquePtr<Action> action = std::move(m_UndoStack.top());
		m_UndoStack.pop();

		action->Undo();

		m_RedoStack.push(std::move(action));
	}

	void ProjectFile::Redo() {
		if (m_RedoStack.empty()) {
			FUNC_INF("Nothing to redo");
			return;
		}
		m_Saved = false;
		UniquePtr<Action> action = std::move(m_RedoStack.top());
		m_RedoStack.pop();

		action->Redo();

		m_UndoStack.push(std::move(action));
	}

	void ProjectFile::Write(const char* fileName) {
		unsigned char null = 0;

		std::ofstream outFile(fileName, std::ios::binary);
		if (!outFile.is_open()) {
			FUNC_ERR("Failed to open file for writing");
			return;
		}

		outFile << "FDProj";

		// 0.3
		uint64_t frames = m_Frames->Size();

		int major = c_FdpVerMajor;
		int minor = c_FdpVerMinor;
		WRITEB(major);
		WRITEB(minor);
		WRITEB(frames);		// frame count (default)
		WRITEB(m_Width);	// animation width
		WRITEB(m_Height);	// animation height
		WRITEB(m_FPS);		// animation fps
		outFile << m_Name;	// animation name
		WRITEB(null);
		outFile << m_Desc;	// animation description
		WRITEB(null);
		outFile << m_Author;  // animation author
		WRITEB(null);
		WRITEB(m_BG.r);
		WRITEB(m_BG.g);
		WRITEB(m_BG.b);
		WRITEB(null);

		auto frameData = AnimFrames();

		// Use a vector and maintain stable indices
		std::vector<Col> uniqueColors;
		std::map<Col, int> colorToIndex;  // Map each color to its stable index

		// First pass: collect unique colors with stable ordering
		for (uint64_t i = 0; i < AnimFrameCount(); i++) {
			const auto* pixels = frameData->Get(i)->Pixels();
			for (int x = 0; x < pixels->Width(); x++) {
				for (int y = 0; y < pixels->Height(); y++) {
					Col px = pixels->Get(x, y);
					if (!colorToIndex.contains(px)) {
						colorToIndex[px] = uniqueColors.size();
						uniqueColors.push_back(px);
					}
				}
			}
		}

		if (count(uniqueColors.begin(), uniqueColors.end(), m_BG) == 0) {
			colorToIndex[m_BG] = uniqueColors.size();
			uniqueColors.push_back(m_BG);
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
		for (uint64_t i = 0; i < AnimFrameCount(); i++) {
			const auto* pixels = frameData->Get(i)->Pixels();
			for (int y = 0; y < pixels->Height(); y++) {
				for (int x = 0; x < pixels->Width(); x++) {
					Col px = pixels->Get(x, y);
					int index = colorToIndex[px];
					WRITEB(index);
				}
			}
			unsigned char null = 0;
			WRITEB(null);
		}

		outFile << "EOP";
		outFile.close();
		m_Saved = true;
		m_LastSavePath = fileName;
	}
	void ProjectFile::ReadAndPopulate(const char* filePath) {
		std::ifstream file(filePath, std::ios::in | std::ios::binary);

		if (!file.is_open()) {
			FUNC_FATAL("Failed to open file");
			return;
		}

		const int numBytes = 6;
		std::string str(numBytes, '\0');

		file.read(str.data(), numBytes);

		if (str != "FDProj") {
			FUNC_FATAL("This isn't a funcdoodle project...");
		}

		m_UndoStack = std::stack<UniquePtr<Action>>();
		m_RedoStack = std::stack<UniquePtr<Action>>();

		int verMajor = 0;
		file.read(reinterpret_cast<char*>(&verMajor), sizeof(verMajor));
		int verMinor = 0;
		file.read(reinterpret_cast<char*>(&verMinor), sizeof(verMinor));

		uint64_t frameCount = 0;  // temp val
		if (verMinor >= 5) {
			file.read(reinterpret_cast<char*>(&frameCount), sizeof(frameCount));
		} else {
			unsigned long oldCount = 0;
			file.read(reinterpret_cast<char*>(&oldCount), sizeof(oldCount));
			frameCount = static_cast<uint64_t>(oldCount);
		}

		int animWidth = 0;
		file.read(reinterpret_cast<char*>(&animWidth), sizeof(animWidth));
		int animHeight = 0;
		file.read(reinterpret_cast<char*>(&animHeight), sizeof(animHeight));
		int animFPS = 0;
		file.read(reinterpret_cast<char*>(&animFPS), sizeof(animFPS));

		// BRUH
		m_Width = animWidth;
		m_Height = animHeight;

		file.getline(m_Name, sizeof(m_Name), '\0');

		if (file.fail()) {
			FUNC_FATAL("Failed to read file");
		}

		file.getline(m_Desc, sizeof(m_Desc), '\0');

		file.getline(m_Author, sizeof(m_Author), '\0');

		m_BG.r = g_MaxColorValue;
		m_BG.g = g_MaxColorValue;
		m_BG.b = g_MaxColorValue;

		if (verMajor >= 0 && verMinor >= 1) {
			file.read(reinterpret_cast<char*>(&m_BG.r), sizeof(m_BG.r));
			file.read(reinterpret_cast<char*>(&m_BG.g), sizeof(m_BG.g));
			file.read(reinterpret_cast<char*>(&m_BG.b), sizeof(m_BG.b));
			unsigned char null;
			file.read(reinterpret_cast<char*>(&null), 1);
		} else {
			if (verMajor != c_FdpVerMajor && verMinor != c_FdpVerMinor) {
				verMinor++;
				if (verMinor >= 10) {
					verMinor = 0;
					verMajor++;
				}
			}
		}

		std::vector<Col> plte;

		std::size_t plteLen = 0;

		if (file.fail()) {
			FUNC_FATAL("Failed to read file");
		}

		file.read(reinterpret_cast<char*>(&plteLen), sizeof(plteLen));
		FUNC_DBG("plteLen = {}", plteLen);

		if (file.fail()) {
			FUNC_FATAL("Failed to read file");
		}

		for (std::size_t i = 0; i < plteLen; i++) {
			// read the rgb
			unsigned char r = 0;
			unsigned char g = 0;
			unsigned char b = 0;
			file.read(reinterpret_cast<char*>(&r), sizeof(r));
			file.read(reinterpret_cast<char*>(&g), sizeof(g));
			file.read(reinterpret_cast<char*>(&b), sizeof(b));

			plte.push_back(Col{.r = r, .g = g, .b = b});
		}

		m_Frames = std::make_shared<LongIndexArray>(m_Width, m_Height, m_BG);
		if (verMajor >= 0 && verMinor >= 2) {
			FUNC_GRAY("Reading {} frames...", (uint64_t)frameCount);
			for (uint64_t i = 0; i < frameCount; i++) {
				ImageArray img(animWidth, animHeight, m_BG);
				for (int y = 0; y < animHeight; y++) {
					for (int x = 0; x < animWidth; x++) {
						std::streampos start = file.tellg();
						unsigned char bytes[sizeof(int)];
						file.read(reinterpret_cast<char*>(bytes), sizeof(int));
						int index = *reinterpret_cast<int*>(bytes);
						if (std::cmp_greater_equal(index, plteLen)) {
							FUNC_DBG("Index: {}", index);
							FUNC_DBG("Index out of bounds; maybe the project "
									 "file is corrupted..?");
							FUNC_DBG("trying to break...");
							file.seekg(start);
							break;
						}

						img.Set(x, y, plte[index]);
					}
				}
				Frame newFrame = Frame(&img);
				m_Frames->PushBack(&newFrame);

				unsigned char null;
				file.read(reinterpret_cast<char*>(&null), 1);
			}
		} else {
			FUNC_GRAY("Reading {} frames...", (long)frameCount);
			for (long i = 0; std::cmp_less(i, frameCount); i++) {
				ImageArray img(animWidth, animHeight, m_BG);
				for (int y = 0; y < animHeight; y++) {
					for (int x = 0; x < animWidth; x++) {
						unsigned char bytes[sizeof(int)];
						file.read(reinterpret_cast<char*>(bytes), sizeof(int));

						int index = *reinterpret_cast<int*>(bytes);

						FUNC_DBG(
							"Reading index, x={} y={} index={}", x, y, index);

						if (index < 0 ||
							std::cmp_greater_equal(index, plteLen)) {
							FUNC_WARN("Index out of bounds -- maybe the "
									  "project file is corrupted..?");
							FUNC_INF("Index: {}", index);
							std::exit(-1);
						}

						img.Set(x, y, plte[index]);
					}
				}
				Frame newFrame = Frame(&img);
				m_Frames->PushBack(&newFrame);
				unsigned char null;
				file.read(reinterpret_cast<char*>(&null), 1);
			}
			if (verMajor != c_FdpVerMajor && verMinor != c_FdpVerMinor) {
				verMinor++;
				if (verMinor >= 10) {
					verMinor = 0;
					verMajor++;
				}
			}
		}
		m_Width = animWidth;
		m_Height = animHeight;
		m_FPS = animFPS;
		m_Saved = true;
		// maybe decide if the color space is needed cos everything is sRGB
		// nowadays.

		char eop[4];
		file.getline(eop, sizeof(eop), '\0');

		if (strcmp(eop, "EOP") == 0) {
			FUNC_INF("SUCCESS READING FILE!");
		}

		if (!file) {
			FUNC_FATAL("Failed to read from file");
		}

		file.close();
	}

	void ProjectFile::DisplayAltFPS(double fps) {
		if (ImGui::GetIO().KeyAlt) {
			auto* title = (char*)malloc(g_LargeBufferSize);
			snprintf(title, g_LargeBufferSize, "FuncDoodle %s: %s%s (%d FPS)",
				c_FuncVer, AnimName(), !m_Saved ? "*" : "",
				(int)(fps > 0.0 ? fps : ImGui::GetIO().Framerate));

			m_Window->SetTitle(title);
			free(title);
		}
	}

	void ProjectFile::UpdateTitle() {
		auto* title = (char*)malloc(g_LargeBufferSize);

		snprintf(title, g_LargeBufferSize,
			"FuncDoodle %s: %s%s (alt for details)", c_FuncVer, AnimName(),
			!m_Saved ? "*" : "");

		m_Window->SetTitle(title);
		free(title);
	}
}  // namespace FuncDoodle
