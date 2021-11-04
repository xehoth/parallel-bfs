//
// Created by xehoth on 2021/11/4.
//

#ifndef CS121_LAB_INCLUDE_WRITER_H_
#define CS121_LAB_INCLUDE_WRITER_H_
#include <fstream>
#include <filesystem>

class Writer {
 public:
  explicit Writer(const std::filesystem::path &path,
                  size_t bufferSize = 1024 * 1024);
  ~Writer();
  void write(char c) {
    if (s == buffer.get() + bufferSize) {
      flush();
      s = buffer.get();
    }
    *s++ = c;
  }

  void write(int x) {
    int buf[12], cnt;
    if (x == 0) {
      write('0');
      return;
    }
    if (x < 0) {
      write('-');
      x = -x;
    }
    for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
    while (cnt) write(static_cast<char>(buf[cnt--]));
  }

  Writer &operator<<(char c) {
    write(c);
    return *this;
  }

  Writer &operator<<(int x) {
    write(x);
    return *this;
  }

  void flush() const { stream.rdbuf()->sputn(buffer.get(), s - buffer.get()); }

 private:
  std::ofstream stream;
  std::unique_ptr<char[]> buffer;
  size_t bufferSize;
  char *s;
};
#endif  // CS121_LAB_INCLUDE_WRITER_H_
