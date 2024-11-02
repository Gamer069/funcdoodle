#pragma once

#include <execinfo.h>

namespace FuncDoodle {
    template <typename T>
        class LongIndexArray {
            public:
                LongIndexArray(long initial_capacity = 10)
                    : capacity(initial_capacity), size(0), data(nullptr) {
                        if (capacity < 1) capacity = 1;
                        data = new T[capacity];
                    }
                ~LongIndexArray() {
                }

                void push_back(const T& value) {
                    if (size == capacity) {
                        resize(capacity * 2);
                    }
                    data[size++] = value;
                }
                void insertAfter(long index, const T& value) {
                    if (index < 0 || index >= size) {
                        std::cout << "insert after" << std::endl;
                        throw std::out_of_range("Index out of range");
                    }

                    if (size == capacity) {
                        resize(capacity * 2);
                    }

                    // Shift elements to the right, starting from the end
                    for (long i = size; i > index + 1; --i) {
                        data[i] = data[i-1];
                    }

                    // Insert the new value after the specified index
                    data[index + 1] = value;
                    ++size;
                }
                void insertBefore(long index, const T& value) {
                    if (index < 0 || index >= size) {
                        std::cout << "insert before" << std::endl;
                        throw std::out_of_range("Index out of range");
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
                T& operator[](long index) {
                    if (index < 0 || index >= size) {
                        std::cout << "[]" << std::endl;
                        throw std::out_of_range("Index out of range");
                    }
                    return data[index];
                }

                const T& operator[](long index) const {
                    if (index < 0 || index >= size) {
                        std::cout << "const[]" << std::endl;
                        throw std::out_of_range("Index out of range");
                    }
                    return data[index];
                }
                T& get(long index) {
                    if (index < 0 || index >= size) {
                        std::cout << "get (DynArr)" << std::endl;
                        void* arr[10];
                        size_t size = backtrace(arr, 10);
                        backtrace_symbols_fd(arr, size, 2);
                        std::cout << index << std::endl;
                        throw std::out_of_range("Index out of range");
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
                    if (new_capacity <= capacity) return;
                    T* new_data = new T[new_capacity];
                    for (long i = 0; i < size; ++i) {
                        new_data[i] = data[i];
                    }
                    //delete[] data;
                    data = new_data;
                    capacity = new_capacity;
                }
        };
}
