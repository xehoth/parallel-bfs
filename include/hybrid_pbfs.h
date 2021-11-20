//
// Created by xehoth on 2021/11/20.
//
#ifndef HYBRID_PBFS_H_
#define HYBRID_PBFS_H_
#include <bfs.h>
#include <frontier.h>
#include <bitmap.h>
#include <prefix_sum.h>
#include <memory>

struct HybridBfs : public Bfs {
  using Bfs::Bfs;

  void init() override {
    Bfs::init();
    if (!f) f = std::make_unique<Frontier>(this->m);
    if (!nf) nf = std::make_unique<Frontier>(this->m);
    if (!b) b = std::make_unique<BitMap64>(this->n);
    if (!nb) nb = std::make_unique<BitMap64>(this->n);
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
    f->push(s);
    b->set(s);
    // current step is top-down / bottom-up
    bool topdownStep = true;

    // the number of edges to check from the frontier
    std::uint32_t m_f = this->deg(s);
    // the number of vertices in the frontier
    std::uint32_t n_f = f->n;
    // the number of edges to check from unexplored vertices
    std::uint32_t mu = this->m;
    const std::uint32_t alpha = 14;
    const std::uint32_t beta = 24;

    auto doTopDown = [&]() {
#pragma omp parallel for
      for (std::uint32_t i = 0; i < f->n; ++i) pi[i + 1] = this->deg(f->d[i]);

      prefixSumOmp(pi, f->n + 1);
      nf->n = pi[f->n];
#pragma omp parallel for schedule(guided) reduction(+:m_f, n_f) reduction(-:mu)
      for (std::uint32_t i = 0; i < f->n; ++i) {
        std::uint32_t u = f->d[i], offset = pi[i];
        for (std::uint32_t j = 0; j < this->deg(u); ++j) {
          std::uint32_t v = this->g[this->o[u] + j];
          if (!b->test(v) && dist[v] == -1u) {
            b->set(v);
            nf->d[offset + j] = v;
            dist[v] = dist[u] + 1;
            parent[v] = u;
            m_f += this->deg(v);
            mu -= this->deg(v);
            ++n_f;
          } else {
            nf->d[offset + j] = -1u;
          }
        }
      }
      f->cullFrom(*nf, cb);
    };

    auto doBottomUp = [&]() {
      nb->clear();
#pragma omp parallel for schedule(guided) reduction(+:m_f, n_f) reduction(-:mu)
      for (std::uint32_t v = 0; v < this->n; ++v) {
        if (!b->test(v)) {
          for (std::uint32_t i = this->o[v]; i < this->o[v + 1]; ++i) {
            std::uint32_t u = this->g[i];
            if (b->test(u)) {
              nb->set(v);
              dist[v] = dist[u] + 1;
              m_f += this->deg(v);
              mu -= this->deg(v);
              ++n_f;
              break;
            }
          }
        }
      }
      *b |= *nb;
    };

    while (!(topdownStep && f->empty())) {
      if (true) {
        doTopDown();
      } else {
        doBottomUp();
      }
      if (topdownStep) {
        if (m_f > mu / alpha) topdownStep = false;
      } else {
        if (n_f < this->n / beta) {
          topdownStep = true;
          f->clear();
          for (std::uint32_t i = 0; i < nb->capacity(); ++i)
            if (nb->test(i)) f->push(i);
        }
        topdownStep = true;
      }
      m_f = n_f = 0;
    }
  }

  std::unique_ptr<Frontier> f{}, nf{};  // frontier and next frontier
  std::unique_ptr<BitMap64> b{}, nb{};
  std::uint32_t *pi{};  // index: prefix sum (degree)
  std::uint32_t *cb{};  // cull index buffer
};
#endif