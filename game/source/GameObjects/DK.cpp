#include "GameObjects/DK.h"
#include "Components/MovementComponents.h"
#include "Components/Transform.h"
#include "Components/SpriteSheetAnimation.h"
#include "Core/Physics.h"
#include "Resources/Texture.h"
#include "Core/WindowManager.h"

GameObject::DK::DK(Core::GameManagers& managers, const Utils::Vector2f& initialPosition, float distance, float speed) : GameObject(managers)
{
	auto size = gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize().x * 0.0677f;
	transform = &GetOrAddComponent<Components::Transform>();
	transform->size = { size,size };

	mru = &GetOrAddComponent<Components::CyclicMRU>();
	mru->initialPosition = initialPosition;
	mru->distance.x = distance;
	mru->speed = speed;

	manager = &managers.GetManager<Core::PhysicsManager>();

	auto texture = gManager.GetManager<ResourceManager>().GetOrLoad<Texture>(DK_AtlasID, DK_AtlasPath);
	auto atlasComponent = &GetOrAddComponent<Components::Atlas>(std::move(texture), DK_AtlasSize.x, DK_AtlasSize.y);
	animation = &GetOrAddComponent<Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,0 }, DK_AtlasSize - Utils::Vector2i{ 1,1 }, 15);
	SetTag("DK");
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
	animation->Update();
}

void GameObject::DK::Draw() const
{
	animation->Draw(*transform);
}

void GameObject::DK::RegisterCollider()
{
	collider = manager->RegisterCollider<Geometry::Circle>(this, transform->position, transform->size.x / 2.f);
}

void GameObject::DK::UpdateCollider()
{
	collider.UpdateColliderBounds(Geometry::Circle{ transform->position, transform->size.x / 2.f });
}

void GameObject::DK::UnregisterCollider()
{
	manager->UnregisterCollider(collider);
}
