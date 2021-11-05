//
// Created by xehoth on 2021/11/4.
//
#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_
#include <vector>
#include <cstdlib>
#include <iostream>

class MemoryManager {
 public:
  static MemoryManager &get() { return _instance; }

  template <typename T>
  T *alloc(size_t cnt) {
    T *ret = reinterpret_cast<T *>(std::malloc(cnt * sizeof(T)));
    mem.emplace_back(ret);
    return ret;
  }

 private:
  MemoryManager() = default;
  ~MemoryManager() {
    for (auto &i : mem) std::free(i);
    std::clog << "destroy memory manager, all memory released" << std::endl;
  }
  static MemoryManager _instance;
  std::vector<void *> mem;
};

MemoryManager MemoryManager::_instance;
#endif