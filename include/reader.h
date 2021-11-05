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
  explicit Reader(const std::string &path, size_t bufferSize = 256 << 20)
      : stream(path), bufferSize(bufferSize), eof() {
    buffer.resize(bufferSize);
    it = buffer.cend();
    updateBuffer();
  }

  char get() const { return *it; }
  void next() {
    ++it;
    updateBuffer();
  }
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
  void updateBuffer() {
    if (it == buffer.cend()) {
      size_t size = stream.rdbuf()->sgetn(buffer.data(), bufferSize);
      if (size != buffer.size()) buffer.resize(size);
      it = buffer.cbegin();
      if (!size) eof = true;
    }
  }
  Reader &operator>>(std::uint32_t &x) {
    readInt(x);
    return *this;
  }
  [[nodiscard]] bool isEof() const { return eof; }

 private:
  std::ifstream stream;
  size_t bufferSize;
  std::string buffer;
  std::string::const_iterator it;
  bool eof;
};
#endif