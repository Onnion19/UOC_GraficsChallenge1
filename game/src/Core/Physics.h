#pragma once

#include "raylib.h"
#include "Utils/Vector2.hpp"
#include "Utils/Geometry.h"
#include <vector>
#include <limits>
#include <unordered_map>

namespace Core {

	// Types and constants definitions
	using Position = Utils::Vector2f;
	using Speed = Utils::Vector2f;
	using ColliderId = unsigned int;
	using ConstColliderId = const ColliderId;
	constexpr ColliderId InvalidCollider = std::numeric_limits<ColliderId>::max();

	// any class Implementing OnCollision() can be a callback listener
	template<typename T>
	concept ColliderCallback = requires (T t) {
		t.OnCollision();
	};


	// Collision valid shapes: Point, Line, Circle, Rectangle
	template<typename T>
	concept ColliderShape =
		std::is_same_v<T, Geometry::Point> ||
		std::is_same_v<T, Geometry::Line> ||
		std::is_same_v<T, Geometry::Circle> ||
		std::is_same_v<T, Geometry::Rectangle>;

	class PhysicsManager {
	public:

		template<ColliderShape Shape, typename ... Args>
		ConstColliderId RegisterCollider(Args&& ... args);

		template<ColliderShape Shape, ColliderCallback T, typename ... Args>
		ConstColliderId RegisterCollider(T& listener, Args&& ... args);

		void UnregisterCollider(ColliderId id);

		bool CheckCollisionOnCollider(ColliderId id);

	private:

		// Internals structs
		using ColliderBounds = Geometry::GeometryData;

		// Struct to hold collider internal data as type erasure so any object can receive collision events.
		struct Collider {

			template<ColliderShape Shape, ColliderCallback T>
			Collider(Shape&& shape, T& t)
				: _ptr(&t)
				, collider(std::move(shape)) {
				_OnCollission = [](void* data) {static_cast<T*>(data)->OnCollision(); };
			}

			template<ColliderShape Shape>
			Collider(Shape&& shape)
				: _ptr(nullptr)
				, collider(std::move(shape))
				, _OnCollission(nullptr) {}


			const ColliderBounds& GetBounds() const { return collider; }

			void OnCollision() { if (_ptr)_OnCollission(_ptr); }

		private:
			void* _ptr;
			const ColliderBounds collider;
			void (*_OnCollission)(void* ptr);
		};

		struct ColliderIdFactory {
			ConstColliderId operator()() { return generate(); }
			ConstColliderId generate() { return ++id; }
		private:
			ColliderId id = 0;
		};

	private:

		// This is expensive, a flat hash map would be better.
		// Also a vector will be faster to iterate per frame but slighly slower to search by id. Probably worth the change.
		std::unordered_map<ColliderId, Collider> colliders;
		ColliderIdFactory idFactory{};

	};

	template<ColliderShape Shape, typename ...Args>
	inline ConstColliderId PhysicsManager::RegisterCollider(Args && ...args)
	{
		auto id = idFactory();
		colliders.emplace(std::make_pair(id, Shape{ std::forward<Args>(args)... }));
		return id;

	}

	template<ColliderShape Shape, ColliderCallback T, typename ...Args>
	inline ConstColliderId PhysicsManager::RegisterCollider(T& listener, Args && ...args)
	{
		auto id = idFactory();
		Shape s{ std::forward<Args>(args)... };
		colliders.emplace(std::make_pair(id, Collider{ std::move(s), listener}));
		return id;
	}

}