#include "GameObjectEmitter.h"
#include "GameObjects/GameOjbect.h"


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
		random.GenerateFloatInRange(origin.begin.x, origin.end.x),
		random.GenerateFloatInRange(origin.begin.y, origin.end.y)
	};

	return SpawnAsteroidInRange(origin_p, dest, speed, size);
}

GameObject::Asteroid Utils::ObjectEmiter::SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Line& dest, float speed, int size)
{
	const Geometry::Point end_p{
			random.GenerateFloatInRange(dest.begin.x, dest.end.x),
			random.GenerateFloatInRange(dest.begin.y, dest.end.y)
	};

	return SpawnAsteroidInRange(origin, end_p, speed, size);
}

float Utils::ObjectEmiter::RandomGenerator::GenerateFloatInRange(float min, float max)
{
	std::uniform_real_distribution distribution(min, max);
	return distribution(randomGenerator);
}
