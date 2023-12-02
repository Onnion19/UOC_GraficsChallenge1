#include "GameObjects/PowerUp.h"
#include "Components/Atlas.h"
#include "Components/Transform.h"
#include "Core/Physics.h"

GameObject::PowerUp::PowerUp(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position)
	: GameObject(managers)
{
	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = position;
	transform->size = { 50,50 };

	atlas = &GetOrAddComponent<Components::Atlas>(texture, atlasSize.x, atlasSize.y);
	atlasCell = cell;
	RegisterCollider();
}

GameObject::PowerUp::~PowerUp() {
	UnregisterCollider();
	RemoveComponent< Components::Atlas>();
	RemoveComponent< Components::Transform>();

}

void GameObject::PowerUp::Draw() const
{
	atlas->Draw(atlasCell, *transform);
}

void GameObject::PowerUp::RegisterCollider()
{
	collider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Circle>(this, transform->position, transform->size.x / 2.f);
	SetTag("PowerUp");
}

void GameObject::PowerUp::UnregisterCollider()
{
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(collider);
}
