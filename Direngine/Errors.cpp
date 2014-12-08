#include "Errors.h"
#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

//Prints out an error message and exits the game
namespace Debug {
  void FatalError(std::string _errorString) {
    std::cout << _errorString << std::endl;
    std::cout << "Enter any key to quit...";
    int tmp;
    std::cin >> tmp;
    SDL_Quit();
    exit(1);
  }

  void Log(std::string _logString) {
    std::cout << _logString << std::endl;
  }
}