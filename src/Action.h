#pragma once

#include "Tool.h"

namespace FuncDoodle {
	class Action {
		public:
			virtual void Undo() = 0;
			virtual void Redo() = 0;
			virtual ~Action() = default;

			friend class DrawAction;
		private:
			void* mf_Proj;
			long mf_FrameIndex;
	};

	class DrawAction : public Action {
		public:
			DrawAction(int x, int y, Col prev, Col next) : m_X(x), m_Y(y), m_Prev(prev), m_Next(next) {};
			DrawAction(const DrawAction& other) : Action(other), m_X(other.m_X), m_Y(other.m_Y), m_Prev(other.m_Prev), m_Next(other.m_Next) {};
			~DrawAction() { }
			void Undo() override;
			void Redo() override;
		private:
			int m_X, m_Y;
			Col m_Prev;
			Col m_Next;
	};
}
