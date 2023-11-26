#include "GameObjects/Mario.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/GameplayManager.h"
#include "Components/Transform.h"
#include "Components/Atlas.h"
#include "Components/SpriteSheetAnimation.h"
#include <iostream>

GameObject::Mario::Mario(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	auto& resourceManager = gManager.GetManager<ResourceManager>();
	auto texture = resourceManager.GetOrLoad<Resources::Texture>(marioTextureID, marioTexturePath);

	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = pos;
	transform->size = { 200,200 };
	atlasComponent = &GetOrAddComponent<Components::Atlas>(std::move(texture), 6, 3);
	spriteAnimation = &GetOrAddComponent<Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,0 }, Utils::Vector2i{ 5,2 }, 10);
}

void GameObject::Mario::Update(float deltatime)
{
	spriteAnimation->Update();
}

void GameObject::Mario::Draw()
{
	auto& cell = spriteAnimation->GetCurrentSprite();
	atlasComponent->Draw(cell, *transform);
}

