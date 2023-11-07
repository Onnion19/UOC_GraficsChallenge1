#include "Game.h"
#include "Resources/ResourceManager.h"
#include "Scenes/SceneManager.h"
#include "Scenes/BackgroundScene.h"
#include "Scenes/EndScreen.h"
#include "Scenes/MainMenu.h"
#include "Scenes/LogoScene.h"
#include "Core/Physics.h"
#include "GameObjects/GameOjbect.h"
#include "Utils/GameplayManager.h"
#include "Resources/Texture.h"

Core::Game::Game(std::string_view name)
	: mainWindow(CreateWindow(1920, 1080, name))
	, gameLoop([window = mainWindow.get()]() {return window->WantsToClose(); }, mainWindow.get())
	, managers() {

	managers.RegisterManager<SceneManager>().RegisterListener(&gameLoop);
	managers.RegisterManager<ResourceManager>();
	managers.RegisterManager<PhysicsManager>();
	managers.RegisterManager<GameplayManager>();

	GameObject::GameObjectFactory::gManager = &managers;
}

Core::Game::~Game()
{
	GameObject::GameObjectFactory::gManager = nullptr;
}

void Core::Game::Initialize()
{
	managers.GetManager<SceneManager>().AddScene<BackgroundScene>(ResourceID{ "GameScene" }, false);
	managers.GetManager<SceneManager>().AddScene<EndScene>(ResourceID{ "EndScene" }, false);
	managers.GetManager<SceneManager>().AddScene<MainMenu>(ResourceID{ "MainMenu" }, false);
	managers.GetManager<SceneManager>().AddAndLoadScene<LogoScene>(ResourceID{ "Logo" }, false);
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
