//
// Created by xehoth on 2021/11/4.
//
#ifndef WRITER_H_
#define WRITER_H_
#include <fstream>
#include <string>

class Writer {
 public:
  explicit Writer(const std::string &path, size_t bufferSize = 32 << 20)
      : stream(path),
        buffer(std::make_unique<char[]>(bufferSize)),
        bufferSize(bufferSize),
        s(buffer.get()) {}
  ~Writer() { flush(); }

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
#endif