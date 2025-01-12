#include "Action.h"

#include "Project.h"

namespace FuncDoodle {
	void DrawAction::Undo() {
		// doesnt get called
		std::cout << "UNDO DOES GET CALLED" << std::endl;
		std::cout << m_FrameIndex << " -- frame index" << std::endl;
		std::cout << m_X << " -- x" << std::endl;
		std::cout << m_Y << " -- y" << std::endl;
		std::cout << m_Prev << " -- prev" << std::endl;
		std::cout << m_Next << " -- next" << std::endl;
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(m_X, m_Y, m_Prev);
		} else {
			std::cout << "PROJECT IS NULL?!?!?!?!?!" << std::endl;
		}
	}
	void DrawAction::Redo() {
		std::cout << "REDO" << std::endl;
		std::cout << m_FrameIndex << " -- frame index" << std::endl;
		std::cout << m_X << " -- x" << std::endl;
		std::cout << m_Y << " -- y" << std::endl;
		std::cout << m_Prev << " -- prev" << std::endl;
		std::cout << m_Next << " -- next" << std::endl;
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->get(m_FrameIndex)->SetPixel(m_X, m_Y, m_Next);
		} else {
			std::cout << "PROJECT IS NULL?!?!?!?!?!(redo)" << std::endl;
		}
	}
}
