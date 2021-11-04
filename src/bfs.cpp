//
// Created by xehoth on 2021/11/4.
//
#include <bfs.h>
#include <writer.h>

void Bfs::write(const std::filesystem::path &path) const {
  Writer writer(path);
  std::vector<std::pair<int, int>> output(graph.originToTrans.size());
  for (int i = 0; i < graph.nVertices; ++i) {
    output[graph.transToOrigin[i]] = std::make_pair(
        dist[i], parent[i] != -1 ? graph.transToOrigin[parent[i]] : -1);
  }
  for (size_t i = 0; i < output.size(); ++i) {
    if (graph.originToTrans[i] != -1) {
      writer << static_cast<int>(i) << ' ' << output[i].first << ' '
             << output[i].second << '\n';
    }
  }
}
