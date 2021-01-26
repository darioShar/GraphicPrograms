#include "MainGame.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "Sprite.h"
#include "Errors.h"
#include "ImageLoader.h"

// TOUTES LES VITESSES PRECISEES ICI SERONT CELLE DES GOUTTES LES PLUS RAPIDES (les plus grosses).
double yspeedInit = 0.000002;     
double xspeedInit = 0.0000004;
long long timeBegin;
long timeElapsed;
//double yspeed = 0.02;
//double xspeed = 0.001;
double yacceleration = 0.00002;
double xacceleration = 0.0000007;
bool random = 0;
const unsigned int rainDrops = 1000;
double raindDropRatio = 5.5;   // rapport entre width et height
unsigned int rainDropSizeRange = 5;  // entre 0 et 5.  5 est donc la plus grosse goutte possible.
double raindDropReductionFactor = 0.0004;



MainGame::MainGame() : _time(0), _window(0), _screenWidth(1600),_screenHeight(900), _gameState(GameState::PLAY)
{
	

}


MainGame::~MainGame()
{
}


void MainGame::run()
{
	initSystems();

	//_sprite.init(0.0f,1.0f, .05f, .05f);
	
	
	for (int i = 0; i < rainDrops; i++)
	{
		double rainDropWidth = ((rand() % (rainDropSizeRange +1)) + rainDropSizeRange) * raindDropReductionFactor;
		double rainDropHeight = rainDropWidth * raindDropRatio;
		(_sprite[i]).init(((rand()%200)/100.0)-1.0, (rand()%200)/100.0 - 1.0, rainDropWidth, rainDropHeight);    // On génére aléatoirement la "pluie".
	} 

	//_sprite.init(-0.4, -0.75, 0.8, 1.5);
	//_playerTexture = ImageLoader::loadPNG("Textures/JimmyJumpPack/PNG/CharacterRight_Standing.png"); ///////////////////////////////////////////////////////////////////////////////  Ligne a corriger pou rajouter texture

	gameLoop();
}


void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL_Window could not be created.");
	}
	//gameLoop();
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created.");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew.");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Background white
	glClearColor(1.0, 1.0, 1.0, 1.0);

	initShaders();
	

}

void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	//_colorProgram.addAttribute("vertexUV");                /////////////////////////////////////////////////////               Ligne a corriger pour rajouter texture
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT) {
		processInput();
		_time += 0.1;
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT :
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION :
			std::cout << evnt.motion.x << " " << evnt.motion.y << std:: endl;
			break;
		case SDL_KEYDOWN : 
			//Sleep(100);
			break;
		}
	}
}

void MainGame::drawGame()
{
	
	
	
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();

	//We are using texture unit 0
	//glActiveTexture(GL_TEXTURE0);
	//Bind the texture to texture unit 0
	//glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	//Get the uniform location
	//GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//Tell the shader that the texture is in texture unit 0
	//glUniform1i(textureLocation, 0);

	//GLuint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);


	//Sleep(12.5);       // On attend un peu afin de réduire la charge de travail.
	for (int i = 0; i < rainDrops; i++)
	{
	_sprite[i].moveThrough(-1, -1, xspeedInit, yspeedInit, xacceleration, yacceleration, random, (_sprite[i].getWidth()) / (rainDropSizeRange*raindDropReductionFactor));  // les deux directions(x et y), les vitesses initiales, les accélérations.
	}


	//_sprite.draw();

	//glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();
	
	
	/*if (_sprite[0].getYPos() >= 1 - yspeed-0.01)
	{
		yspeed = 0.0001;   // Vitesse initiale.
	}
	else
	{
		yspeed += 0.0000002;
	}*/

	/*for (int i = 0; i < 1000; i++)
	{
		_sprite[i].draw();
		_sprite[i].move(0.0, -yspeed);
	}*/
	//////  ATTENTION !!!! En réalité il faudrait que l'accélérationsoit propre à chaque gouttelette.
	SDL_GL_SwapWindow(_window);
}

