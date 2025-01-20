#include "DynArr.h"

#include "Frame.h"
#include <iostream>

namespace FuncDoodle {
	LongIndexArray::LongIndexArray(int width, int height, long initialcap)
		: m_Capacity(initialcap), size(0), m_Width(width), m_Height(height) {
		if (m_Capacity < 1)
			m_Capacity = 1;
		if (m_Width < 1)
			m_Width = 1;
		if (m_Height < 1)
			m_Height = 1;
		m_Data = (Frame*)malloc(m_Capacity * sizeof(Frame));
		for (int i = 0; i < m_Capacity; ++i) {
			m_Data[i] = Frame(m_Width, m_Height);
		}
		if (!m_Data) {	// Error if allocation fails
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Memory allocation for m_Data failed.\n";
		}
	}

	LongIndexArray::~LongIndexArray() {
		free(m_Data);  // Always delete m_Data pointer
	}

	void LongIndexArray::PushBack(const Frame* value) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted push_back on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		m_Data[size++] = *value;
		std::cout << "PUSH BACK" << std::endl;
	}

	void LongIndexArray::PushBackEmpty() {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted push_back on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		static Frame empty = Frame(m_Width, m_Height);
		m_Data[size++] = empty;
	}

	void LongIndexArray::InsertAfterEmpty(long index) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted insertAfter on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right, starting from the end
		for (long i = size; i > index + 1; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new value after the specified index
		Frame* empty = new Frame(m_Width, m_Height);
		m_Data[index + 1] = *empty;
		++size;
	}

	void LongIndexArray::InsertBeforeEmpty(long index) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted insertBefore on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right from the selected index
		for (long i = size; i > index; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new element at the selected index
		Frame* empty = new Frame(m_Width, m_Height);
		m_Data[index] = *empty;
		++size;
	}

	void LongIndexArray::InsertAfter(long index, const Frame* value) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted insertAfter on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (!value) {
			std::cerr << "Trying to insert nullptr frame" << std::endl;
			return;
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right, starting from the end
		for (long i = size; i > index + 1; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new value after the specified index
		m_Data[index + 1] = *value;
		++size;
	}

	void LongIndexArray::InsertAfter(long index, const Frame val) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted insertAfter on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right, starting from the end
		for (long i = size; i > index + 1; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new value after the specified index
		m_Data[index + 1] = val;
		++size;
	}

	void LongIndexArray::InsertBefore(long index, const Frame* value) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted insertBefore on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (!value) {
			std::cerr << "Trying to insert a nullptr frame." << std::endl;
			return;
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right from the selected index
		for (long i = size; i > index; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new element at the selected index
		m_Data[index] = *value;
		++size;
	}
	void LongIndexArray::InsertBefore(long index, const Frame val) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted insertBefore on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}
		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		if (size == m_Capacity) {
			Resize(m_Capacity * 2);
		}

		// Shift elements to the right from the selected index
		for (long i = size; i > index; --i) {
			m_Data[i] = m_Data[i - 1];
		}

		// Insert the new element at the selected index
		m_Data[index] = val;
		++size;
	}

	void LongIndexArray::MoveBackward(long i) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted moveBackward on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (i <= 0 || i >= size)
			return;	 // Bounds check
		Frame temp = m_Data[i];
		m_Data[i] = m_Data[i - 1];
		m_Data[i - 1] = temp;
	}

	void LongIndexArray::MoveForward(long i) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted moveForward on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (i < 0 || i >= size - 1)
			return;	 // Bounds check
		Frame temp = m_Data[i];
		m_Data[i] = m_Data[i + 1];
		m_Data[i + 1] = temp;
	}

	void LongIndexArray::Remove(long index) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted remove on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return;
		}

		// Shift all elements after index one position to the left
		for (long i = index; i < size - 1; i++) {
			m_Data[i] = m_Data[i + 1];
		}
		size--;
	}

	Frame* LongIndexArray::operator[](long index) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted index access on nullptr m_Data.\n";
			// Return a reference to a dummy value (nullptr equivalent
			// behavior)
			return nullptr;
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			// Returning a reference to a dummy value
			return nullptr;
		}

		return &m_Data[index];
	}

	const Frame* LongIndexArray::operator[](long index) const {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted index access on nullptr m_Data.\n";
			return nullptr;
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return nullptr;
		}

		return &m_Data[index];
	}

	Frame* LongIndexArray::Get(long index) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted get on nullptr m_Data.\n";
			return nullptr;
		}

		if (index < 0 || index >= size) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__ << ": Index " << index
					  << " is out of range.\n";
			return nullptr;
		}

		return &m_Data[index];
	}

	void LongIndexArray::Resize(long newCap) {
		if (!m_Data) {
			std::cerr << "Error in " << __func__ << " in file " << __FILE__
					  << " at line " << __LINE__
					  << ": Attempted Resize on nullptr m_Data.\n";
			return;	 // Don't proceed if m_Data is nullptr
		}

		if (newCap <= m_Capacity)
			return;
		Frame* newData = (Frame*)malloc(newCap * sizeof(Frame));
		for (long i = 0; i < size; ++i) {
			newData[i] = m_Data[i];
		}

		m_Data = newData;
		m_Capacity = newCap;
	}
}  // namespace FuncDoodle
