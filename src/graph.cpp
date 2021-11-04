//
// Created by xehoth on 2021/11/4.
//
#include <graph.h>
#include <reader.h>
#include <iostream>

void Graph::load(const std::filesystem::path &path) {
  Reader reader(path);
  std::vector<std::pair<int, int>> edges;
  int max = std::numeric_limits<int>::min();
  while (!reader.isEof()) {
    int u, v;
    reader >> u >> v;
    max = std::max({max, u, v});
    edges.emplace_back(u, v);
  }
  edges.shrink_to_fit();
  originToTrans.assign(max + 1, -1);
  transToOrigin.clear();
  this->nVertices = 0;
  for (auto &[u, v] : edges) {
    if (originToTrans[u] == -1) {
      originToTrans[u] = this->nVertices++;
      transToOrigin.push_back(u);
    }
    if (originToTrans[v] == -1) {
      originToTrans[v] = this->nVertices++;
      transToOrigin.push_back(v);
    }
  }
  transToOrigin.shrink_to_fit();

  std::vector<int> deg(this->nVertices);
  for (auto &[u, v] : edges) ++deg[originToTrans[u]];

  this->nEdges = static_cast<int>(edges.size());
  this->gBuffer = std::move(std::make_unique<int *[]>(this->nVertices + 1));
  this->vBuffer = std::move(std::make_unique<int[]>(nEdges));
  this->g = gBuffer.get();
  this->g[0] = this->vBuffer.get();
  for (int i = 1; i <= this->nVertices; ++i) {
    this->g[i] = this->g[i - 1] + deg[i - 1];
  }
  for (int i = 0; i < this->nVertices; ++i) deg[i] = 0;
  for (auto &[u, v] : edges) {
    this->g[originToTrans[u]][deg[originToTrans[u]]++] = originToTrans[v];
  }
}
