//
// Created by xehoth on 2021/11/4.
//
#ifndef BFS_H_
#define BFS_H_
#include <graph.h>
#include <cstring>

struct Bfs : public Graph {
  using Graph::Graph;

  virtual void init() {
    for (std::uint32_t i = 0; i < this->n; ++i) dist[i] = -1u;
    for (std::uint32_t i = 0; i < this->n; ++i) parent[i] = -1u;
  }

  virtual void bfs(std::uint32_t s) = 0;
};
#endif