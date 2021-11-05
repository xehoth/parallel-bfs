#include <timer.h>
#include <serial_bfs.h>
#include <topdown_pbfs.h>
#include <omp.h>

int main(int argc, const char *argv[]) {
  // omp_set_num_threads(8);
  std::string bench = "web-Stanford";
  std::string path = "data/" + bench + ".txt";
  // std::unique_ptr<Bfs> g = std::make_unique<SerialBfs>(path);
  std::unique_ptr<Bfs> g = std::make_unique<TopdownPbfs>(path);
  srand(0xC5121);
  Timer timer;
  for (int i = 0; i < 32; ++i) {
    timer.benchOnce(g.get(), rand() % g->n);
  }
  timer.report(g->m);
  g->write("out.txt");
  return 0;
}