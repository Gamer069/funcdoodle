#include "Action.h"

#include "Project.h"

namespace FuncDoodle {
	void DrawAction::Undo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)
				->AnimFrames()
				->get(m_FrameIndex)
				->SetPixel(m_X, m_Y, m_Prev);
		}
	}
	void DrawAction::Redo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)
				->AnimFrames()
				->get(m_FrameIndex)
				->SetPixel(m_X, m_Y, m_Next);
		}
	}

	void FillAction::Undo() {
		if (m_Proj) {
			for (const std::pair<int, int>& xy : m_Pixels) {
				static_cast<ProjectFile*>(m_Proj)
					->AnimFrames()
					->get(m_FrameIndex)
					->SetPixel(xy.first, xy.second, m_Prev);
			}
		}
	}
	void FillAction::Redo() {
		if (m_Proj) {
			for (const std::pair<int, int>& xy : m_Pixels) {
				static_cast<ProjectFile*>(m_Proj)
					->AnimFrames()
					->get(m_FrameIndex)
					->SetPixel(xy.first, xy.second, m_Next);
			}
		}
	}

	void DeleteFrameAction::Undo() {
		if (m_Proj) {
			if (m_Empty) {
				if (m_FrameIndex > 1)
					static_cast<ProjectFile*>(m_Proj)
						->AnimFrames()
						->insertBeforeEmpty(m_FrameIndex);
				else
					static_cast<ProjectFile*>(m_Proj)
						->AnimFrames()
						->push_back_empty();
			} else {
				if (m_FrameIndex > 1)
					static_cast<ProjectFile*>(m_Proj)
						->AnimFrames()
						->insertBefore(m_FrameIndex, m_Frame);
				else
					static_cast<ProjectFile*>(m_Proj)->AnimFrames()->push_back(
						m_Frame);
			}
		}
	}
	void DeleteFrameAction::Redo() {
		if (m_Proj)
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->remove(
				m_FrameIndex);
	}

	void InsertFrameAction::Undo() {
		if (m_Proj)
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->remove(
				m_FrameIndex);
	}
	void InsertFrameAction::Redo() {
		if (m_Proj) {
			if (m_Empty) {
				if (m_FrameIndex > 1)
					static_cast<ProjectFile*>(m_Proj)
						->AnimFrames()
						->insertBeforeEmpty(m_FrameIndex);
				else
					static_cast<ProjectFile*>(m_Proj)
						->AnimFrames()
						->push_back_empty();
			} else {
				if (m_FrameIndex > 1)
					static_cast<ProjectFile*>(m_Proj)
						->AnimFrames()
						->insertBefore(m_FrameIndex, m_Frame);
				else
					static_cast<ProjectFile*>(m_Proj)->AnimFrames()->push_back(
						m_Frame);
			}
		}
	}
	void MoveFrameLeftAction::Undo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->moveForward(
				m_FrameIndex - 1);
		}
	}
	void MoveFrameLeftAction::Redo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->moveBackward(
				m_FrameIndex);
		}
	}
	void MoveFrameRightAction::Undo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->moveBackward(
				m_FrameIndex);
		}
	}
	void MoveFrameRightAction::Redo() {
		if (m_Proj) {
			static_cast<ProjectFile*>(m_Proj)->AnimFrames()->moveForward(
				m_FrameIndex);
		}
	}
}  // namespace FuncDoodle
