#include "raylib.h"
#include "Ball.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"

GameObject::Ball::Ball(Core::GameManagers& manager, const BallProperties& prop) : GameObject(manager), properties(prop), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	RegisterCollider();
}

GameObject::Ball::Ball(const Ball& b)
	: GameObject(b.gManager)
	, properties(b.properties)
	, position(b.position)
	, physics(gManager.GetManager<Core::PhysicsManager>())
{
	RegisterCollider();
}

GameObject::Ball& GameObject::Ball::operator=(const Ball& b)
{
	properties = b.properties;
	position = b.position;
	gManager = b.gManager;
	physics = gManager.GetManager<Core::PhysicsManager>();
	RegisterCollider();

	return *this;
}

GameObject::Ball::~Ball()
{
	UnregisterCollider();
}

const Utils::Vector2i& GameObject::Ball::GetPosition() const
{
	return position;
}


void GameObject::Ball::SetPosition(const Utils::Vector2i& pos)
{
	collider.UpdateColliderBounds(Geometry::Circle{ pos, properties.radius });

	if (physics.CheckCollisionOnCollider(collider))
	{
		collider.UpdateColliderBounds(Geometry::Circle{ position, properties.radius });
		return;
	}

	position = pos;
}

void GameObject::Ball::Draw()
{
	DrawCircle(position.x, position.y, properties.radius, properties.color);
}

void GameObject::Ball::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Circle>(position, properties.radius);
}

void GameObject::Ball::UnregisterCollider()
{
	if (!collider.Valid()) return;
	physics.UnregisterCollider(collider);
}
