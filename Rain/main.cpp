#include <Windows.h>
#include <SDL/SDL.h>
#include <GL\glew.h>
#include "MainGame.h"
#include <iostream>


int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();

	return 0;
}