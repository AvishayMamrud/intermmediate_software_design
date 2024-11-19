//
// Created by avish on 11/11/2024.
//
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#ifndef ARRAY_LIST_CPP
#define ARRAY_LIST_CPP
#include "ArrayList.h"
#include "ArrayListIter.h"

/**
 * Creates an ArrayList of size 0.
 */
template <typename T> ArrayList<T>::ArrayList(): mArray(nullptr), mSize(0), mCapacity(0)
{}

/**
 * Creates an ArrayList of the provided size and fills it with the provided
 * value - default to the default value of the template type.
 * @param size size of the ArrayList to create
 * @param value value used to fill the ArrayList
 */
template <typename T> ArrayList<T>::ArrayList(const uint32_t& size, const T& value)
    : mArray(new T[size]), mSize(size), mCapacity(size)
{
    std::uninitialized_fill_n(mArray.get(), mSize, value);
}

/**
 * Creates a deep copy of the provided ArrayList
 * @param src ArrayList to copy
 */
template <typename T> ArrayList<T>::ArrayList(const ArrayList<T>& src)
    : mArray(new T[src.mCapacity]), mSize(src.mSize), mCapacity(src.mCapacity)
{
    std::copy(src.mArray.get(), src.mArray.get() + mSize, mArray.get());
}

/**
 * Performs move constructor semantics on the provided ArrayList
 * @param src ArrayList to move
 */
template <typename T> ArrayList<T>::ArrayList(ArrayList<T>&& src) noexcept
: mArray(src.mArray.release()), mSize(src.mSize), mCapacity(src.mCapacity)
{
    src.mSize = src.mCapacity = 0;
}

/**
 * Makes *this a deep copy of the provided ArrayList.
 * @param src ArrayList to copy
 * @return *this for chaining
 */
template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& src)
{
    ArrayList<T>(src).swap(*this);
    return *this;
}

/**
 * Performs move assignment semantics on the provided ArrayList.
 * @param src ArrayList to move
 * @return *this for chaining
 */
template <typename T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& src) noexcept
{
    mSize = src.mSize;
    mCapacity = src.mCapacity;
    mArray.reset(src.mArray.release());
    src.mSize = src.mCapacity = 0;
    return *this;
}

/**
 * Adds the provided element to the end of this ArrayList.  If the
 * ArrayList needs to be enlarged, double the capacity from the
 * current capacity, or go from zero to one.
 * @param value value to add
 * @return total array capacity
 */
template <typename T>
const uint32_t& ArrayList<T>::add(const T& value)
{
    return ArrayList<T>::add(mSize, value);
}

/**
 * Inserts the specified value into this ArrayList at the specified index.
 * The object is inserted before any previous element at the specified
 * location. If the ArrayList needs to be enlarged, continue doubling the capacity
 * from the current capacity until the desired index is in range.  Fill any empty
 * elements up to the new element's index with the default value for the template type.
 * @param index location at which to insert the new element
 * @param value the element to insert
 * @return total array capacity
 */
template <typename T>
const uint32_t& ArrayList<T>::add(const uint32_t& index, const T& value)
{
    uint32_t cap = std::max(1U, mCapacity);
    while(index >= cap || mSize >= cap)
        cap *= 2;
    std::unique_ptr<T[]> arr(new T[cap]);// T[cap];
    std::copy(mArray.get(), mArray.get() + std::min(index, mSize), arr.get());
    if(index > mSize)
        std::uninitialized_fill_n(arr.get() + mSize, index - mSize, T());
    std::copy(mArray.get() + index, mArray.get() + mSize, arr.get() + index + 1);
    arr[index] = value;
    mArray.reset(arr.release());
    mCapacity = cap;
    mSize = 1 + std::max(index, mSize);
    return mCapacity;
}

/**
 * Clears this ArrayList, leaving it empty.
 */
template <typename T>
void ArrayList<T>::clear()
{
    mArray.reset();
    mSize = mCapacity = 0;
}

/**
 * Returns a const T & to the element stored at the specified index.
 * If the index is out of bounds, std::out_of_range is thrown with the index
 * as its message.
 * @param index the desired location
 * @return a const T & to the desired element.
 */
template <typename T>
const T& ArrayList<T>::get(const uint32_t& index) const
{
    checkRange(index);
    return mArray[index];
}

/**
 * Returns a T & to the element stored at the specified index.
 * If the index is out of bounds, std::out_of_range is thrown with the index
 * as its message.
 * @param index the desired location
 * @return a T & to the desired element.
 */
template <typename T>
T& ArrayList<T>::get(const uint32_t& index)
{
    checkRange(index);
    return mArray[index];
}

/**
 * Returns a T & to the element stored at the specified index.
 * No range checking is performed.
 * Must call to the const operator[] - do not reimplement logic!
 * @param index the desired location
 * @return a T & to the desired element.
 */
template <typename T>
T& ArrayList<T>::operator[](const uint32_t& index)
{
    return mArray[index];
}

/**
 * Returns a const T & to the element stored at the specified index.
 * No range checking is performed.
 * @param index the desired location
 * @return a const T & to the desired element.
 */
template <typename T>
const T& ArrayList<T>::operator[](const uint32_t& index) const
{
    return mArray[index];
}

/**
 * Empty check.
 * @return True if this ArrayList is empty and false otherwise.
 */
template <typename T>
[[nodiscard]] bool ArrayList<T>::isEmpty() const
{
    return mSize == 0;
}

/**
     * Returns iterator to the beginning; in this case, a random access iterator
     * @return an iterator to the beginning of this ArrayList.
     */
template <typename T>
ArrayListIterator<T> ArrayList<T>::begin()
{
    return ArrayListIterator<T>(mArray.get());
}

/**
 * Returns the past-the-end iterator of this ArrayList.
 * @return a past-the-end iterator of this ArrayList.
 */
template <typename T>
ArrayListIterator<T> ArrayList<T>::end()
{
    return ArrayListIterator<T>(mArray.get() + mSize);
}

/**
 * Removes an element at the specified location from this ArrayList and
 * returns it. Elements following index are shifted down. If index is out of
 * range, std::out_of_range is thrown with index as its message.
 * @param index the desired location
 * @return a copy of the removed element.
 */
template <typename T>
T ArrayList<T>::remove(const uint32_t& index)
{
    checkRange(index);
    T value = std::move(mArray[index]);
    std::copy(mArray.get() + index + 1, mArray.get() + mSize, mArray.get() + index);
    mSize--;
    return value;
}

/**
 * Sets the element at the desired location to the specified value. If index
 * is out of range, std::out_of_range is thrown with index as its message.
 * @param index the location to change
 * @param value the new value of the specified element.
 */
template <typename T>
void ArrayList<T>::set(const uint32_t& index, const T& value)
{
    checkRange(index);
    mArray[index] = value;
}

/**
 * Returns the size of this ArrayList.
 * @return the size of this ArrayList.
 */
template <typename T>
[[nodiscard]] uint32_t ArrayList<T>::size() const
{
    return mSize;
}

/**
 * Perform an exception-safe swap of the contents of *this with
 * src.
 */
template <typename T>
void ArrayList<T>::swap(ArrayList<T> &src) noexcept
{
    std::swap(mSize, src.mSize);
    std::swap(mCapacity, src.mCapacity);
    mArray.swap(src.mArray);
}

template <typename T> void ArrayList<T>::checkRange(const uint32_t& index) const
{
    if(index >= mSize) throw std::out_of_range(std::to_string(index));
}


#endif //ARRAY_LIST_CPP