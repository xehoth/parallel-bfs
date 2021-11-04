//
// Created by xehoth on 2021/11/4.
//
#include <serial_bfs.h>
#include <queue>
#include <iostream>
void SerialBfs::bfs(int s) {
  int **g = graph.g;
  const int n = graph.nVertices;
  const int m = graph.nEdges;
  std::vector<int> q(n + 1);
  int l = 0, r = 0;
  dist[s] = 0;
  q[r++] = s;
  for (int u; l != r;) {
    u = q[l++];
    for (int i = 0; i < graph.deg(u); ++i) {
      int v = g[u][i];
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        parent[v] = u;
        q[r++] = v;
      }
    }
  }
//  dist[s] = 0;
//  q.push(s);
//  for (int u; !q.empty();) {
//    u = q.front();
//    q.pop();
//    for (int i = 0; i < graph.deg(u); ++i) {
//      int v = g[u][i];
//      if (dist[v] == -1) {
//        dist[v] = dist[u] + 1;
//        parent[v] = u;
//        q.push(v);
//      }
//    }
//  }
}
