#include <timer.h>
#include <serial_bfs.h>
#include <topdown_pbfs.h>
#include <omp.h>

int main(int argc, const char *argv[]) {
  omp_set_num_threads(6);
  std::string bench = "web-Stanford";
  std::string path = "data/" + bench + ".txt";
  // std::unique_ptr<Bfs> g = std::make_unique<SerialBfs>(path);
  std::unique_ptr<Bfs> g = std::make_unique<TopdownPbfs>(path);

  Timer timer;
  timer.benchOnce(g.get(), g->o2c[2]);
  timer.report(g->m);
  g->write("out.txt");
  return 0;
}