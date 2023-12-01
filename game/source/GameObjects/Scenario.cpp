#include "GameObjects/Scenario.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Core/WindowManager.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Stair.h"

GameObject::Scenario::Scenario(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(texture);
	colliders.reserve(30);
	SetupTransform();
	RegisterWalls();
	RegisterStairs();
}

GameObject::Scenario::Scenario(Core::GameManagers& managers, Utils::ResourceHandle<Resources::Texture>&& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(std::move(texture));
	SetupTransform();
	RegisterWalls();
	RegisterStairs();
}

GameObject::Scenario::~Scenario()
{
	RemoveComponent<Components::Sprite>();
	RemoveComponent<Components::Transform>();
	colliders.clear();
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
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.500f,0.98440f }, Utils::Vector2f{ 1.f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.5000f,0.82810f }, Utils::Vector2f{ .9286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.125f,0.67190f }, Utils::Vector2f{ .1786f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.4821f,0.6719f }, Utils::Vector2f{ .3929f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.8571f,0.6719f }, Utils::Vector2f{ .2143f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.21430f,0.5156f }, Utils::Vector2f{ .4286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.78570f,0.5156f }, Utils::Vector2f{ .4286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.500f,0.35940f }, Utils::Vector2f{ .9286f,0.0313f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Wall>(Utils::Vector2f{ 0.500f,0.23440f }, Utils::Vector2f{ .2143f,0.0313f }));

}

void GameObject::Scenario::RegisterStairs()
{
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.1250f, 0.9063f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.3750f, 0.9063f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.6250f, 0.9063f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.8750f, 0.9063f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.3036f, 0.7500f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.6607f, 0.7500f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.1250f, 0.5938f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.3750f, 0.5938f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.6250f, 0.5938f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.8750f, 0.5938f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.0893f, 0.4375f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.9107f, 0.4375f }, Utils::Vector2f{ 0.0357f, 0.1250f }));
	colliders.emplace_back(GameObjectFactory::MakePureGameObjectHandle<Stair>(Utils::Vector2f{ 0.5893f, 0.2969f }, Utils::Vector2f{ 0.0357f, 0.0938f }));
}
