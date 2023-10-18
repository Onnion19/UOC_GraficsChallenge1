#pragma once

#include "Utils/Vector2.hpp"
#include <variant>
#include <tuple>
#include <assert.h>

namespace Geometry {

	using Point = Utils::Vector2f;
	struct Line {
		Point begin;
		Point end;
	};

	struct Circle {
		Point center;
		float radius;
	};
	struct Rectangle {
		Point topLeft;
		Point botRight;
	};


	using GeometryData = std::variant<Point, Line, Circle, Rectangle>;


	template<typename T>
	T GetGeometryData(const GeometryData& data)
	{
#if NDEBUG
		return std::get<T>(&data);
#else
		auto ptr = std::get_if<T>(&data);
		assert(ptr != nullptr && "Trying to get the geometry data using the wrong type");
		return (ptr) ? *ptr : T{};
#endif		
	}

}