#include "Action.h"

#include "Project.h"

namespace FuncDoodle {
	void DrawAction::Undo() {
		if (mf_Proj)
			static_cast<ProjectFile*>(mf_Proj)->AnimFrames()->get(mf_FrameIndex)->SetPixel(m_X, m_Y, m_Prev);
	}
	void DrawAction::Redo() {
		if (mf_Proj)
			static_cast<ProjectFile*>(mf_Proj)->AnimFrames()->get(mf_FrameIndex)->SetPixel(m_X, m_Y, m_Next);
	}
}
