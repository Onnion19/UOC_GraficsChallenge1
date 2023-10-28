#include "BackgroundScene.h"
#include "Scenes/SceneManager.h"
#include "Utils/GameObjectEmitter.h"
#include "Utils/GameplayManager.h"
#include "Utils/RandomGenerator.h"
namespace {
	struct AsteroidsTimerFunctor
	{
		float operator()(unsigned timesTriggered, float initialTime)
		{
			return std::max(1.f, initialTime * (1 - (timesTriggered * 0.1f)));
		}
	};
}


BackgroundScene::BackgroundScene(Core::GameManagers& manager)
	: Scenes::SceneBase<BackgroundScene>(manager)
	, asteroidsSpawnerTimer(5.f, AsteroidsTimerFunctor{}) {
	asteroids.reserve(100);
}


void BackgroundScene::Activate()
{
	// Create actors
	spaceship = GameObject::GameObjectFactory::MakeGameObjectHandle <GameObject::Spaceship>(Utils::Vector2f{ 500.f , 500.f });
	hud = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::HUD>();

	auto initial_asteroids = Utils::RandomGenerator::GenerateRandom(1u, 5u);
	for(auto i = 0u; i < initial_asteroids; i++)
		SpawnAsteroid();

	// Reset gameplay data
	auto& gameplayManager = managers.GetManager<GameplayManager>();
	gameplayManager.SetScore(0);
	gameplayManager.SetHealth(3);
	gameplayManager.StartGame();
	// Register callbacks
	healthCallback = gameplayManager.RegisterHealthCallback(*this);

	asteroidsSpawnerObject = asteroidsSpawnerTimer.Start([this]() {SpawnAsteroid(); });
}

void BackgroundScene::DeActivate()
{
	// Unload all resources. This should be automated but the current core can't do it....
	auto& resourceManager = managers.GetManager<ResourceManager>();
	resourceManager.Unload<Texture2D>(GameObject::Asteroid::asteroidTextureID);
	resourceManager.Unload<Texture2D>(GameObject::Bullet::bulletTextureId);
	resourceManager.Unload<Texture2D>(GameObject::Spaceship::sppaceshipTextureID);

	asteroids.clear();
}

void BackgroundScene::Update(float deltaTime) {
	asteroidsSpawnerTimer.Update(deltaTime);
	std::for_each(asteroids.begin(), asteroids.end(),
		[deltaTime](GameObject::Asteroid& asteroid) {
			asteroid.Update(deltaTime);
		});
	spaceship->Update(deltaTime);
	hud->Update(deltaTime);
}

void BackgroundScene::Draw() {
	hud->Draw();
	spaceship->Draw();
	std::for_each(asteroids.begin(), asteroids.end(),
		[](GameObject::Asteroid& asteroid) {
			asteroid.Draw();
		});
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

void BackgroundScene::SpawnAsteroid()
{
	/*
		TODO: This way will mean that eventually the buffer will be full even with non valid asteroids
		so each update will try to update all 100.

		An improvement is to remove invalid ones, by MOVING the invalid object with the last one.
		This will require to implement a propre move operator for the asteroid.
	*/
	static constexpr Geometry::Line OriginLine{ Geometry::Point {0, 0}, Geometry::Point{1920, 0} };
	static constexpr Geometry::Line EndLine{ Geometry::Point {0, 1080}, Geometry::Point{1920, 1080} };

	const GameObject::Asteroid newAsteroid = Utils::ObjectEmiter::SpawnAsteroidInRange(OriginLine, EndLine, 50.f, 20);

	// Search for an invalid asteroid to re-use
	auto iter = std::find_if(asteroids.begin(), asteroids.end(), [](const GameObject::Asteroid& asteroid) {return !asteroid.Valid(); });
	if (iter != asteroids.end())
	{
		*iter = newAsteroid;
	}
	else if (asteroids.size() < asteroids.capacity())
	{
		// All asteroids are valid, try to push a new one.
		asteroids.push_back(newAsteroid);
	}
}

