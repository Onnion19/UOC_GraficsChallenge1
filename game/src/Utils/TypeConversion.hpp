#pragma once
#include "raylib.h"
#include "Utils/Vector2.hpp"
#include "Utils/Geometry.h"


namespace GeometryToRaylib {
	inline Rectangle RectangleToRaylib(const Geometry::Rectangle& r)
	{
		return {
			r.topLeft.x,
			r.topLeft.y,
			r.botRight.x - r.topLeft.x,
			r.botRight.y - r.topLeft.y
		};
	}

	inline Rectangle RectangleToRaylib(const Utils::Vector2f& pos, const Utils::Vector2f& size)
	{
		return {
			pos.x,
			pos.y,
			size.x,
			size.y
		};
	}

	inline Rectangle RectangleToRaylib(const Utils::Vector2i& pos, const Utils::Vector2i& size)
	{
		return {
			static_cast<float>(pos.x),
			static_cast<float>(pos.y),
			static_cast<float>(size.x),
			static_cast<float>(size.y)
		};
	}
}

namespace TypesToRayLib {

	inline Vector2 VectorToRaylib(const Utils::Vector2f& vec)
	{
		return { vec.x, vec.y };
	}

	inline Vector2 VectorToRaylib(const Utils::Vector2i& vec)
	{
		return { static_cast<float>(vec.x) , static_cast<float>(vec.y) };
	}
}
