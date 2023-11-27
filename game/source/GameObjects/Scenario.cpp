#include "GameObjects/Scenario.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"

GameObject::Scenario::Scenario(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(texture);
	SetupTransform();
}

GameObject::Scenario::Scenario(Core::GameManagers& managers, Utils::ResourceHandle<Resources::Texture>&& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(std::move(texture));
	SetupTransform();
}

void GameObject::Scenario::Draw()
{
	spriteComponent->Render(*transformComponent);
}

void GameObject::Scenario::SetupTransform()
{
	transformComponent = &GetOrAddComponent<Components::Transform>();
	transformComponent->position = { 1920 / 2.f , 1080 / 2.f };
	transformComponent->rotation = 0.f;
	transformComponent->size = { 1920.f, 1080.f };
}
