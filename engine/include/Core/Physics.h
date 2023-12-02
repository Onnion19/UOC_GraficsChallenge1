#pragma once

#include "raylib.h"
#include "Utils/Vector2.hpp"
#include <vector>
#include <limits>
#include <unordered_map>
#include "Core/Collider.h"

namespace GameObject {
	class GameObject;
}

namespace Core {

	// Types and constants definitions
	using Position = Utils::Vector2f;
	using Speed = Utils::Vector2f;

	class PhysicsManager {
		// Internals structs
		using ColliderBounds = Geometry::GeometryData;
	public:
		// Add a collider to the registry.
		template<ColliderShape Shape, typename ... Args>
		Collider RegisterCollider(GameObject::GameObject* owner, Args&& ... args);

		// Add a collider with a listener when it collides to the registry.
		template<ColliderShape Shape, ColliderCallback T, typename ... Args>
		Collider RegisterCollider(T& listener, GameObject::GameObject* owner, Args&& ... args);

		// Unregister the collider starting on the next frame
		void UnregisterCollider(Collider& id);
		// Verifies if the collider is colliding with any other inside the registry.
		bool CheckCollisionOnCollider(const Collider& id);

#if DEBUG
		void DrawDebugColliders();
#endif
	private:
		// Removes collider from the registry and invalidates the data.
		void UnregisterColliderInternal(Collider& id);
		// This is expensive, a flat hash map would be better.
		// Also a vector will be faster to iterate per frame but slighly slower to search by id. Probably worth the change.
		std::unordered_map<ColliderId, ::Internal::_InternalCollider> colliders;
		std::vector< Collider*> collidersToErase;
		::Internal::ColliderIdFactory idFactory{};
	};


	// Template functions implementation

	template<ColliderShape Shape, typename ...Args>
	inline Collider PhysicsManager::RegisterCollider(GameObject::GameObject* owner, Args && ...args)
	{
		auto id = idFactory();
		Shape s{ std::forward<Args>(args)... };
		auto result = colliders.emplace(std::make_pair(id, ::Internal::_InternalCollider{std::move(s), owner}));
		return {result.first->second, id};
	}

	template<ColliderShape Shape, ColliderCallback T, typename ...Args>
	inline Collider PhysicsManager::RegisterCollider(T& listener, GameObject::GameObject* owner, Args && ...args)
	{
		auto id = idFactory();
		Shape s{ std::forward<Args>(args)... };
		auto result = colliders.emplace(std::make_pair(id, ::Internal::_InternalCollider{ std::move(s), listener, owner}));
		return {result.first->second, id };
	}

}