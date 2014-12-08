#include "IOManager.h"
#include <fstream>

bool IOManager::ReadFileToBuffer(std::string _filePath, std::vector<unsigned char>& _buffer) {
  std::ifstream file(_filePath, std::ios::binary);
  if (file.fail()) {
    perror(_filePath.c_str());
    return false;
  }

  // Seek to the end
  file.seekg(0, std::ios::end);

  // Get the file size
  int fileSize = (int)file.tellg();
  file.seekg(0, std::ios::beg);

  // Reduce the file size by any header bytes that might be present
  fileSize -= (int)file.tellg();

  _buffer.resize(fileSize);
  file.read((char *)&(_buffer[0]), fileSize);
  file.close();

  return true;
}