#include <timer.h>
#include <serial_bfs.h>
#include <topdown_pbfs.h>
#include <omp.h>

int main(int argc, const char *argv[]) {
  std::string bench = "RMAT1";
  std::string path = "data/" + bench + ".txt";
  // std::unique_ptr<Bfs> g = std::make_unique<SerialBfs>(p./math);
  std::unique_ptr<Bfs> g = std::make_unique<TopdownPbfs>(path, "RMAT");
  srand(0xC5121);
  Timer timer;
  for (int i = 0; i < 32; ++i) {
    timer.benchOnce(g.get(), rand() % g->n);
  }
  timer.report(g->m);
  g->write("out.txt");
  return 0;
}