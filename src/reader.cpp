//
// Created by xehoth on 2021/11/4.
//
#include <reader.h>
#include <fstream>
#include <sstream>

Reader::Reader(const std::filesystem::path &path) {
  std::ifstream stream(path);
  std::stringstream content;
  content << stream.rdbuf();
  stream.close();
  this->buffer = content.str();
  this->it = this->buffer.cbegin();
}
