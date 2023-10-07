#include "Game.h"

void Core::Game::Initialize()
{
}

void Core::Game::Start()
{
	// Initialize all resources and other requirements
	Initialize();

	// Play the game
	gameLoop.Loop();

	// when reach this point the game loop has stoped, so we can de initialize everything
	DeInitialize();
}

void Core::Game::DeInitialize()
{
}
