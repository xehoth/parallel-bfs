//
// Created by xehoth on 2021/11/4.
//

#ifndef CS121_LAB_INCLUDE_GRAPH_BFS_H_
#define CS121_LAB_INCLUDE_GRAPH_BFS_H_
#include <graph.h>

struct Bfs {
  explicit Bfs(const Graph &graph) : graph(graph) { init(); }

  virtual void bfs(int s) = 0;
  virtual ~Bfs() = default;
  int *dist{}, *parent{};
  std::unique_ptr<int[]> distBuffer;
  std::unique_ptr<int[]> parentBuffer;

  void init() {
    const int n = graph.nVertices;
    if (!dist) {
      distBuffer = std::move(std::make_unique<int[]>(n));
      parentBuffer = std::move(std::make_unique<int[]>(n));
      dist = distBuffer.get();
      parent = parentBuffer.get();
    }
    for (int i = 0; i < n; ++i) parent[i] = -1;
    for (int i = 0; i < n; ++i) dist[i] = -1;
  }

  void write(const std::filesystem::path &path) const;

  const Graph &graph;
};
#endif  // CS121_LAB_INCLUDE_GRAPH_BFS_H_
