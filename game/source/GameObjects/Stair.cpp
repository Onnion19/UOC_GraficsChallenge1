#include "GameObjects/Stair.h"
#include "Core/WindowManager.h"
#include "Core/Physics.h"
#include "Components/StairComponent.h"

GameObject::Stair::Stair(Core::GameManagers& managers, const Utils::Vector2f& pos, const Utils::Vector2f& size) : GameObject(managers)
{
	RegisterCollider(pos, size);
	GetOrAddComponent<Components::StairComponent>(collider);
}

GameObject::Stair::~Stair()
{
	RemoveComponent<Components::StairComponent>();
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(collider);
}

void GameObject::Stair::RegisterCollider(const Utils::Vector2f& pos, const Utils::Vector2f& size)
{
	auto screenSize = gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	Utils::Vector2f Size{ screenSize.x * size.x, screenSize.y * size.y };
	Utils::Vector2f Center{ screenSize.x * pos.x, screenSize.y * pos.y };
	Geometry::Point topLeft = Center - (Size * 0.5f);
	Geometry::Point botRight = Center + (Size * 0.5f);

	Geometry::Rectangle shape{ topLeft, botRight };
	collider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(this, topLeft, botRight);
}
