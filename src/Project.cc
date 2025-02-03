#include "Project.h"

#include "Frame.h"

#include <string.h>

#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

#include <GLFW/glfw3.h>

#include "MacroUtils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <ctime>

#include <stdlib.h>

#define WRITEB(b)                                                              \
	do {                                                                       \
		outFile.write(reinterpret_cast<const char*>(&(b)), sizeof((b)));       \
	} while (0)

namespace FuncDoodle {
	ProjectFile::ProjectFile(char name[], int width, int height, char author[],
							 int fps, char desc[], GLFWwindow* win, Col bgCol)
		: m_Window(win) {
		strcpy(m_Name, name);
		m_Width = width;
		m_Height = height;
		strcpy(m_Author, author);
		m_FPS = fps;
		strcpy(m_Desc, desc);

		m_BG = bgCol;

		m_Frames = new LongIndexArray(width, height, bgCol);
		m_Frames->PushBackEmpty();

		m_UndoStack = std::stack<std::unique_ptr<Action>>();
		m_RedoStack = std::stack<std::unique_ptr<Action>>();
	}
	ProjectFile::~ProjectFile() {
		delete m_Frames;
	}

	const char* ProjectFile::AnimName() const {
		return m_Name;
	}
	void ProjectFile::SetAnimName(char name[]) {
		strcpy(m_Name, name);
	}

	void ProjectFile::Export(char* filePath, int format) {
		FUNC_GRAY(strcat((char*)"Exporting to ", filePath));

		LongIndexArray* frames = AnimFrames();

		char curFilePath[512];

		for (unsigned long i = 0; i < AnimFrameCount(); i++) {
#ifndef _WIN32
			sprintf(curFilePath, "%s/frame_%ld.png", filePath, i);
#else
			sprintf(curFilePath, "%s\\frame_%ld.png", filePath, i);
#endif
			frames->Get(i)->Export(curFilePath);
		}

		if (format == 1) {
			FUNC_GRAY("Exporting to mp4...");
			// TODO: properly implement video exporting...
			char cmd[1024];
#ifndef _WIN32
			sprintf(
				cmd,
				"ffmpeg -framerate %d -pattern_type glob -i \"%s/frame_*.png\" "
				"-c:v libx264 -pix_fmt yuv420p %s/result.mp4 -y",
				m_FPS, filePath, filePath);
#else
			sprintf(cmd,
					"ffmpeg.exe -framerate %d -pattern-type glob -i "
					"\"%s/frame_*.png\" "
					"-c:v libx264 -pix_fmt yuv420p %s\\result.mp4 -y",
					m_FPS, filePath, filePath, filePath);
#endif

			system(cmd);
		}

		if (format > 1) {
			FUNC_WARN("Failed to export animation -- format not yet supported, this shouldn't normally occur unless there's a bug. Submit a github issue");
			std::exit(-1);
		}
	}

	const int ProjectFile::AnimWidth() const {
		return m_Width;
	}

	void ProjectFile::SetAnimWidth(int width, bool clear) {
		for (unsigned long i = 0; i < AnimFrameCount(); ++i) {
			m_Frames->Get(i)->SetWidth(width, clear);
		}
		m_Width = width;
	}

