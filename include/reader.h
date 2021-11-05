//
// Created by xehoth on 2021/11/4.
//
#ifndef READER_H_
#define READER_H_
#include <string>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <memory>

class Reader {
 public:
  explicit Reader(const std::string &path) {
    std::ifstream stream(path);
    std::stringstream content;
    content << stream.rdbuf();
    stream.close();
    this->buffer = content.str();
    this->it = this->buffer.cbegin();
  }

  char get() const { return *it; }
  void next() { ++it; }
  void skipLine() {
    while (!isEof() && get() != '\n' && get() != '\r') next();
    while (get() == '\n' || get() == '\r') next();
  }
  void skipSpace() {
    while (!isEof() && std::isspace(get())) next();
  }
  void readInt(std::uint32_t &ret) {
    std::uint32_t x = 0;
    while (!isEof() && !std::isdigit(get())) {
      if (get() == '#') {
        skipLine();
        continue;
      }
      next();
    }
    if (isEof()) return;
    while (!isEof() && std::isdigit(get())) {
      x = x * 10 + (get() ^ 48);
      next();
    }
    ret = x;
  }
  Reader &operator>>(std::uint32_t &x) {
    readInt(x);
    return *this;
  }
  [[nodiscard]] bool isEof() const { return it == buffer.cend(); }

 private:
  std::string buffer;
  std::string::const_iterator it;
};
#endif