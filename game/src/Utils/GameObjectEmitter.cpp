#include "GameObjectEmitter.h"
#include "GameObjects/GameOjbect.h"
#include <Utils/RandomGenerator.h>

namespace {
	GameObject::AsteroidTransform MakeAsteroidTransfromFrom(const Geometry::Point& origin, const Geometry::Point& dest, float speed, int size)
	{
		return
		{
			origin,
			{ size,size },
			Utils::NormalizeVector(dest - origin) * speed
		};

	}
}

GameObject::Asteroid Utils::ObjectEmiter::SpawnAsteroidInRange(const Geometry::Point& origin, const Geometry::Point& dest, float speed, int size)
{
	const auto transform = MakeAsteroidTransfromFrom(origin, dest, speed, size);
	return GameObject::GameObjectFactory::MakeGameObject<GameObject::Asteroid>(std::move(transform));
}

GameObject::Asteroid Utils::ObjectEmiter::SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Point& dest, float speed, int size)
{
	const Geometry::Point origin_p{
		Utils::RandomGenerator::GenerateRandom(origin.begin.x, origin.end.x),
		Utils::RandomGenerator::GenerateRandom(origin.begin.y, origin.end.y)
	};

	return SpawnAsteroidInRange(origin_p, dest, speed, size);
}

GameObject::Asteroid Utils::ObjectEmiter::SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Line& dest, float speed, int size)
{
	const Geometry::Point end_p{
		Utils::RandomGenerator::GenerateRandom(dest.begin.x, dest.end.x),
		Utils::RandomGenerator::GenerateRandom(dest.begin.y, dest.end.y)
	};

	return SpawnAsteroidInRange(origin, end_p, speed, size);
}