#include "raylib.h"
#include "Ball.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"

GameObject::Ball::Ball(Core::GameManagers& manager, const BallProperties& prop) : GameObject(manager), properties(prop), collider()
{
	RegisterCollider();
}

GameObject::Ball::Ball(const Ball& b)
	: GameObject(b.gManager)
	, properties(b.properties)
	, position(b.position)
{
	RegisterCollider();
}

GameObject::Ball& GameObject::Ball::operator=(const Ball& b)
{
	properties = b.properties;
	position = b.position;
	gManager = b.gManager;
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

Utils::Vector2i& GameObject::Ball::GetPosition()
{
	return position;
}

void GameObject::Ball::SetPosition(const Utils::Vector2i& pos)
{
	position = pos;
	collider.UpdateColliderBounds(Geometry::Circle{ position, properties.radius });
}

void GameObject::Ball::Draw()
{
	DrawCircle(position.x, position.y, properties.radius, properties.color);
}

void GameObject::Ball::RegisterCollider()
{
	auto& physicsManager = gManager.GetManager<Core::PhysicsManager>();
	collider = physicsManager.RegisterCollider<Geometry::Circle>(position, properties.radius);
}

void GameObject::Ball::UnregisterCollider()
{
	if (!collider.Valid()) return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(collider);
}
