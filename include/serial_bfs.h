//
// Created by xehoth on 2021/11/4.
//

#ifndef CS121_LAB_INCLUDE_SERIAL_BFS_H_
#define CS121_LAB_INCLUDE_SERIAL_BFS_H_
#include <bfs.h>

struct SerialBfs : public Bfs {
  using Bfs::Bfs;
  void bfs(int s) override;
};
#endif  // CS121_LAB_INCLUDE_SERIAL_BFS_H_
