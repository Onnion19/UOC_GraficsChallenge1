#include "GameObjects/PowerUp.h"
#include "Scenes/SceneManager.h"

 GameObject::InteractiveItem::InteractiveItem(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position, const Utils::Vector2f& size)
	: GameObject(managers)
{
	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = position;
	transform->size = size;

	atlas = &GetOrAddComponent<Components::Atlas>(texture, atlasSize.x, atlasSize.y);
	atlasCell = cell;
	RegisterCollider();
}


 GameObject::InteractiveItem::~InteractiveItem()
{
	UnregisterCollider();
	RemoveComponent< Components::Atlas>();
	RemoveComponent< Components::Transform>();
}

 void GameObject::InteractiveItem::Draw() const
{
	if (!collider.Valid()) return;
	atlas->Draw(atlasCell, *transform);
}

 void GameObject::InteractiveItem::RegisterCollider()
{
	collider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Circle>(*this, this, transform->position, transform->size.x / 2.f);
	SetTag("PowerUp");
}

 void GameObject::InteractiveItem::UnregisterCollider()
{
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(collider);
}


GameObject::PowerUp::PowerUp(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position, const Utils::Vector2f& size)
	: InteractiveItem(managers, texture, atlasSize, cell, position, size)
{}


void GameObject::PowerUp::OnCollision(GameObject* other)
{
	assert(other && other->GetTag() == Core::Tag{ "Player" });
	UnregisterCollider();
	gManager.GetManager<GameplayManager>().UpdateScore(300);
}

GameObject::Peach::Peach(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position, const Utils::Vector2f& size)
	: InteractiveItem(managers, texture, atlasSize, cell, position, size) {}

void GameObject::Peach::OnCollision(GameObject* other)
{
	assert(other && other->GetTag() == Core::Tag{ "Player" });
	UnregisterCollider();
	gManager.GetManager<GameplayManager>().EndGame(true);
}
