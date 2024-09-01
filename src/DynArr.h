#pragma once


namespace FuncDoodle {
    template <typename T>
        class LongIndexArray {
            public:
                LongIndexArray(long initial_capacity = 10)
                    : capacity(initial_capacity), size(0) {
                        if (capacity < 1) capacity = 1;
                        data = new T[capacity];
                    }
                ~LongIndexArray() {
                    //std::cout << "Error??" << std::endl;
                    //delete[] data;
                    //std::cout << "NVM" << std::endl;
                }

                void push_back(const T& value) {
                    if (size == capacity) {
                        resize(capacity * 2);
                    }
                    data[size++] = value;
                }
                void insertAfter(long index, const T& value) {
                    if (index < 0 || index >= size) {
                        throw std::out_of_range("Index out of range");
                    }

                    std::cout << "Before: " << sizeof(data) / sizeof(Frame) << std::endl;

                    if (size == capacity) {
                        resize(capacity * 2);
                    }

                    if (size + 1 == index) {
                        data[index+1] = value;
                        std::cout << "After: " << sizeof(data) / sizeof(Frame) << std::endl;
                    }
                    ++size;
                }   
                void insertBefore(long index, const T& value) {
                    if (index < 0 || index >= size) {
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
                        throw std::out_of_range("Index out of range");
                    }
                    return data[index];
                }

                const T& operator[](long index) const {
                    if (index < 0 || index >= size) {
                        throw std::out_of_range("Index out of range");
                    }
                    return data[index];
                }
                T& get(long index) {
                    if (index < 0 || index >= size) {
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
                long size;
                void resize(long new_capacity) {
                    std::cout << "Before: resize: " << getSize() << std::endl;
                    if (new_capacity <= capacity) return;
                    T* new_data = new T[new_capacity];
                    for (long i = 0; i < size; ++i) {
                        new_data[i] = data[i];
                    }
                    delete[] data;
                    data = new_data;
                    capacity = new_capacity;
                    std::cout << "After: resize: " << getSize() << std::endl;
                }
        };
}
