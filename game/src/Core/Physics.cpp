#include "Physics.h"
#include "Core/CollisionSolver.h"

void Core::PhysicsManager::UnregisterCollider(Collider& col)
{
	colliders.erase(col.id);
	col = {};
}

bool Core::PhysicsManager::CheckCollisionOnCollider(const Collider& collider)
{
	// Only 1 colision algorithm
	for (auto&& [colliderId, col] : colliders)
	{
		// no self collision
		if (colliderId == collider.id)continue;

		const bool hasCollided = CollisionSolver::Collides(collider.internal_collider->bounds, col.bounds);
		if (hasCollided)
		{
			col.OnCollision();
			collider.internal_collider->OnCollision();
			return true;
		}
	}

	return false;

}
