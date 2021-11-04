//
// Created by xehoth on 2021/11/4.
//
#include <graph.h>
#include <serial_bfs.h>
#include <timer.h>
#include <iostream>

int main() {
  Graph g;
  g.load("../../../data/soc-LiveJournal1.txt");
  std::cout << "load done" << std::endl;
  std::cout << "vertices: " << g.nVertices << ", "
            << "edges: " << g.nEdges << std::endl;
  SerialBfs sbfs(g);
  Timer timer;

  auto testSbfs = [&](int s) {
    std::cout << "start bfs at " << g.transToOrigin[s] << " --- ";
    sbfs.init();
    timer.start();
    sbfs.bfs(s);
    timer.end();
    std::cout << "done" << std::endl;
  };

  for (int i = 1; i <= 100; ++i) {
    testSbfs(rand() % g.nVertices);
  }
  sbfs.write("out.txt");
  std::cout << timer.report(g.nEdges) << "me/s" << std::endl;
  return 0;
}