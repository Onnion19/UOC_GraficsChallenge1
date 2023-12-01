#include "Components/StairComponent.h"
#include "Utils/TypeConversion.hpp"
#include "Core/Collider.h"


Components::StairComponent::StairComponent(const Collider& collider)
{
	auto colliderBounds = collider.GetBounds();
	if (!colliderBounds) return;
	auto colliderShape = Geometry::GetGeometryData<Geometry::Rectangle>(*colliderBounds);
	Rectangle r = GeometryToRaylib::RectangleToRaylib(colliderShape);
	stairPos = static_cast<int>(r.x + r.width/2.f);
}

int Components::StairComponent::GetStairPos() const
{
	return stairPos;
}
