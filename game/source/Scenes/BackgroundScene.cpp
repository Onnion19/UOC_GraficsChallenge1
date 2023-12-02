#include "Scenes/BackgroundScene.h"
#include "Scenes/SceneManager.h"
#include "Utils/GameObjectEmitter.h"
#include "Utils/GameplayManager.h"
#include "Utils/RandomGenerator.h"
#include "Components/Sprite.h"
#include "Core/WindowManager.h"
#include "GameObjects/Mario.h"
#include "GameObjects/DK.h"
#include "GameObjects/HUD.h"
#include "GameObjects/Scenario.h"
#include "GameObjects/Enemies.h"
#include "GameObjects/PowerUp.h"

#if DEBUG
#include "Core/Physics.h"
#endif


BackgroundScene::BackgroundScene(Core::GameManagers& manager)
	: Scenes::SceneBase<BackgroundScene>(manager) {
}


BackgroundScene::~BackgroundScene()
{
}

void BackgroundScene::Activate()
{
	// Get screen size to compute game object size based on resolution (Need to improve to make the code more redable)
	auto screenSize = managers.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	hud = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::HUD>();
	mario = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Mario>(Utils::Vector2f{ screenSize.x *0.0156f ,screenSize.y * 0.9444f });

	dk = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::DK>(Utils::Vector2f{ screenSize.x * 0.4167f,screenSize.y * 0.2963f }, screenSize.x * 0.15625f, screenSize.x* 0.047f);
	enemiesSpawner = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::EnemiesPool>(70);

	auto handle = managers.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(mapTextureId, mapTexturePath);
	map = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Scenario>(std::move(handle));
	PrepareEnemiesSpawner();
	RegisterPowerUps(screenSize);
	gameplayManager = &managers.GetManager<GameplayManager>();
	gameOver = false;
}

void BackgroundScene::DeActivate()
{
	mario.reset();
	dk.reset();
	enemiesSpawner.reset();
	map.reset();	
	powerUps.clear();
}

void BackgroundScene::Update(float deltaTime) {

	gameOver = gameplayManager->IsGameOver();
	if (!gameOver)
	{
		mario->Update(deltaTime);
		dk->Update(deltaTime);
		enemiesSpawner->Update(deltaTime);
	}
	else
	{
		managers.GetManager<GameplayManager>().EndGame();
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "EndScene" });
	}
}

void BackgroundScene::Draw() {
	map->Draw();
#if DEBUG
	managers.GetManager<Core::PhysicsManager>().DrawDebugColliders();
#endif
	std::for_each(powerUps.begin(), powerUps.end(), [](auto& p) {p->Draw(); });
	dk->Draw();
	enemiesSpawner->Draw();
	mario->Draw();
	hud->Draw();
}

void BackgroundScene::Finish() {

}

void BackgroundScene::PrepareEnemiesSpawner()
{
	auto screenSize = managers.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	// Prepare 1st Row enemies
	{
		Utils::RepeatingTimerWithVariation timer{ Utils::RandomGenerator::GenerateRandom(1.f,4.f) ,
			[](unsigned int iterations, float initialTime) -> float {return Utils::RandomGenerator::GenerateRandom(1.f,4.f); } };

		auto params = [x = screenSize.x, y = screenSize.y]() ->  GameObject::EnemiesPool::SpawnEnemyProperties {return { EnemyType::Cake, {x * 1.f, y * 0.945f}, {-70.f, 0.f} }; };
		enemiesCallbackObjects.emplace_back(enemiesSpawner->ScheduleSpawnEnemy(std::move(params), std::move(timer)));
	}

	// Prepare 2nd row enemies

	{
		Utils::RepeatingTimerWithVariation timer{ Utils::RandomGenerator::GenerateRandom(2.f,6.f) ,
			[](unsigned int iterations, float initialTime) -> float {return Utils::RandomGenerator::GenerateRandom(2.f,6.f); } };

		auto params = [x = screenSize.x, y = screenSize.y]() ->  GameObject::EnemiesPool::SpawnEnemyProperties {return { EnemyType::Barrel , {x * 0.01f, y * 0.79f}, {150.f, 0.f} }; };
		enemiesCallbackObjects.emplace_back(enemiesSpawner->ScheduleSpawnEnemy(std::move(params), std::move(timer)));
	}


	// Prepare 3rd row enemies

	{
		Utils::RepeatingTimerWithVariation timer{ Utils::RandomGenerator::GenerateRandom(1.f,5.5f) ,
			[](unsigned int iterations, float initialTime) -> float {return Utils::RandomGenerator::GenerateRandom(1.f,5.5f); } };

		auto params = [x = screenSize.x, y = screenSize.y]() ->  GameObject::EnemiesPool::SpawnEnemyProperties {
			const Utils::Vector2f speed = (Utils::RandomGenerator::GenerateRandomBool()) ? Utils::Vector2f{ 90.f, 0.f } : Utils::Vector2f{ -90.f, 0.f };
			return { EnemyType::Fire , {x * 0.5f, y * 0.48f}, speed };
			};
		enemiesCallbackObjects.emplace_back(enemiesSpawner->ScheduleSpawnEnemy(std::move(params), std::move(timer)));
	}
}

void BackgroundScene::RegisterPowerUps(const Utils::Vector2i& screenSize)
{
	powerUps.reserve(10);
	auto texture = managers.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(ResourceID{ "Bonus atlas" }, "resources/Bonus/BonusAtlas.png");
	const Utils::Vector2i atlasSize{ 4,1 };
	const Utils::Vector2f peachPosition{ screenSize.x * 0.5f, screenSize.y * 0.2f };
	const Utils::Vector2f size{ screenSize.x * 0.0365f, screenSize.x * 0.0365f };
	const float powerUpPositionY = screenSize.y * 0.62f;

	powerUps.push_back(GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Peach>(texture, atlasSize, Utils::Vector2i{0,0}, peachPosition, size));
	powerUps.push_back(GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::PowerUp>(texture, atlasSize, Utils::Vector2i{1,0}, Utils::Vector2f{screenSize.x * 0.04f,powerUpPositionY}, size));
	powerUps.push_back(GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::PowerUp>(texture, atlasSize, Utils::Vector2i{2,0}, Utils::Vector2f{screenSize.x * 0.5f,powerUpPositionY}, size));
	powerUps.push_back(GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::PowerUp>(texture, atlasSize, Utils::Vector2i{3,0}, Utils::Vector2f{screenSize.x * 0.87f,powerUpPositionY}, size));
}


