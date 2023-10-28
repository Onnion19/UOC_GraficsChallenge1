#pragma once

#include "Utils/Vector2.hpp"
#include <variant>
#include <tuple>
#include <assert.h>
#include <cmath>

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
	inline T GetGeometryData(const GeometryData& data)
	{
#if NDEBUG
		return std::get<T>(data);
#else
		auto ptr = std::get_if<T>(&data);
		assert(ptr != nullptr && "Trying to get the geometry data using the wrong type");
		return (ptr) ? *ptr : T{};
#endif		
	}

	inline constexpr float _PI = 3.14159265f;
	inline constexpr float DEG_TO_RAD = _PI / 180.f;
	inline constexpr float RAD_TO_DEG = 1 / DEG_TO_RAD;


	inline Utils::Vector2f ForwardVector(float roationDeg)
	{
		roationDeg += 90; 
		const float angle = roationDeg * DEG_TO_RAD;
		return { std::cos(angle) , std::sin(angle) };
	}
}