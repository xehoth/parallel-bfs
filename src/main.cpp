//
// Created by xehoth on 2021/11/4.
//
#include <graph.h>
#include <serial_bfs.h>
#include <chrono>
#include <iostream>

int main() {
  Graph g;
  g.load("../../../data/soc-LiveJournal1.txt");
  std::cout << "load done" << std::endl;
  std::cout << "vertices: " << g.nVertices << ", "
            << "edges: " << g.nEdges << std::endl;
  SerialBfs sbfs(g);
  sbfs.init();
  std::cout << "start bfs" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  sbfs.bfs(g.originToTrans[1]);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time = end - start;
  sbfs.write("out.txt");
  std::cout << time.count() / 1000.0 << "s" << std::endl;
  return 0;
}