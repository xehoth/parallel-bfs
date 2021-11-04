//
// Created by xehoth on 2021/11/4.
//

#ifndef CS121_LAB_INCLUDE_GRAPH_H_
#define CS121_LAB_INCLUDE_GRAPH_H_
#include <filesystem>
#include <vector>
class Graph {
 public:
  void load(const std::filesystem::path &path);
  [[nodiscard]] inline int deg(int i) const {
    return static_cast<int>(this->g[i + 1] - this->g[i]);
  }

  std::vector<int> originToTrans;
  std::vector<int> transToOrigin;

  int **g;
  int nVertices, nEdges;

 private:
  std::unique_ptr<int *[]> gBuffer;
  std::unique_ptr<int[]> vBuffer;
};

#endif  // CS121_LAB_INCLUDE_GRAPH_H_
