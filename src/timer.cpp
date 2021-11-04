//
// Created by xehoth on 2021/11/4.
//
#include <timer.h>

void Timer::start() { point = std::chrono::high_resolution_clock::now(); }

void Timer::end() {
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time = now - point;
  times.push_back(time.count() / 1000.0);
}

void Timer::clear() { times.clear(); }

double Timer::average() const {
  return std::accumulate(times.begin(), times.end(), 0.0) /
         static_cast<double>(times.size());
}

double Timer::report(int edges) const { return edges / 1e6 / average(); }
