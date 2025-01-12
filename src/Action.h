#pragma once

#include "Frame.h"

namespace FuncDoodle {
	class Action {
		public:
			virtual void Undo() = 0;
			virtual void Redo() = 0;
			virtual ~Action() = default;

			friend class DrawAction;
	};

	class DrawAction : public Action {
		public:
			DrawAction(int x, int y, Col prev, Col next, long frameI, void* proj) : m_X(x), m_Y(y), m_Prev(prev), m_Next(next), m_FrameIndex(frameI), m_Proj(proj) {};
			DrawAction(const DrawAction& other) : Action(other), m_X(other.m_X), m_Y(other.m_Y), m_Prev(other.m_Prev), m_Next(other.m_Next), m_FrameIndex(other.m_FrameIndex), m_Proj(other.m_Proj) {};
			~DrawAction() { }
			void Undo() override;
			void Redo() override;
		private:
			int m_X, m_Y;
			Col m_Prev;
			Col m_Next;
			long m_FrameIndex;
			void* m_Proj;
	};
}
