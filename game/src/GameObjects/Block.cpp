#include "GameObjects/Block.h"
#include "raylib.h"
#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/TypeConversion.hpp"

GameObject::Block::Block(Core::GameManagers& manager, const BlockTransform& trans)
	: GameObject(manager)
	, transform(trans)
{
	RegisterCollider();
	texture = manager.GetManager<ResourceManager>().GetOrLoad<Texture>(ResourceID("Example"), "resources/Block.png");
}

GameObject::Block::~Block()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Block::Draw()
{
	if (!colider.Valid()) return;
	DrawRectangle(transform.position.x, transform.position.y, transform.size.x, transform.size.y, BLUE);
	//DrawTexture(texture, transform.position.x, transform.position.y, WHITE);
	//const Rectangle rec = 
	//DrawTextureRec(texture, rec, Vector2{ static_cast<float>(transform.position.x), static_cast<float>(transform.position.y) }, WHITE);
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
