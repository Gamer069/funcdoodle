#pragma once

#include "Frame.h"

namespace FuncDoodle {
	class Action {
		public:
			virtual void Undo() = 0;
			virtual void Redo() = 0;
			virtual ~Action() = default;

			friend class DrawAction;
			friend class FillAction;
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
	class FillAction : public Action {
		public:
			FillAction(Col prev, Col next, long frameI, void* proj, std::vector<std::tuple<int>> affected) : m_Prev(prev), m_Next(next), m_FrameIndex(frameI), m_Proj(proj), m_Pixels(affected) {};
			FillAction(const FillAction& other) : Action(other), m_Prev(other.m_Prev), m_Next(other.m_Next), m_FrameIndex(other.m_FrameIndex), m_Proj(other.m_Proj), m_Pixels(other.m_Pixels) {};
			~FillAction() {}
			void Undo() override;
			void Redo() override;
		private:
			Col m_Prev;
			Col m_Next;
			long m_FrameIndex;
			void* m_Proj;
			std::vector<std::tuple<int>> m_Pixels;
	};
}
