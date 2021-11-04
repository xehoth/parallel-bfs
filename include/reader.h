//
// Created by xehoth on 2021/11/4.
//

#ifndef CS121_LAB_INCLUDE_READER_H_
#define CS121_LAB_INCLUDE_READER_H_
#include <filesystem>
#include <string>
#include <cctype>

class Reader {
 public:
  explicit Reader(const std::filesystem::path &path);
  [[nodiscard]] char get() const { return *it; }
  void next() { ++it; }
  void skipLine() {
    while (!isEof() && get() != '\n' && get() != '\r') next();
    while (get() == '\n' || get() == '\r') next();
  }
  void skipSpace() {
    while (!isEof() && std::isspace(get())) next();
  }
  void readInt(int &ret) {
    int x = 0;
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
  Reader &operator>>(int &x) {
    readInt(x);
    return *this;
  }
  [[nodiscard]] bool isEof() const { return it == buffer.cend(); }

 private:
  std::string buffer;
  std::string::const_iterator it;
};
#endif  // CS121_LAB_INCLUDE_READER_H_
