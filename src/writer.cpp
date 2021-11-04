//
// Created by xehoth on 2021/11/4.
//
#include <writer.h>

Writer::Writer(const std::filesystem::path &path, size_t bufferSize)
    : stream(path),
      buffer(std::make_unique<char[]>(bufferSize)),
      bufferSize(bufferSize),
      s(buffer.get()) {}

Writer::~Writer() { flush(); }
