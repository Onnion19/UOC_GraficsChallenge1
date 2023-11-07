#include "CollisionSolver.h"
#include "raylib.h"
#include "Utils/TypeConversion.hpp"
// Internal functionalities.
// This could be move to an internal class in another file, but the premake is not defined to handle private files.
namespace {

	template<typename T1, typename T2>
	bool ResolveCollision(const T1& col1, const T2& col2) {
		return false;
	}

	template<>
	bool ResolveCollision<Geometry::Rectangle, Geometry::Rectangle>(const Geometry::Rectangle& t1, const Geometry::Rectangle& t2)
	{
		const Rectangle r1 = GeometryToRaylib::RectangleToRaylib(t1);
		const Rectangle r2 = GeometryToRaylib::RectangleToRaylib(t2);
		return CheckCollisionRecs(r1, r2);
	}


	// Circle rectangle collision

	template<>
	bool ResolveCollision<Geometry::Rectangle, Geometry::Circle>(const Geometry::Rectangle& t1, const Geometry::Circle& t2)
	{
		const Rectangle r1 = GeometryToRaylib::RectangleToRaylib(t1);
		const Vector2 center{ t2.center.x, t2.center.y };
		return CheckCollisionCircleRec(center, t2.radius, r1);
	}

	template<>
	bool ResolveCollision<Geometry::Circle, Geometry::Rectangle>(const Geometry::Circle& t1, const Geometry::Rectangle& t2)
	{
		return ResolveCollision(t2, t1);
	}


	// Circle Collision
	template<>
	bool ResolveCollision<Geometry::Circle, Geometry::Circle>(const Geometry::Circle& t1, const Geometry::Circle& t2)
	{
		const Vector2 center1{ t1.center.x, t1.center.y };
		const Vector2 center2{ t2.center.x, t2.center.y };
		return CheckCollisionCircles(center1, t1.radius, center2, t2.radius);
	}
}


bool Core::CollisionSolver::Collides(const Geometry::GeometryData& col1, const Geometry::GeometryData& col2)
{
	// This whole function is crap, this should be improved with visitor pattern
	if (std::holds_alternative<Geometry::Circle>(col1))
	{
		auto circle1 = Geometry::GetGeometryData<Geometry::Circle>(col1);
		if (std::holds_alternative<Geometry::Circle>(col2))
		{
			auto circle2 = Geometry::GetGeometryData<Geometry::Circle>(col2);
			return ResolveCollision(circle1, circle2);
		}
		else if (std::holds_alternative<Geometry::Rectangle>(col2))
		{
			auto rect2 = Geometry::GetGeometryData<Geometry::Rectangle>(col2);
			return ResolveCollision(circle1, rect2);
		}
	}
	else
	{
		auto rect1 = Geometry::GetGeometryData<Geometry::Rectangle>(col1);
		if (std::holds_alternative<Geometry::Rectangle>(col2)) {
			auto rect2 = Geometry::GetGeometryData<Geometry::Rectangle>(col2);
			return ResolveCollision(rect1, rect2);
		}
		else
		{
			auto circle2 = Geometry::GetGeometryData<Geometry::Circle>(col2);
			return ResolveCollision(rect1, circle2);
		}
	}

	assert(false && "Non reaching code");
	return false;
}

