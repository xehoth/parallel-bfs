//
// Created by xehoth on 2021/11/4.
//

#ifndef CS121_LAB_INCLUDE_TIMER_H_
#define CS121_LAB_INCLUDE_TIMER_H_
#include <vector>
#include <chrono>
#include <numeric>

class Timer {
 public:
  void start();
  void end();
  void clear();
  [[nodiscard]] double average() const;
  [[nodiscard]] double report(int edges) const;

 private:
  std::vector<double> times;
  std::chrono::high_resolution_clock::time_point point;
};
#endif  // CS121_LAB_INCLUDE_TIMER_H_
