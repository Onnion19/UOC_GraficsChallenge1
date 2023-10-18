#include "Physics.h"
#include "Core/CollisionSolver.h"

void Core::PhysicsManager::UnregisterCollider(ColliderId id)
{
	colliders.erase(id);
}

bool Core::PhysicsManager::CheckCollisionOnCollider(ColliderId id)
{
	auto iter = colliders.find(id);
	if (iter == colliders.end()) return false;
	auto collider = iter->second;

	// Only 1 colision algorithm
	for (auto&& [colliderId, col] : colliders)
	{
		// no self collision
		if (colliderId == id)continue;

		const bool hasCollided = CollisionSolver::Collides(collider.GetBounds(), col.GetBounds());
		if (hasCollided)
		{
			col.OnCollision();
			collider.OnCollision();
			return true;
		}
	}

	return false;

}
