//
// Created by xehoth on 2021/11/4.
//
#ifndef GRAPH_H_
#define GRAPH_H_
#include <reader.h>
#include <writer.h>
#include <memory_manager.h>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>

inline int loadEdgesFromSnap(
    const std::string &path,
    std::vector<std::pair<std::uint32_t, std::uint32_t>> &edges) {
  Reader reader(path);
  std::uint32_t max = 0;
  for (std::uint32_t u, v; reader >> u >> v, !reader.isEof();) {
    max = std::max(std::max(max, u), v);
    edges.emplace_back(u, v);
  }
  return max;
}

inline int loadEdgesFromMatrixMarket(
    const std::string &path,
    std::vector<std::pair<std::uint32_t, std::uint32_t>> &edges) {
  Reader reader(path);
  std::uint32_t n, m, l;
  reader >> n >> m >> l;
  std::uint32_t max = 0;
  for (std::uint32_t u, v, _; reader >> u >> v >> _, !reader.isEof();) {
    max = std::max(std::max(max, u), v);
    edges.emplace_back(u, v);
  }
  return max;
}

struct Graph {
  Graph(const std::string &path, std::string format = "SNAP") {
    std::clog << "load graph from: " << path << " ... ";
    std::vector<std::pair<std::uint32_t, std::uint32_t>> edges;
    int max = 0;
    if (format == "SNAP") {
      max = loadEdgesFromSnap(path, edges);
    } else if (format == "MatrixMarket") {
      max = loadEdgesFromMatrixMarket(path, edges);
    } else if (format == "RMAT") {
      edges.reserve(1000000000);
      max = loadEdgesFromSnap(path, edges);
    } else {
      std::cerr << "invalid format type!" << std::endl;
      std::exit(0);
    }

    edges.shrink_to_fit();

    std::clog << "done" << std::endl;
    std::clog << "vertices: " << max << ", edges: " << edges.size()
              << std::endl;
    std::clog << "construct mapping and edges ... ";

    constructMapping(edges, max);

    std::vector<int> deg(this->n);
    for (auto &[u, v] : edges) ++deg[o2c[u]];

    this->m = edges.size();

    this->g = MemoryManager::get().alloc<std::uint32_t>(this->m);
    this->o = MemoryManager::get().alloc<std::uint32_t>(this->n + 1);

    this->o[0] = 0;
    for (std::uint32_t i = 0; i < this->n; ++i) {
      this->o[i + 1] = this->o[i] + deg[i];
    }

    for (std::uint32_t i = 0; i < this->n; ++i) deg[i] = 0;
    for (auto &[u, v] : edges)
      this->g[this->o[o2c[u]] + deg[o2c[u]]++] = o2c[v];

    std::clog << "done" << std::endl;

    std::clog << "sorting adjacent list ... ";
    for (std::uint32_t i = 0; i < this->n; ++i) {
      std::sort(this->g + this->o[i], this->g + this->o[i + 1]);
    }

    this->dist = MemoryManager::get().alloc<std::uint32_t>(this->n);
    this->parent = MemoryManager::get().alloc<std::uint32_t>(this->n);

    std::clog << "done" << std::endl;
    std::clog << "after mapping, vertices: " << n << ", edges: " << m
              << std::endl;
  }

  void constructMapping(
      const std::vector<std::pair<std::uint32_t, std::uint32_t>> &edges,
      std::uint32_t max) {
    o2c.resize(max + 1, -1u);

    this->n = 0;
    for (auto &[u, v] : edges) {
      if (o2c[u] == -1u) {
        o2c[u] = this->n++;
        c2o.push_back(u);
      }
      if (o2c[v] == -1u) {
        o2c[v] = this->n++;
        c2o.push_back(v);
      }
    }
    c2o.shrink_to_fit();
  }

  void write(const std::string &path) {
    std::clog << "write output to: " << path << " ... ";
    Writer writer(path);
    std::vector<std::pair<int, int>> output(o2c.size());
    for (std::uint32_t i = 0; i < this->n; ++i) {
      output[c2o[i]] = std::make_pair<int, int>(
          static_cast<int>(dist[i]),
          parent[i] != -1u ? static_cast<int>(c2o[parent[i]]) : -1);
    }
    for (size_t i = 0; i < output.size(); ++i) {
      if (o2c[i] != -1u && output[i].second != -1) {
        writer << static_cast<int>(i) << ' ' << output[i].first << ' '
               << output[i].second << '\n';
      }
    }
    std::clog << "done" << std::endl;
  }

  inline std::uint32_t deg(std::uint32_t u) const {
    return this->o[u + 1] - this->o[u];
  }

  virtual ~Graph() = default;

  std::uint32_t *g;       // vertices data
  std::uint32_t *o;       // offset index
  std::uint32_t n;        // vertices
  std::uint32_t m;        // edges
  std::uint32_t *dist;    // dist
  std::uint32_t *parent;  // parent

  std::vector<std::uint32_t> c2o;  // map current id (0 ~ n - 1) to original one
  std::vector<std::uint32_t> o2c;  // map original id to current id (0 ~ n - 1)
};

#endif