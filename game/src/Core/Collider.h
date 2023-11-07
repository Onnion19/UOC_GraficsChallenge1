#pragma once 

#include "Utils/Geometry.h"
namespace Core
{
	class PhysicsManager;
}
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


using ColliderId = unsigned int;
using ConstColliderId = const ColliderId;
constexpr ColliderId InvalidCollider = std::numeric_limits<ColliderId>::max();

namespace Internal{

	// Struct to hold collider internal data as type erasure so any object can receive collision events.
	// This will be stored inside the physics manager, which will own all the data.
	struct _InternalCollider {
		using ColliderBounds = Geometry::GeometryData;
		
		/**
		* Constructor taking a callback OnCollision()
		*/
		template<ColliderShape Shape, ColliderCallback T>
		_InternalCollider(Shape&& shape, T& t)
			: _ptr(&t)
			, bounds(std::move(shape)) {
			_OnCollission = [](void* data) {static_cast<T*>(data)->OnCollision(); };
		}

		/**
		* Constructor without callback
		*/
		template<ColliderShape Shape>
		_InternalCollider(Shape&& shape)
			: _ptr(nullptr)
			, bounds(std::move(shape))
			, _OnCollission(nullptr) {}


		void OnCollision() { if (_ptr)_OnCollission(_ptr); }

		/*
			RAW DATA, CAUTION HERE
		*/
		// Pointer to the object callback. can be null if no callback is registered.
		void* _ptr;

		// Collider geometry used to compute physics.
		ColliderBounds bounds;

		// Type erasure lambda.
		void (*_OnCollission)(void* ptr);

		friend class Collider;
	};

	struct ColliderIdFactory {
		ConstColliderId operator()() { return generate(); }
		ConstColliderId generate() { return ++id; }
	private:
		ColliderId id = 0;
	};
}

/**
* Public collider class, it's a safe wrapper of the internal data used by the "engine".
* Collider has as a raw pointer the internals, this is dangerous and probably can be improved.
*/
class Collider
{
public:
	Collider() : internal_collider(nullptr), id(InvalidCollider) {}
	ConstColliderId GetId() const { return id; }
	template<ColliderShape Shape>
	void UpdateColliderBounds(Shape&& bound) { assert(Valid()); internal_collider->bounds = bound; }
	bool Valid()const { return id != InvalidCollider; }
private:
	Collider(Internal::_InternalCollider& col, ConstColliderId i) : internal_collider(&col), id(i) {}

	Internal::_InternalCollider* internal_collider;
	ColliderId id;
	friend Core::PhysicsManager;
};