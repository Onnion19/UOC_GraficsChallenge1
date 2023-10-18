#pragma once 

#include "Utils/Geometry.h"
#include "raylib.h"

namespace Core {

	class CollisionSolver
	{
	public:
		/**
		* Verifies if 2 Geometries overlaps each other
		*/
		static bool Collides(const Geometry::GeometryData& col1, const Geometry::GeometryData& col2);

	};
}