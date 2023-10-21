#include "Game.h"
#include "Resources/ResourceManager.h"
#include "Scenes/SceneManager.h"
#include "Scenes/BackgroundScene.h"
#include "Core/Physics.h"
#include "GameObjects/GameOjbect.h"

Core::Game::Game(std::string_view name)
	: mainWindow(CreateWindow(1920, 1080, name))
	, gameLoop([window = mainWindow.get()]() {return window->WantsToClose(); }, mainWindow.get())
	, managers() {

	managers.RegisterManager<SceneManager>().RegisterListener(&gameLoop);
	managers.RegisterManager<ResourceManager>();
	managers.RegisterManager<PhysicsManager>();

	GameObject::GameObjectFactory::gManager = &managers;
}

Core::Game::~Game()
{
	GameObject::GameObjectFactory::gManager = nullptr;
}

void Core::Game::Initialize()
{
	managers.GetManager<SceneManager>().AddAndLoadScene<BackgroundScene>(ResourceID{ "Background" }, false);
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
