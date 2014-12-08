#pragma once
#include <vector>

class IOManager {
public:
  static bool ReadFileToBuffer(std::string _filePath, std::vector<unsigned char>& _buffer);
};