#pragma once

#include <memory>

// in-case i'd ever want to make my own smart ptr thing or smth
// with like ref count
// for debugging
template <typename T> using UniquePtr = std::unique_ptr<T>;

template <typename T> using SharedPtr = std::shared_ptr<T>;

template <typename T> using WeakPtr = std::weak_ptr<T>;
