//
// Created by xehoth on 2021/11/5.
//
#ifndef PREFIX_SUM_H_
#define PREFIX_SUM_H_
#include <memory_manager.h>
#include <omp.h>

static const int MAX_THREADS = 64;

template <typename T>
inline void prefixSum(T *a, std::uint32_t n) {
  for (std::uint32_t i = 1; i < n; ++i) a[i] += a[i - 1];
}

template <typename T>
inline void prefixSumOmp(T *a, std::uint32_t n) {
  static T *buffer{};
  if (!buffer) buffer = MemoryManager::get().alloc<T>(MAX_THREADS + 1);
  buffer[0] = 0;
#pragma omp parallel
  {
    const int id = omp_get_thread_num();
    T s = 0;
#pragma omp for schedule(static) nowait
    for (std::uint32_t i = 0; i < n; ++i) {
      s += a[i];
      a[i] = s;
    }
    buffer[id + 1] = s;

#pragma omp barrier
    T offset = 0;
    for (int i = 1; i < id + 1; ++i) offset += buffer[i];

#pragma omp for schedule(static)
    for (std::uint32_t i = 0; i < n; ++i) a[i] += offset;
  }
}
#endif