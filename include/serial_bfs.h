//
// Created by xehoth on 2021/11/4.
//
#ifndef SERIAL_BFS_H_
#define SERIAL_BFS_H_
#include <bfs.h>
#include <queue>

struct SerialBfs : public Bfs {
  using Bfs::Bfs;

  void bfs(std::uint32_t s) override {
    std::queue<std::uint32_t> q;
    dist[s] = 0;
    q.push(s);
    for (std::uint32_t u; !q.empty();) {
      u = q.front();
      q.pop();
      for (std::uint32_t j = this->o[u]; j < this->o[u + 1]; ++j) {
        std::uint32_t v = this->g[j];
        if (dist[v] == -1u) {
          dist[v] = dist[u] + 1;
          parent[v] = u;
          q.push(v);
        }
      }
    }
  }
};
#endif