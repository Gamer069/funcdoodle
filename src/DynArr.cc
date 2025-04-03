#include "DynArr.h"

#include "Frame.h"
#include <iostream>

#include "MacroUtils.h"

namespace FuncDoodle {
	LongIndexArray::LongIndexArray(int width, int height, Col bgCol,
								   unsigned long initialcap)
		: m_Capacity(initialcap), size(0), m_Width(width), m_Height(height) {
		if (m_Capacity < 1)
			m_Capacity = 1;
		if (m_Width < 1)
			m_Width = 1;
		if (m_Height < 1)
			m_Height = 1;
		m_Data = (Frame*)malloc(m_Capacity * sizeof(Frame));
		for (int i = 0; i < m_Capacity; ++i) {
			m_Data[i] = Frame(m_Width, m_Height, bgCol);
		}
		if (!m_Data) {	// Error if allocation fails
			FUNC_DBG("Memory allocation for m_Data failed.");
			FUNC_FATAL("Failed to initialize LongIndexArray");
		}

		m_BG = bgCol;
	}

	LongIndexArray::~LongIndexArray() {
		free(m_Data);  // Always delete m_Data pointer
	}

	void LongIndexArray::PushBack(const Frame* value) {
		if (!m_Data) {
			FUNC_ERR("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted push_back on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		m_Data[size++] = *value;
		FUNC_DBG("PUSH BACK TO LONGINDEXARRAY");
	}

	void LongIndexArray::PushBackEmpty() {
		if (!m_Data) {
			FUNC_FATAL("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted push_back on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		Frame empty = Frame(m_Width, m_Height, m_BG);
		m_Data[size++] = empty;
	}

	void LongIndexArray::InsertAfterEmpty(unsigned long index) {
		if (!m_Data) {
			FUNC_ERR("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted insert_after on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			FUNC_ERR("index out of range");
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right, starting from the end
		for (unsigned long i = size; i > index + 1; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		Frame* empty = new Frame(m_Width, m_Height, m_BG);
		m_Data[index + 1] = *empty;
		++size;
	}

	void LongIndexArray::InsertBeforeEmpty(unsigned long index) {
		if (!m_Data) {
			FUNC_ERR("Tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("InsertBeforeEmpty");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right from the selected index
		for (unsigned long i = size; i > index; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new element at the selected index
		Frame* empty = new Frame(m_Width, m_Height, m_BG);
		m_Data[index] = *empty;
		++size;
	}

	void LongIndexArray::InsertAfter(unsigned long index, const Frame* value) {
		if (!m_Data) {
			FUNC_WARN("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted insert_after on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (!value) {
			FUNC_WARN("Tried to insert invalid frame");
			return;
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right, starting from the end
		for (unsigned long i = size; i > index + 1; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new value after the specified index
		m_Data[index + 1] = *value;
		++size;
	}

	void LongIndexArray::InsertAfter(unsigned long index, const Frame val) {
		if (!m_Data) {
			FUNC_WARN("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted insert_after on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right, starting from the end
		for (unsigned long i = size; i > index + 1; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new value after the specified index
		m_Data[index + 1] = val;
		++size;
	}

	void LongIndexArray::InsertBefore(unsigned long index, const Frame* value) {
		if (!m_Data) {
			FUNC_WARN("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted insert_before on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (!value) {
			FUNC_WARN("tried to add invalid frame");
			return;
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right from the selected index
		for (unsigned long i = size; i > index; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new element at the selected index
		m_Data[index] = *value;
		++size;
	}
	void LongIndexArray::InsertBefore(unsigned long index, const Frame val) {
		if (!m_Data) {
			FUNC_WARN("tried to add a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted insert_before on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}
		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right from the selected index
		for (unsigned long i = size; i > index; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new element at the selected index
		m_Data[index] = val;
		++size;
	}

	void LongIndexArray::MoveBackward(unsigned long i) {
		if (!m_Data) {
			FUNC_WARN("tried to move a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted move_backward on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (i <= 0 || i >= size)
			return;	 // Bounds check
		Frame temp = m_Data[i];
		m_Data[i] = m_Data[i - 1];
		m_Data[i - 1] = temp;
	}

	void LongIndexArray::MoveForward(unsigned long i) {
		if (!m_Data) {
			FUNC_WARN("tried to move a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted move_forward on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (i < 0 || i >= size - 1)
			return;	 // Bounds check
		Frame temp = m_Data[i];
		m_Data[i] = m_Data[i + 1];
		m_Data[i + 1] = temp;
	}

	void LongIndexArray::Remove(unsigned long index) {
		if (!m_Data) {
			FUNC_WARN("Tried to delete a frame on invalid LongIndexArray");
			FUNC_DBG("Attempted remove on nullptr m_Data.");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return;
		}

		// Shift all elements after index one position to the left
		for (unsigned long i = index; i < size - 1; i++) {
			m_Data[i] = m_Data[i + 1];
		}
		size--;
	}

	Frame* LongIndexArray::operator[](unsigned long index) {
		if (!m_Data) {
			FUNC_WARN("Attempted to access index on invalid LongIndexArray");
			FUNC_DBG("operator[], NOT LongIndexArray::Get even though they're "
					 "the exact same");
			return nullptr;
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			// Returning a reference to a dummy value
			return nullptr;
		}

		return &m_Data[index];
	}

	const Frame* LongIndexArray::operator[](unsigned long index) const {
		if (!m_Data) {
			FUNC_WARN("Attempted to access index on invalid LongIndexArray");
			FUNC_DBG("const operator[], NOT LongIndexArray::Get even though "
					 "they're the exact same");
			return nullptr;
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return nullptr;
		}

		return &m_Data[index];
	}

	Frame* LongIndexArray::Get(unsigned long index) {
		if (!m_Data) {
			FUNC_WARN("Attempted to access index on invalid LongIndexArray");
			FUNC_DBG("LongIndexArray::Get, NOT const operator[] even though "
					 "they're the exact same");
			return nullptr;
		}

		if (index < 0 || index >= size) {
			FUNC_WARN("index out of range");
			return nullptr;
		}

		return &m_Data[index];
	}

	void LongIndexArray::Resize(unsigned long newCap) {
		if (!m_Data) {
			FUNC_WARN("Attempted to resize invalid LongIndexArray");
			FUNC_DBG("LongIndexArray::Resize, nullptr m_Data");
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (newCap <= m_Capacity)
			return;
		Frame* newData = (Frame*)malloc(newCap * sizeof(Frame));
		for (unsigned long i = 0; i < size; ++i) {
			newData[i] = m_Data[i];
		}

		m_Data = newData;
		m_Capacity = newCap;
	}
}  // namespace FuncDoodle
