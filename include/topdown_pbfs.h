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
    if (!f) f = std::make_unique<Frontier>(this->m);
    if (!nf) nf = std::make_unique<Frontier>(this->m);
    if (!b) b = std::make_unique<BitMap32>(this->n);
    if (!pi) pi = MemoryManager::get().alloc<std::uint32_t>(this->n + 1);
    if (!cb) cb = MemoryManager::get().alloc<std::uint32_t>(this->m + 1);
    f->clear();
    nf->clear();
    b->clear();
    pi[0] = 0;
  }

  void bfs(std::uint32_t s) override {
    if (s >= this->n) return;
    dist[s] = 0;
    parent[s] = s;
    f->d[0] = s;
    f->n = 1;
    // f->push(s);
    // b->set(s);
    while (!f->empty()) {
#pragma omp parallel for
      for (std::uint32_t i = 0; i < f->n; ++i) pi[i + 1] = this->deg(f->d[i]);

      prefixSum(pi, f->n + 1);
      nf->n = pi[f->n];
#pragma omp parallel for schedule(guided)
      for (std::uint32_t i = 0; i < f->n; ++i) {
        std::uint32_t u = f->d[i], offset = pi[i];
        for (std::uint32_t j = 0; j < this->deg(u); ++j) {
          std::uint32_t v = this->g[this->o[u] + j];
          // if (!b->testAndSet(v) /*dist[v] == -1u*/) {
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
      printf("fn: %d\n", f->n);
    }
  }

  std::unique_ptr<Frontier> f{}, nf{};  // frontier and next frontier
  std::unique_ptr<BitMap32> b{}, nb{};
  std::uint32_t *pi{};  // index: prefix sum (degree)
  std::uint32_t *cb{};  // cull index buffer
};
#endif