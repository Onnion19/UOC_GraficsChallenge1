#include "Components/WallComponent.h"
#include "Core/Collider.h"



Components::WallComponent::WallComponent(const Collider& collider)
{
	auto colliderBounds = collider.GetBounds();
	if (!colliderBounds) return;
	auto colliderShape = Geometry::GetGeometryData<Geometry::Rectangle>(*colliderBounds);

	surfacePosY = static_cast<int>(colliderShape.topLeft.y);
}

int Components::WallComponent::GetSurfacePos() const
{
	return surfacePosY;
}
