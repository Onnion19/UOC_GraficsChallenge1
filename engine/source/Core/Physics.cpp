#include "Core/Physics.h"
#include "Core/CollisionSolver.h"


void Core::PhysicsManager::UnregisterCollider(Collider& col)
{
	collidersToErase.emplace_back(col.id);
	col = {};
}


bool Core::PhysicsManager::CheckCollisionOnCollider(const Collider& collider)
{
	// Remove Invalid colliders
	std::for_each(collidersToErase.rbegin(), collidersToErase.rend(), [&](unsigned int id) { colliders.erase(id); });
	collidersToErase.clear();

	bool collides = false;
	for (auto&& [colliderId, col] : colliders)
	{
		// no self collision
		if (colliderId == collider.id)continue;

		const bool hasCollided = CollisionSolver::Collides(collider.internal_collider->bounds, col.bounds);
		if (hasCollided)
		{
			GameObject::GameObject* checkedGameObject = collider.internal_collider->owner;
			GameObject::GameObject* collidedGameObject = col.owner;
			col.OnCollision(checkedGameObject);
			collider.internal_collider->OnCollision(collidedGameObject);
			collides = true;
		}
	}


	return collides;

}

#if DEBUG
#include "Utils/TypeConversion.hpp"
void Core::PhysicsManager::DrawDebugColliders()
{
	for (auto&& [colliderId, col] : colliders)
	{
		if (std::holds_alternative<Geometry::Circle>(col.bounds))
		{
			auto circle = Geometry::GetGeometryData<Geometry::Circle>(col.bounds);
			DrawCircle(static_cast<int>(circle.center.x), static_cast<int>(circle.center.y), circle.radius, col.debugColor);
		}
		else if (std::holds_alternative<Geometry::Rectangle>(col.bounds))
		{
			auto rectangle = Geometry::GetGeometryData<Geometry::Rectangle>(col.bounds);
			auto raylibRectangle = GeometryToRaylib::RectangleToRaylib(rectangle);
			DrawRectangle(static_cast<int>(raylibRectangle.x), static_cast<int>(raylibRectangle.y), static_cast<int>(raylibRectangle.width), static_cast<int>(raylibRectangle.height), col.debugColor);
		}
	}
}
#endif
