#pragma once

#include <iostream>

#include "Frame.h"

namespace FuncDoodle {
	class LongIndexArray {
		public:
			LongIndexArray(int width, int height, long initial_capacity = 10)
				: capacity(initial_capacity), size(0), m_Width(width),
				  m_Height(height) {
				if (capacity < 1)
					capacity = 1;
				if (m_Width < 1)
					m_Width = 1;
				if (m_Height < 1)
					m_Height = 1;
				data = (Frame*)malloc(capacity * sizeof(Frame));
				for (int i = 0; i < capacity; ++i) {
					data[i] = Frame(m_Width, m_Height);
				}
				if (!data) {  // Error if allocation fails
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Memory allocation for data failed.\n";
				}
			}

			~LongIndexArray() {
				// was already freed??
				free(data);	 // Always delete data pointer
			}

			void push_back(const Frame* value) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted push_back on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				data[size++] = *value;
				std::cout << "PUSH BACK" << std::endl;
			}

			void push_back_empty() {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted push_back on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				static Frame empty = Frame(m_Width, m_Height);
				data[size++] = empty;
			}

			void insertAfterEmpty(long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted insertAfter on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return;
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				// Shift elements to the right, starting from the end
				for (long i = size; i > index + 1; --i) {
					data[i] = data[i - 1];
				}

				// Insert the new value after the specified index
				static Frame empty = Frame(m_Width, m_Height);
				data[index + 1] = empty;
				++size;
			}

			void insertBeforeEmpty(long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted insertBefore on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return;
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				// Shift elements to the right from the selected index
				for (long i = size; i > index; --i) {
					data[i] = data[i - 1];
				}

				// Insert the new element at the selected index
				static Frame empty = Frame(m_Width, m_Height);
				data[index] = empty;
				++size;
			}

			void insertAfter(long index, const Frame* value) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted insertAfter on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (!value) {
					std::cerr << "Trying to insert nullptr frame" << std::endl;
					return;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return;
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				// Shift elements to the right, starting from the end
				for (long i = size; i > index + 1; --i) {
					data[i] = data[i - 1];
				}

				// Insert the new value after the specified index
				data[index + 1] = *value;
				++size;
			}

			void insertBefore(long index, const Frame* value) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted insertBefore on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (!value) {
					std::cerr << "Trying to insert a nullptr frame."
							  << std::endl;
					return;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return;
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				// Shift elements to the right from the selected index
				for (long i = size; i > index; --i) {
					data[i] = data[i - 1];
				}

				// Insert the new element at the selected index
				data[index] = *value;
				++size;
			}

			void moveBackward(long i) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted moveBackward on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (i <= 0 || i >= size)
					return;	 // Bounds check
				Frame temp = data[i];
				data[i] = data[i - 1];
				data[i - 1] = temp;
			}

			void moveForward(long i) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted moveForward on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (i < 0 || i >= size - 1)
					return;	 // Bounds check
				Frame temp = data[i];
				data[i] = data[i + 1];
				data[i + 1] = temp;
			}

			void remove(long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted remove on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return;
				}

				// Shift all elements after index one position to the left
				for (long i = index; i < size - 1; i++) {
					data[i] = data[i + 1];
				}
				size--;
			}

			Frame* operator[](long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted index access on nullptr data.\n";
					// Return a reference to a dummy value (nullptr equivalent
					// behavior)
					return nullptr;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					// Returning a reference to a dummy value
					return nullptr;
				}

				return &data[index];
			}

			const Frame* operator[](long index) const {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted index access on nullptr data.\n";
					return nullptr;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return nullptr;
				}

				return &data[index];
			}

			Frame* get(long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted get on nullptr data.\n";
					return nullptr;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Index " << index << " is out of range.\n";
					return nullptr;
				}

				return &data[index];
			}

			long getSize() const { return size; }

			long getCapacity() const { return capacity; }

		private:
			Frame* data;
			long capacity;
			long size = 0;
			int m_Width;
			int m_Height;

			void resize(long new_capacity) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file "
							  << __FILE__ << " at line " << __LINE__
							  << ": Attempted resize on nullptr data.\n";
					return;	 // Don't proceed if data is nullptr
				}

				if (new_capacity <= capacity)
					return;
				Frame* new_data = (Frame*)malloc(new_capacity * sizeof(Frame));
				for (long i = 0; i < size; ++i) {
					new_data[i] = data[i];
				}

				//delete[] data;	  // Properly delete the old array
				data = new_data;  // Point to the newly allocated memory
				capacity = new_capacity;
			}
	};
}  // namespace FuncDoodle
