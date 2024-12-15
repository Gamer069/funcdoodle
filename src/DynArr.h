#pragma once

#include <iostream>

#include <execinfo.h>
#include <unistd.h>

namespace FuncDoodle {
    template <typename T>
    class LongIndexArray {
        public:
            LongIndexArray(long initial_capacity = 10)
                : capacity(initial_capacity), size(0), data(nullptr) {
                    if (capacity < 1) capacity = 1;
                    data = new T[capacity];  // Allocate memory for the data
                    if (!data) {  // Error if allocation fails
                        std::cerr << "Error in " << __func__ << " in file " << __FILE__
                                  << " at line " << __LINE__ << ": Memory allocation for data failed.\n";
                    }
                }

            ~LongIndexArray() {
				std::cout << "DELETING LONG INDEX ARRAY!" << __FILE__ << " -- " << __func__ << " on line " << __LINE__ << std::endl;
				const int max_frames = 64;
				void *frames[max_frames];
				int frame_count = backtrace(frames, max_frames);

				// Print the raw backtrace
				char **symbols = backtrace_symbols(frames, frame_count);
				if (symbols != nullptr) {
					for (int i = 0; i < frame_count; i++) {
						printf("%s\n", symbols[i]);
					}
					free(symbols);
				}

				delete[] data;  // Always delete data pointer
			}

			void push_back(const T& value) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted push_back on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (size == capacity) {
					resize(capacity * 2);
				}

				std::cout << "CRASH?! " << (data == nullptr) << std::endl;
				std::cout << "Maybe this is 1..? " << data[0].Width() << std::endl;
				data[size++] = value;
				std::cout << "NOPE" << std::endl;
			}

			void insertAfter(long index, const T& value) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted insertAfter on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Index " << index << " is out of range.\n";
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
				data[index + 1] = value;
				++size;
			}

			void insertBefore(long index, const T& value) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted insertBefore on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Index " << index << " is out of range.\n";
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
				data[index] = value;
				++size;
			}

			void moveBackward(long i) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted moveBackward on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (i <= 0 || i >= size) return;  // Bounds check
				T temp = data[i];
				data[i] = data[i - 1];
				data[i - 1] = temp;
			}

			void moveForward(long i) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted moveForward on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (i < 0 || i >= size - 1) return;  // Bounds check
				T temp = data[i];
				data[i] = data[i + 1];
				data[i + 1] = temp;
			}

			void remove(long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted remove on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Index " << index << " is out of range.\n";
					return;
				}

				// Shift all elements after index one position to the left
				for (long i = index; i < size - 1; i++) {
					data[i] = data[i + 1];
				}
				size--;
			}

			T& operator[](long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted index access on nullptr data.\n";
					// Return a reference to a dummy value (nullptr equivalent behavior)
					static T dummy;
					return dummy;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Index " << index << " is out of range.\n";
					// Returning a reference to a dummy value
					static T dummy;
					return dummy;
				}

				return data[index];
			}

			const T& operator[](long index) const {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted index access on nullptr data.\n";
					static T dummy;
					return dummy;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Index " << index << " is out of range.\n";
					static T dummy;
					return dummy;
				}

				return data[index];
			}

			T& get(long index) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted get on nullptr data.\n";
					static T dummy;
					return dummy;
				}

				if (index < 0 || index >= size) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Index " << index << " is out of range.\n";
					static T dummy;
					return dummy;
				}

				return data[index];
			}

			long getSize() const {
				return size;
			}

			long getCapacity() const {
				return capacity;
			}

		private:
			T* data;
			long capacity;
			long size = 0;

			void resize(long new_capacity) {
				if (!data) {
					std::cerr << "Error in " << __func__ << " in file " << __FILE__
						<< " at line " << __LINE__ << ": Attempted resize on nullptr data.\n";
					return;  // Don't proceed if data is nullptr
				}

				if (new_capacity <= capacity) return;
				T* new_data = new T[new_capacity];
				for (long i = 0; i < size; ++i) {
					new_data[i] = data[i];
				}

				delete[] data;  // Properly delete the old array
				data = new_data;  // Point to the newly allocated memory
				capacity = new_capacity;
			}
	};
}
