#pragma once

#include <iostream>

#include "Frame.h"

namespace FuncDoodle {
	class LongIndexArray {
		public:
			LongIndexArray(int width, int height, long initial_capacity = 10);
			~LongIndexArray();

			void PushBack(const Frame* value);
			void PushBackEmpty();
			void InsertAfterEmpty(long index);
			void InsertBeforeEmpty(long index);
			void InsertAfter(long index, const Frame* value);
			void InsertAfter(long index, const Frame val);
			void InsertBefore(long index, const Frame* value);
			void InsertBefore(long index, const Frame val);
			void MoveBackward(long i);
			void MoveForward(long i);
			void Remove(long index);

			Frame* operator[](long index);
			const Frame* operator[](long index) const;
			Frame* Get(long index);

			inline long Size() const { return size; }
			inline long Capacity() const { return m_Capacity; }

		private:
			Frame* m_Data;
			long m_Capacity;
			long size = 0;
			int m_Width;
			int m_Height;

			void Resize(long new_capacity);
	};
}  // namespace FuncDoodle
