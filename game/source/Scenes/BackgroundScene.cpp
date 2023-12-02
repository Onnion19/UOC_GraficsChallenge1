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
	hud = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::HUD>();
	mario = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Mario>(Utils::Vector2f{ 30	,1020 });
	dk = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::DK>(Utils::Vector2f{ 800,320 }, 300.f, 90.f);
	enemiesSpawner = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::EnemiesPool>(70);

	auto handle = managers.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(mapTextureId, mapTexturePath);
	map = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Scenario>(std::move(handle));
	PrepareEnemiesSpawner();
}

void BackgroundScene::DeActivate()
{
	mario.reset();
}

void BackgroundScene::Update(float deltaTime) {
	mario->Update(deltaTime);
	dk->Update(deltaTime);
	enemiesSpawner->Update(deltaTime);
}

void BackgroundScene::Draw() {
	map->Draw();
#if DEBUG
	managers.GetManager<Core::PhysicsManager>().DrawDebugColliders();
#endif
	dk->Draw();
	enemiesSpawner->Draw();
	mario->Draw();
	hud->Draw();
}

void BackgroundScene::Finish() {

}

void BackgroundScene::OnHealthUpdate(unsigned int newHealth)
{
	if (newHealth == 0)
	{
		managers.GetManager<GameplayManager>().EndGame();
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "EndScene" });
	}
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
			const Utils::Vector2f speed = (Utils::RandomGenerator::GenerateRandomBool()) ? Utils::Vector2f{90.f, 0.f} : Utils::Vector2f{-90.f, 0.f};
			return { EnemyType::Fire , {x * 0.5f, y * 0.48f}, speed}; 
			};
		enemiesCallbackObjects.emplace_back(enemiesSpawner->ScheduleSpawnEnemy(std::move(params), std::move(timer)));
	}
}


