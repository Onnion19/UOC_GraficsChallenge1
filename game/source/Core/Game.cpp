#include "Core/Game.h"
#include "Core/WindowManager.h"
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
{

	managers.RegisterManager<Components::ComponentManager>();
	managers.RegisterManager<SceneManager>().RegisterListener(&gameLoop);
	managers.RegisterManager<ResourceManager>();
	managers.RegisterManager<PhysicsManager>();
	managers.RegisterManager<GameplayManager>();
	auto& windowManager = managers.RegisterManager<WindowManager>(name, 1920, 1080);
	GameObject::GameObjectFactory::RegisterGameManagers(managers);

	gameLoop = { [window = windowManager.GetCurrentWindow()]() {return window->WantsToClose(); }, windowManager.GetCurrentWindow() };
}

Core::Game::~Game()
{
	managers.UnregisterManager<Components::ComponentManager>();
	managers.UnregisterManager<SceneManager>();
	managers.UnregisterManager<ResourceManager>();
	managers.UnregisterManager<PhysicsManager>();
	managers.UnregisterManager<GameplayManager>();
	GameObject::GameObjectFactory::InvalidateGameManagers();
}

void Core::Game::Initialize()
{
	//managers.GetManager<SceneManager>().AddScene<BackgroundScene>(ResourceID{ "GameScene" }, false);
	//managers.GetManager<SceneManager>().AddScene<EndScene>(ResourceID{ "EndScene" }, false);
	//managers.GetManager<SceneManager>().AddScene<MainMenu>(ResourceID{ "MainMenu" }, false);
	//managers.GetManager<SceneManager>().AddAndLoadScene<LogoScene>(ResourceID{ "Logo" }, false);
	managers.GetManager<SceneManager>().AddAndLoadScene<BackgroundScene>(ResourceID{ "GameScene" }, false);
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
