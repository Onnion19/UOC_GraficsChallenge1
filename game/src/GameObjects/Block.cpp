#include "GameObjects/Block.h"
#include "raylib.h"
#include "Resources/ResourceManager.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"

GameObject::Block::Block(Core::GameManagers& manager, const BlockTransform& trans)
	: GameObject(manager)
	, transform(trans)
{
	RegisterCollider();
}

GameObject::Block::~Block()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Block::Draw()
{
	if (!colider.Valid()) return;
	DrawRectangle(transform.position.x, transform.position.y, transform.size.x, transform.size.y, RED);
}

void GameObject::Block::OnCollision()
{
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Block::RegisterCollider()
{
	auto bot_right = transform.position + transform.size;
	colider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(*this, Geometry::Point{ transform.position }, Geometry::Point{ bot_right });
}
