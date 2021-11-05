#include <timer.h>
#include <serial_bfs.h>

int main(int argc, const char *argv[]) {
  std::string bench = "roadNet-CA";
  std::string path = "data/" + bench + ".txt";
  std::unique_ptr<Bfs> g = std::make_unique<SerialBfs>(path);
  Timer timer;
  timer.benchOnce(g.get(), g->o2c[2]);
  timer.report(g->m);
  g->write("out.txt");
  return 0;
}