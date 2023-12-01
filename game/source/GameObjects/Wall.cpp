#include "GameObjects/Wall.h"
#include "Core/WindowManager.h"
#include "Core/Physics.h"
#include "Components/WallComponent.h"

GameObject::Wall::Wall(Core::GameManagers& managers, const Utils::Vector2f& pos, const Utils::Vector2f& size) : GameObject(managers)
{
	RegisterCollider(pos, size);
	GetOrAddComponent<Components::WallComponent>(collider);
}

GameObject::Wall::~Wall()
{
	UnregisterCollider();
	RemoveComponent< Components::WallComponent>();
}

void GameObject::Wall::RegisterCollider(const Utils::Vector2f& pos, const Utils::Vector2f& size)
{
	auto screenSize = gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	Utils::Vector2f Size{ screenSize.x * size.x, screenSize.y * size.y };
	Utils::Vector2f Center{ screenSize.x * pos.x, screenSize.y * pos.y };
	Geometry::Point topLeft = Center - (Size * 0.5f);
	Geometry::Point botRight = Center + (Size * 0.5f);

	Geometry::Rectangle shape{ topLeft, botRight };
	collider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(this, topLeft, botRight);
}

void GameObject::Wall::UnregisterCollider()
{
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(collider);
}
