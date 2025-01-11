#include "Action.h"

#include "Project.h"

namespace FuncDoodle {
	void DrawAction::Undo() {
		// doesnt get called
		std::cout << "UNDO DOES GET CALLED" << std::endl;
		std::cout << m_FrameIndex << " -- frame index" << std::endl;
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(m_X, m_Y, m_Prev);
		}
	}
	void DrawAction::Redo() {
		if (m_Proj)
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(m_X, m_Y, m_Next);
	}
}
