//
// Created by xehoth on 2021/11/5.
//
#ifndef TOPDOWN_PBFS_H_
#define TOPDOWN_PBFS_H_
#include <bfs.h>
#include <frontier.h>
#include <bitmap.h>
#include <prefix_sum.h>
#include <memory>

struct TopdownPbfs : public Bfs {
  using Bfs::Bfs;

  void init() override {
    Bfs::init();
    if (!f) f = std::move(std::make_unique<Frontier>(this->m));
    if (!nf) nf = std::move(std::make_unique<Frontier>(this->m));
    if (!b) b = std::move(std::make_unique<BitMap64>(this->n));
    if (!pi) pi = MemoryManager::get().alloc<std::uint32_t>(this->m + 1);
    if (!cb) cb = MemoryManager::get().alloc<std::uint32_t>(this->m + 1);
    f->clear();
    nf->clear();
    b->clear();
    pi[0] = 0;
  }

  void bfs(std::uint32_t s) override {
    dist[s] = 0;
    f->push(s);
    b->set(s);
    while (!f->empty()) {
#pragma omp parallel for
      for (std::uint32_t i = 0; i < f->n; ++i) pi[i + 1] = this->deg(f->d[i]);

      prefixSum(pi, f->n + 1);
      nf->n = pi[f->n];
#pragma omp parallel for schedule(guided)
      for (std::uint32_t i = 0; i < f->n; ++i) {
        const std::uint32_t u = f->d[i], offset = pi[i];
        for (std::uint32_t j = 0; j < this->deg(u); ++j) {
          const std::uint32_t v = this->g[this->o[u] + j];
          // if (!b->testAndSet(v)) {
          if (!b->test(v) && dist[v] == -1u) {
            b->set(v);
            nf->d[offset + j] = v;
            dist[v] = dist[u] + 1;
            parent[v] = u;
          } else {
            nf->d[offset + j] = -1u;
          }
        }
      }

      f->cullFrom(*nf, cb);
    }
  }

  std::unique_ptr<Frontier> f{}, nf{};  // frontier and next frontier
  std::unique_ptr<BitMap64> b{}, nb{};
  std::uint32_t *pi{};  // index: prefix sum (degree)
  std::uint32_t *cb{};  // cull index buffer
};
#endif