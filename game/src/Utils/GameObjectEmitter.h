#pragma once

// This whole class must be improved, this is just fast coding

#include "GameObjects/Asteroid.h"
#include "Utils/Geometry.h"
#include <type_traits>
#include <random>

namespace Utils {

	class ObjectEmiter {
	public:
		/*
		Will spawn an asteorid at given origin point.
		The asteroid will move at the direction of the given point
		*/

		static GameObject::Asteroid SpawnAsteroidInRange(const Geometry::Point& origin, const Geometry::Point& dest, float speed, int size);


		/*
		Will spawn an asteorid at any point on the given origin line.
		The asteroid will move at the direction of the given point
		*/
		static GameObject::Asteroid SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Point& dest, float speed, int size);

		/*
			Will spawn an asteorid at any point on the given origin line.
			The asteroid will move at the direction of any point of the given destiny line
		*/
		static GameObject::Asteroid SpawnAsteroidInRange(const Geometry::Line& origin, const Geometry::Line& dest, float speed, int size);
	};

}