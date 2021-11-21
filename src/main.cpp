//
// Created by xehoth on 2021/11/4.
//
#include <timer.h>
#include <serial_bfs.h>
#include <topdown_pbfs.h>
#include <hybrid_pbfs.h>
#include <omp.h>

void showUsage() {
  std::cout << "usage (bench): <input> <format> <bench output> -1 <bfs type> <thread num>"
            << std::endl
            << "usage (run): <input> <format> <output> <source node> <bfs type> <thread num>"
            << std::endl
            << "<format>: choose from SNAP, MatrixMarket, RMAT" << std::endl
            << "<bfs type>: choose from: serial, topdown, hybrid" << std::endl;
}

int main(int argc, const char *argv[]) {
  std::string input, output, format, type;
  int source;
  if (argc == 7) {
    input = argv[1];
    format = argv[2];
    output = argv[3];
    source = std::stoi(argv[4]);
    type = argv[5];
    omp_set_num_threads(std::stoi(argv[6]));
  } else {
    showUsage();
    return 0;
  }
  std::unique_ptr<Bfs> g;
  if (type == "serial") {
    g = std::make_unique<SerialBfs>(input, format);
  } else if (type == "topdown") {
    g = std::make_unique<TopdownPbfs>(input, format);
  } else if (type == "hybrid") {
    g = std::make_unique<HybridBfs>(input, format);
  } else {
    std::cerr << "invalid bfs type!" << std::endl;
    showUsage();
    return 0;
  }
  bool bench = source == -1;
  Timer timer;
  if (bench) {
    std::clog << "benchmark mode" << std::endl;
    for (int _ = 0; _ < 4; ++_) {
      std::clog << "round " << _ << " begin" << std::endl;
      srand(495);
      for (int i = 0; i < 20; ++i) {
        std::uint32_t src = rand() % g->n;
        std::clog << "  step " << i << ": source = (" << src << " -> "
                  << g->o2c[src] << ")" << std::endl;
        timer.benchOnce(g.get(), g->o2c[src]);
      }
      std::clog << "round " << _ << " done" << std::endl;
    }
    timer.report(g->m);
    timer.write(g->m, output);
  } else {
    std::clog << "run mode" << std::endl;
    timer.benchOnce(g.get(), static_cast<std::uint32_t>(g->o2c[source]));
    timer.report(g->m);
    g->write(output);
  }
  return 0;
}