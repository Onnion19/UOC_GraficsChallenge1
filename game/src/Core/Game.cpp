#include "Game.h"
#include "Resources/ResourceManager.h"
#include "Scenes/SceneManager.h"
#include "Scenes/BackgroundScene.h"

Core::Game::Game(std::string_view name)
	: mainWindow(CreateWindow(800, 400, name))
	, gameLoop([window = mainWindow.get()]() {return window->WantsToClose(); }, mainWindow.get())
	, managers(){

	auto& sceneManager = managers.RegisterManager<SceneManager>();
	sceneManager.RegisterListener(&gameLoop); // Feels weird to reigster the game loop as scene listener. Probably this requires some decoupling
	managers.RegisterManager<ResourceManager>();
}

void Core::Game::Initialize()
{
	managers.GetManager<SceneManager>().AddAndLoadScene<BackgroundScene>(ResourceID{"Background"}, false);
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
	managers.GetManager<SceneManager>().Clear();
}
