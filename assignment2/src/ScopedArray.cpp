#ifndef SCOPEDARRAY_CPP
#define SCOPEDARRAY_CPP

#include "ScopedArray.h"

template<typename T> ScopedArray<T>::ScopedArray(T array_[]) : array(array_)
{}

template <typename T> ScopedArray<T>::~ScopedArray()
{}

template<typename T> T *ScopedArray<T>::get() const
{
    T *result = array.get();
    return result;
}

template<typename T> void ScopedArray<T>::swap(ScopedArray& rhs)
{
    std::swap(array, rhs.array);
}

template<typename T> T *ScopedArray<T>::release()
{
    return array.release();
}

template<typename T> void ScopedArray<T>::reset(val_type* array_)
{
    if(array.get() != array_)
        array.reset(array_);
}

template <typename T> T& ScopedArray<T>::operator[](size_t index) {
    return array[index];
}
template <typename T> const T& ScopedArray<T>::operator[](size_t index) const {
    return array[index];
}

#endif //SCOPEDARRAY_CPP