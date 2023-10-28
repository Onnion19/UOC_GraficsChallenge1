#include "BackgroundScene.h"
#include "Scenes/SceneManager.h"
#include "Utils/GameObjectEmitter.h"
#include "Utils/GameplayManager.h"

BackgroundScene::BackgroundScene(Core::GameManagers& manager)
	: Scenes::SceneBase<BackgroundScene>(manager)
	, spaceship(GameObject::GameObjectFactory::MakeGameObject<GameObject::Spaceship>(Utils::Vector2f{ 500.f , 500.f }))
	, hud(GameObject::GameObjectFactory::MakeGameObject<GameObject::HUD>()) {
	asteroids.reserve(100);
}


void BackgroundScene::Activate()
{
	managers.GetManager<GameplayManager>().SetScore(0);
	managers.GetManager<GameplayManager>().SetHealth(3);
	SpawnAsteroid();
}

void BackgroundScene::Update(float deltaTime) {
	std::for_each(asteroids.begin(), asteroids.end(),
		[deltaTime](GameObject::Asteroid& asteroid) {
			asteroid.Update(deltaTime);
		});


	spaceship.Update(deltaTime);

}

void BackgroundScene::Draw() {
	hud.Draw();
	spaceship.Draw();
	std::for_each(asteroids.begin(), asteroids.end(),
		[](GameObject::Asteroid& asteroid) {
			asteroid.Draw();
		});
}

void BackgroundScene::Finish() {
	
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
	auto iter = std::find_if(asteroids.begin(), asteroids.end(), [](const GameObject::Asteroid& asteroid) {return asteroid.Valid(); });
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

