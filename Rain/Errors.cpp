#include "Errors.h"
#include <iostream>
#include <SDL\SDL.h>
#include <cstdlib>

// Prints error message
void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit..." << std::endl;
	std::cin.get();
	SDL_Quit();
	exit(1);
}