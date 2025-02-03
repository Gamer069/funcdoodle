#pragma once

#include "DynArr.h"

#include <stack>

#include "Action.h"

#include "MacroUtils.h"

#include <memory>
#include <algorithm>

#include <GLFW/glfw3.h>

namespace FuncDoodle {
	class ProjectFile {
		public:
			ProjectFile(char name[256], int width, int height, char author[100],
						int fps, char desc[512], GLFWwindow* win, Col bgCol);
			~ProjectFile();
			const char* AnimName() const;
			void SetAnimName(char name[256]);
			const int AnimWidth() const;
			void SetAnimWidth(int width, bool clear = false);
			const int AnimHeight() const;
			void SetAnimHeight(int height, bool clear = false);
			const char* AnimAuthor() const;
			void SetAnimAuthor(char* author);
			const int AnimFPS() const;
			void SetAnimFPS(int FPS);
			const char* AnimDesc() const;
			void SetAnimDesc(char* desc);
			const unsigned long AnimFrameCount() const;
			void SetAnimFrameCount(unsigned long count);
			inline void SetBgCol(const float* bgCol) {
				// Ensure bgCol is valid and has at least 3 elements
				if (bgCol) {
					m_BG = Col{
						.r = static_cast<unsigned char>(std::clamp(bgCol[0] * 255, 0.0f, 255.0f)),
						.g = static_cast<unsigned char>(std::clamp(bgCol[1] * 255, 0.0f, 255.0f)),
						.b = static_cast<unsigned char>(std::clamp(bgCol[2] * 255, 0.0f, 255.0f))
					};
				}
				m_Frames = new LongIndexArray(m_Width, m_Height, m_BG);
				m_Frames->PushBackEmpty();
   			}
			inline const Col BgCol() { return m_BG; }
			LongIndexArray* AnimFrames();
			void Write(char* filePath);
			void ReadAndPopulate(char* filePath);
			void Export(char* filePath, int format);
			constexpr inline GLFWwindow* Window() const { return m_Window; }
			inline bool Saved() { return m_Saved; }
			void DisplayFPS();

			// Undo management
			void PushUndoableDrawAction(DrawAction action);
			void PushUndoableFillAction(FillAction action);
			void PushUndoableDeleteFrameAction(DeleteFrameAction action);
			void PushUndoableInsertFrameAction(InsertFrameAction action);
			void PushUndoableMoveFrameLeftAction(MoveFrameLeftAction action);
			void PushUndoableMoveFrameRightAction(MoveFrameRightAction action);

			void Undo();
			void Redo();

		private:
			char m_Name[256];  // 256
			int m_Width = 0;
			int m_Height = 0;
			char m_Author[100];	 // 100
			int m_FPS = 0;
			char m_Desc[512];  // 512
			LongIndexArray* m_Frames;
			GLFWwindow* m_Window;
			std::stack<std::unique_ptr<Action>> m_UndoStack;
			std::stack<std::unique_ptr<Action>> m_RedoStack;
			bool m_Saved = false;
			Col m_BG;
	};
}  // namespace FuncDoodle
