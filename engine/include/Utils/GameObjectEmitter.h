#pragma once

// This whole class must be improved, this is just fast coding
#include "GameObjects/GameOjbect.h"
#include "Utils/RandomGenerator.h"
#include "Utils/Geometry.h"
#include <type_traits>
#include <random>

namespace Utils {

	template <typename T>
	concept GameObjectTy = std::is_base_of_v<GameObject::GameObject, T>;

	class ObjectEmiter {
	public:
		/*
		Will spawn an asteorid at given origin point.
		The asteroid will move at the direction of the given point
		*/
		template<GameObjectTy Ty>
		static Ty SpawnAsteroidInRange(const Geometry::Point& origin, const Geometry::Point& dest, float speed, int size);


		/*
		Will spawn an asteorid at any point on the given origin line.
		The asteroid will move at the direction of the given point
		*/
		template<GameObjectTy Ty>
		static Ty SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Point& dest, float speed, int size);

		/*
			Will spawn an asteorid at any point on the given origin line.
			The asteroid will move at the direction of any point of the given destiny line
		*/
		template<GameObjectTy Ty>
		static Ty SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Line& dest, float speed, int size);
	};

	/** implementation**/

	template<GameObjectTy Ty>
	inline Ty ObjectEmiter::SpawnAsteroidInRange(const Geometry::Point& origin, const Geometry::Point& dest, float speed, int size)
	{
		return GameObject::GameObjectFactory::MakeGameObject<Ty>(origin, size, Utils::NormalizeVector(dest - origin) * speed);
	}

	template<GameObjectTy Ty>
	inline Ty ObjectEmiter::SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Point& dest, float speed, int size)
	{
		const Geometry::Point origin_p{
			Utils::RandomGenerator::GenerateRandom(origin.begin.x, origin.end.x),
			Utils::RandomGenerator::GenerateRandom(origin.begin.y, origin.end.y)
		};

		return SpawnAsteroidInRange<Ty>(origin_p, dest, speed, size);
	}

	template<GameObjectTy Ty>
	inline Ty ObjectEmiter::SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Line& dest, float speed, int size)
	{
		const Geometry::Point end_p{
			Utils::RandomGenerator::GenerateRandom(dest.begin.x, dest.end.x),
			Utils::RandomGenerator::GenerateRandom(dest.begin.y, dest.end.y)
		};

		return SpawnAsteroidInRange<Ty>(origin, end_p, speed, size);
	}

}