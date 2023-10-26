#pragma once

#include "raylib.h"
#include "Utils/Vector2.hpp"
#include "Utils/Geometry.h"
#include <vector>
#include <limits>
#include <unordered_map>
#include "Core/Collider.h"

namespace Core {

	// Types and constants definitions
	using Position = Utils::Vector2f;
	using Speed = Utils::Vector2f;

	class PhysicsManager {
		// Internals structs
		using ColliderBounds = Geometry::GeometryData;
	public:

		template<ColliderShape Shape, typename ... Args>
		Collider RegisterCollider(Args&& ... args);

		template<ColliderShape Shape, ColliderCallback T, typename ... Args>
		Collider RegisterCollider(T& listener, Args&& ... args);

		void UnregisterCollider(Collider& id);

		bool CheckCollisionOnCollider(const Collider& id);
	private:

		// This is expensive, a flat hash map would be better.
		// Also a vector will be faster to iterate per frame but slighly slower to search by id. Probably worth the change.
		std::unordered_map<ColliderId, ::Internal::_InternalCollider> colliders;
		::Internal::ColliderIdFactory idFactory{};
	};


	// Template functions implementation

	template<ColliderShape Shape, typename ...Args>
	inline Collider PhysicsManager::RegisterCollider(Args && ...args)
	{
		auto id = idFactory();
		auto result = colliders.emplace(std::make_pair(id, Shape{ std::forward<Args>(args)... }));
		return {result.first->second, id};

	}

	template<ColliderShape Shape, ColliderCallback T, typename ...Args>
	inline Collider PhysicsManager::RegisterCollider(T& listener, Args && ...args)
	{
		auto id = idFactory();
		Shape s{ std::forward<Args>(args)... };
		auto result = colliders.emplace(std::make_pair(id, ::Internal::_InternalCollider{ std::move(s), listener}));
		return {result.first->second, id };
	}

}