//
// Created by xehoth on 2021/11/4.
//
#ifndef TIMER_H_
#define TIMER_H_
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <fstream>
#include <bfs.h>

class Timer {
 public:
  void start() { point = std::chrono::high_resolution_clock::now(); }
  void end() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = now - point;
    times.push_back(time.count() / 1000.0);
  }
  void clear() { times.clear(); }
  double average() const {
    return std::accumulate(times.begin(), times.end(), 0.0) /
           static_cast<double>(times.size());
  }
  double stddev() const {
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double avg = sum / static_cast<double>(times.size());
    double ret = 0;
    for (auto &v : times) ret += (v - avg) * (v - avg);
    ret = std::sqrt(ret / static_cast<double>(times.size()));
    return ret;
  }
  void report(std::uint32_t edges) const {
    double avg = average();
    double mteps = edges / 1e6 / avg;
    std::clog << mteps << "MTEPS, stddev: " << mteps * stddev()
              << ", time: " << avg << std::endl;
  }

  void benchOnce(Bfs *g, std::uint32_t s) {
    g->init();
    start();
    g->bfs(s);
    end();
  }

  void write(std::uint32_t edges, const std::string &output) {
    std::clog << "write benchmark to " << output << " ... ";
    std::ofstream out(output);
    double avg = average();
    double mteps = edges / 1e6 / avg;
    out << mteps << std::endl
        << mteps * stddev() << std::endl
        << avg << std::endl;
    std::clog << "done" << std::endl;
  }

 private:
  std::vector<double> times;
  std::chrono::high_resolution_clock::time_point point;
};
#endif