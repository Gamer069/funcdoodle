#include "Action.h"

#include "Project.h"

namespace FuncDoodle {
	void DrawAction::Undo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(m_X, m_Y, m_Prev);
		} else {
			std::cout << "PROJECT IS NULL?!?!?!?!?!" << std::endl;
		}
	}
	void DrawAction::Redo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(m_X, m_Y, m_Next);
		} else {
			std::cout << "PROJECT IS NULL?!?!?!?!?!(redo)" << std::endl;
		}
	}

	void FillAction::Undo() {
		if (m_Proj) {
			for (const std::pair<int, int>& xy : m_Pixels) {
				static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(xy.first, xy.second, m_Prev);
			}
		} else {
			std::cout << "PROJECT IS NULL?!?!?!?!?!(undo fill)" << std::endl;
		}
	}
	void FillAction::Redo() {
		if (m_Proj) {
			for (const std::pair<int, int>& xy : m_Pixels) {
				static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(xy.first, xy.second, m_Next);
			}
		} else {
			std::cout << "PROJECT IS NULL?!?!?!?!?!(redo fill)" << std::endl;
		}
	}
}
