#ifndef SCOPEDARRAY_H
#define SCOPEDARRAY_H

#include <memory>

template<typename T> class ScopedArray {
public:
  typedef T val_type;
  ~ScopedArray();
  ScopedArray(val_type array_[] = nullptr);
  ScopedArray(const ScopedArray &src);
  // ScopedArray<T>& operator=(const ScopedArray &src);
  operator bool() const {
    return array.get() != nullptr;
  }
  val_type& operator[](size_t index);
  const val_type& operator[](size_t index) const;
  val_type *get() const;
  void swap(ScopedArray& rhs) noexcept;
  val_type *release();
  void reset(val_type* array = nullptr);

private:
  std::unique_ptr<val_type[]> array;
};
#include "../src/ScopedArray.cpp"
#endif //SCOPEDARRAY_H