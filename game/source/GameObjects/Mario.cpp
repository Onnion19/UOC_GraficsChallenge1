#include "GameObjects/Mario.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/GameplayManager.h"
#include "Components/Transform.h"
#include "Components/Atlas.h"
#include "Components/SpriteSheetAnimation.h"
#include "Components/WallComponent.h"




GameObject::Mario::Mario(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	auto& resourceManager = gManager.GetManager<ResourceManager>();
	auto texture = resourceManager.GetOrLoad<Resources::Texture>(marioTextureID, marioTexturePath);

	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = pos;
	transform->size = { 50,50 };
	atlasComponent = &GetOrAddComponent<Components::Atlas>(std::move(texture), 6, 3);
	spriteAnimation = &GetOrAddComponent<Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,0 }, Utils::Vector2i{ 5,2 }, 10);
	RegisterCollider();
}

const Utils::Vector2f& GameObject::Mario::GetPosition() const
{
	return transform->position;
}

void GameObject::Mario::SetPosition(const Utils::Vector2f& pos)
{
	transform->position.x = std::clamp(pos.x, 0.f, 1920.f);
	transform->position.y = std::clamp(pos.y, 0.f, 1800.f);

}

void GameObject::Mario::Update(float deltatime)
{
	physics.CheckCollisionOnCollider(collider);
	if (IsKeyDown(KEY_D))
	{
		SetPosition(transform->position + Utils::Vector2f{ 100 * deltatime, 0 });
	}
	else if (IsKeyDown(KEY_A))
	{
		SetPosition(transform->position - Utils::Vector2f{ 100 * deltatime, 0 });
	}

	if (!collidedThisFrame)
	{
		transform->position.y += 5;
	}

	spriteAnimation->Update();
	UpdateCollider();
}

void GameObject::Mario::OnCollision(GameObject* other)
{
	if (!other)return; 

	if (auto wall = other->GetComponent<Components::WallComponent>())
	{
		transform->position.y = wall->GetSurfacePos() - transform->size.y/2;
		collidedThisFrame = true;
	}
}

void GameObject::Mario::Draw()
{
	auto& cell = spriteAnimation->GetCurrentSprite();
	atlasComponent->Draw(cell, *transform);
}

void GameObject::Mario::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Circle>(*this, this, transform->position, transform->size.x / 2);
}

void GameObject::Mario::UpdateCollider()
{
	collider.UpdateColliderBounds(Geometry::Circle{ transform->position, transform->size.x / 2.5f });
}

void GameObject::Mario::UnregisterCollider()
{
	physics.UnregisterCollider(collider);
}

