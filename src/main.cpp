#include <timer.h>
#include <serial_bfs.h>
#include <topdown_pbfs.h>
#include <omp.h>

int main(int argc, const char *argv[]) {
  std::string bench = "com-orkut.ungraph";
  std::string path = "data/" + bench + ".txt";
  std::unique_ptr<Bfs> g = std::make_unique<TopdownPbfs>(path, "RMAT");
  // std::unique_ptr<Bfs> g = std::make_unique<TopdownPbfs>(path);//, "RMAT");
  srand(0xC5121);
  Timer timer;
  for (int i = 0; i < 32; ++i) {
    std::uint32_t src = rand() % /*g->n;*/ 3072627;
    std::clog << "src = " << src << ", " << g->o2c[src] << std::endl;
    timer.benchOnce(g.get(), g->o2c[src]);
  }
  timer.report(g->m);
  g->write("out.txt");
  return 0;
}