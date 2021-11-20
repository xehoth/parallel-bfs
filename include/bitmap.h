//
// Created by xehoth on 2021/11/5.
//
#ifndef BITMAP_H_
#define BITMAP_H_
#include <memory_manager.h>
#include <cstdint>

template <typename T>
class BitMap {
 public:
  BitMap(std::uint32_t n)
      : n(n), d(MemoryManager::get().alloc<T>(capacity())) {}

  inline std::uint32_t capacity() const {
    return this->n / SIZE + (this->n % SIZE != 0);
  }

  inline bool testAndSet(std::uint32_t i) {
    const std::uint32_t index = i / SIZE;
    const std::uint32_t offset = i % SIZE;
    bool ret = d[index] >> offset & 1;
    if (!ret) {
      ret = __atomic_fetch_or(&d[index], T{1} << offset, __ATOMIC_SEQ_CST) >>
                offset &
            1;
    }
    return ret;
  }

  inline bool test(std::uint32_t i) {
    const std::uint32_t index = i / SIZE;
    const std::uint32_t offset = i % SIZE;
    return d[index] >> offset & 1;
  }

  inline void set(std::uint32_t i) {
    const std::uint32_t index = i / SIZE;
    const std::uint32_t offset = i % SIZE;
    d[index] |= T{1} << offset;
  }

  void clear() {
    for (std::uint32_t i = 0; i < this->capacity(); ++i) this->d[i] = 0;
  }

  BitMap &operator|=(const BitMap &rhs) {
    for (std::uint32_t i = 0; i < this->capacity(); ++i) this->d[i] |= rhs.d[i];
    return *this;
  }

 private:
  static constexpr std::uint32_t SIZE = sizeof(T) * 8;
  std::uint32_t n;
  T *d;
};

using BitMap32 = BitMap<std::uint32_t>;
using BitMap64 = BitMap<std::uint64_t>;
#endif