	const int ProjectFile::AnimHeight() const {
		return m_Height;
	}
	void ProjectFile::SetAnimHeight(int height, bool clear) {
		for (unsigned long i = 0; i < AnimFrameCount(); ++i) {
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

	const unsigned long ProjectFile::AnimFrameCount() const {
		return m_Frames->Size();
	}
	LongIndexArray* ProjectFile::AnimFrames() {
		return m_Frames;
	}

	void ProjectFile::PushUndoableDrawAction(DrawAction action) {
		m_UndoStack.push(std::make_unique<DrawAction>(std::move(action)));
		m_Saved = false;
	}
	void ProjectFile::PushUndoableFillAction(FillAction action) {
		m_UndoStack.push(std::make_unique<FillAction>(std::move(action)));
		m_Saved = false;
	}
	void ProjectFile::PushUndoableDeleteFrameAction(DeleteFrameAction action) {
		m_UndoStack.push(std::make_unique<DeleteFrameAction>(std::move(action)));
		m_Saved = false;
	}
	void ProjectFile::PushUndoableInsertFrameAction(InsertFrameAction action) {
		m_UndoStack.push(std::make_unique<InsertFrameAction>(std::move(action)));
		m_Saved = false;
	}
	void ProjectFile::PushUndoableMoveFrameLeftAction(MoveFrameLeftAction action) {
		m_UndoStack.push(std::make_unique<MoveFrameLeftAction>(std::move(action)));
		m_Saved = false;
	}
	void ProjectFile::PushUndoableMoveFrameRightAction(MoveFrameRightAction action) {
		m_UndoStack.push(std::make_unique<MoveFrameRightAction>(std::move(action)));
		m_Saved = false;
	}
	void ProjectFile::Undo() {
		if (m_UndoStack.empty()) {
			FUNC_INF("Nothing to undo");
			return;
		}

		m_Saved = false;

		std::unique_ptr<Action> action = std::move(m_UndoStack.top());
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
		std::unique_ptr<Action> action = std::move(m_RedoStack.top());
		m_RedoStack.pop();

		action->Redo();

		m_UndoStack.push(std::move(action));
	}

	void ProjectFile::Write(char* fileName) {
		std::ofstream outFile(fileName, std::ios::binary);
		if (!outFile.is_open()) {
			FUNC_WARN("Failed to open file for writing");
			return;
		}

		outFile << "FDProj";
		// 0.3
		unsigned long frames = m_Frames->Size();
		WRITEB(FDPVERMAJOR);
		WRITEB(FDPVERMINOR);
		WRITEB(frames);	   // frame count (default)
		WRITEB(m_Width);   // animation width
		WRITEB(m_Height);  // animation height
		WRITEB(m_FPS);	   // animation fps
		unsigned char null = 0;
		outFile << m_Name;	// animation name
		WRITEB(null);
		outFile << m_Desc;	// animation description
		WRITEB(null);
		outFile << m_Author;  // animation author
		WRITEB(null);
		outFile << m_BG;
		WRITEB(null);

		LongIndexArray* frameData = AnimFrames();

		// Use a vector and maintain stable indices
		std::vector<Col> uniqueColors;
		std::map<Col, int> colorToIndex;  // Map each color to its stable index

		// First pass: collect unique colors with stable ordering
		for (unsigned long i = 0; i < AnimFrameCount(); i++) {
			auto pixels = frameData->Get(i)->Pixels();
			for (int x = 0; x < pixels->Width(); x++) {
				for (int y = 0; y < pixels->Height(); y++) {
					Col px = pixels->Get(x, y);
					if (colorToIndex.find(px) == colorToIndex.end()) {
						colorToIndex[px] = uniqueColors.size();
						uniqueColors.push_back(px);
					}
				}
			}
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
		for (unsigned long i = 0; i < AnimFrameCount(); i++) {
			auto pixels = frameData->Get(i)->Pixels();
			for (int y = 0; y < pixels->Height(); y++) {
				for (int x = 0; x < pixels->Width(); x++) {
					Col px = pixels->Get(x, y);
					int index = colorToIndex[px];  // Get stable index
					WRITEB(index);
				}
			}
			unsigned char null = 0;
			WRITEB(null);
		}

		outFile << "EOP";
		outFile.close();
	}
	void ProjectFile::ReadAndPopulate(char* filePath) {
		std::ifstream file(filePath, std::ios::in | std::ios::binary);

		if (!file.is_open()) {
			FUNC_WARN("Failed to open file");
			std::exit(-1);
		}

		const int numBytes = 6;
		std::string str(numBytes, '\0');

		file.read(&str[0], numBytes);

		if (str != "FDProj") {
			FUNC_WARN("This isn't a funcdoodle project...");
			std::exit(-1);
		}

		int verMajor = 0;
		file.read(reinterpret_cast<char*>(&verMajor), sizeof(verMajor));
		int verMinor = 0;
		file.read(reinterpret_cast<char*>(&verMinor), sizeof(verMinor));
		if (verMinor != FDPVERMINOR || verMajor != FDPVERMAJOR) {
			FUNC_WARN("The project version " + std::to_string(FDPVERMAJOR) + "." + std::to_string(FDPVERMINOR) + " isn't supported by this version by FuncDoodle.");
			FUNC_GRAY("Perhaps migrate your project file to this version: " + std::to_string(FDPVERMAJOR) + "." + std::to_string(FDPVERMINOR));
			std::exit(-1);
		}
		unsigned long frameCount = 0;
		file.read(reinterpret_cast<char*>(&frameCount), sizeof(frameCount));
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
			FUNC_WARN("Failed to read file");
		}

		file.getline(m_Desc, sizeof(m_Desc), '\0');

		file.getline(m_Author, sizeof(m_Author), '\0');

		unsigned char bgR = 255;
		unsigned char bgG = 255;
		unsigned char bgB = 255;

		file.read(reinterpret_cast<char*>(&bgR), sizeof(bgR));
		file.read(reinterpret_cast<char*>(&bgG), sizeof(bgG));
		file.read(reinterpret_cast<char*>(&bgB), sizeof(bgB));

		FUNC_DBG("BG RGB - " + std::to_string(bgR) + ";" + std::to_string(bgG) + ";" + std::to_string(bgB));

		std::vector<Col> plte;

		std::size_t plteLen = 0;

		if (file.fail()) {
			FUNC_WARN("Failed to read file");
		}

		file.read(reinterpret_cast<char*>(&plteLen), sizeof(plteLen));

		if (file.fail()) {
			FUNC_WARN("Failed to read file");
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

		m_Frames = new LongIndexArray(m_Width, m_Height, m_BG);

		for (unsigned long i = 0; i < frameCount; i++) {
			ImageArray* img = new ImageArray(animWidth, animHeight, m_BG);
			for (int y = 0; y < animHeight; y++) {
				for (int x = 0; x < animWidth; x++) {
					unsigned char bytes[sizeof(int)];
					file.read(reinterpret_cast<char*>(bytes), sizeof(int));

					int index = *reinterpret_cast<int*>(bytes);

					if (index > plteLen) {
						FUNC_WARN("Index out of bounds -- maybe the project file is corrupted..?");
						std::exit(-1);
					}

					img->Set(x, y, plte[index]);
				}
			}
			Frame newFrame = Frame(img);
			m_Frames->PushBack(&newFrame);

			unsigned char null;
			file.read(reinterpret_cast<char*>(&null), 1);
		}
		m_Width = animWidth;
		m_Height = animHeight;
		m_FPS = animFPS;
		// maybe decide if the color space is needed cos everything is sRGB
		// nowadays.

		char eop[4];
		file.getline(eop, sizeof(eop), '\0');

		if (strcmp(eop, "EOP") == 0) {
			FUNC_INF("SUCCESS READING FILE!");
		}

		if (!file) {
			FUNC_WARN("Failed to read from file");
			std::exit(-1);
		}

		file.close();
		m_Saved = true;
	}

	void ProjectFile::DisplayFPS() {
		char* title = (char*)malloc(1024);
		sprintf(title, "FuncDoodle -- %s -- %s (%d FPS)%s", FUNCVER, AnimName(),
				(int)ImGui::GetIO().Framerate, !m_Saved ? "*" : "");
		glfwSetWindowTitle(m_Window, title);
		free(title);
	}
}  // namespace FuncDoodle
