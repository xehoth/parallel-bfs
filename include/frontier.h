//
// Created by xehoth on 2021/11/5.
//
#ifndef FRONTIER_H_
#define FRONTIER_H_
#include <memory_manager.h>
#include <cstdint>
#include <memory>
#include <prefix_sum.h>

struct Frontier {
  Frontier(std::uint32_t maxCapacity)
      : d(MemoryManager::get().alloc<std::uint32_t>(maxCapacity)), n() {}

  inline bool empty() const { return n == 0; }
  inline void push(std::uint32_t v) { d[n++] = v; }
  inline void clear() { this->n = 0; }

  void cullFrom(const Frontier &f, std::uint32_t *buf) {
    buf[0] = 0;

#pragma omp parallel for
    for (std::uint32_t i = 0; i < f.n; ++i) buf[i + 1] = f.d[i] != -1u;

    prefixSumOmp(buf, f.n + 1);
    // prefixSum(buf, f.n + 1);
    this->n = buf[f.n];

#pragma omp parallel for
    for (std::uint32_t i = 0; i < f.n; ++i)
      if (f.d[i] != -1u) this->d[buf[i]] = f.d[i];
  }

  std::uint32_t *d;  // data
  std::uint32_t n;   // current size
};
#endif