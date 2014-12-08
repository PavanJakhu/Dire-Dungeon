#pragma once
#include <string>

// Header for global error functions

namespace Debug {
  extern void FatalError(std::string _errorString);
  extern void Log(std::string _logString);
}