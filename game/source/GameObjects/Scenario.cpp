#include "GameObjects/Scenario.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Core/WindowManager.h"
#include "GameObjects/Wall.h"

GameObject::Scenario::Scenario(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(texture);
	colliders.reserve(30);
	SetupTransform();
	RegisterWalls();
}

GameObject::Scenario::Scenario(Core::GameManagers& managers, Utils::ResourceHandle<Resources::Texture>&& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(std::move(texture));
	SetupTransform();
	RegisterWalls();
}

void GameObject::Scenario::Draw()
{
	spriteComponent->Render(*transformComponent);
}

void GameObject::Scenario::SetupTransform()
{
	auto screensize = gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	transformComponent = &GetOrAddComponent<Components::Transform>();
	transformComponent->position = { screensize.x / 2.f , screensize.y / 2.f };
	transformComponent->rotation = 0.f;
	transformComponent->size = { static_cast<float>(screensize.x), static_cast<float>(screensize.y) };
}

void GameObject::Scenario::RegisterWalls()
{
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.500f,0.98440f  }, Utils::Vector2f{ 1.f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.5000f,0.82810f }, Utils::Vector2f{ .9286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.125f,0.67190f  }, Utils::Vector2f{ .1786f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.4821f,0.6719f  }, Utils::Vector2f{ .3929f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.8571f,0.6719f  }, Utils::Vector2f{ .2143f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.21430f,0.5156f }, Utils::Vector2f{ .4286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.78570f,0.5156f }, Utils::Vector2f{ .4286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.500f,0.35940f  }, Utils::Vector2f{ .9286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{0.500f,0.23440f  }, Utils::Vector2f{ .2143f,0.0313f }));

}

void GameObject::Scenario::RegisterStairs()
{
}
