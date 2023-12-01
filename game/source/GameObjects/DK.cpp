#include "GameObjects/DK.h"
#include "Components/MovementComponents.h"
#include "Components/Transform.h"

GameObject::DK::DK(Core::GameManagers& managers, const Utils::Vector2f& initialPosition, float distance, float speed) : GameObject(managers)
{
	transform = &GetOrAddComponent<Components::Transform>();
	transform->size = { 30.f, 30.f };
	mru = &GetOrAddComponent<Components::CyclicMRU>();
	mru->initialPosition = initialPosition;
	mru->distance.x = distance;
	mru->speed = speed;
	manager = &managers.GetManager<Core::PhysicsManager>();
	SetTag("Enemy");
	RegisterCollider();
}

GameObject::DK::~DK()
{
	RemoveComponent<Components::Transform>();
	RemoveComponent<Components::CyclicMRU>();
	UnregisterCollider();
}

void GameObject::DK::Update(float deltaTime)
{
	UpdateCollider();
	mru->Update(deltaTime, *transform);
}

void GameObject::DK::Draw() const
{
	DrawCircle(transform->position.x, transform->position.y, transform->size.x, YELLOW);
}

void GameObject::DK::RegisterCollider()
{
	collider = manager->RegisterCollider<Geometry::Circle>(this, transform->position, transform->size.x);
}

void GameObject::DK::UpdateCollider()
{
	collider.UpdateColliderBounds(Geometry::Circle{ transform->position, transform->size.x });
}

void GameObject::DK::UnregisterCollider()
{
	manager->UnregisterCollider(collider);
}
