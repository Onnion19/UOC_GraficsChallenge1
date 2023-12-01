#include "Components/MovementComponents.h"
#include "Components/Transform.h"
#include "Utils/Geometry.h"

void Components::MRU::Update(float deltaTime, Transform& t)
{
	t.position += speed * deltaTime;
}

void Components::CyclicMRU::Update(float deltaTime, Transform& t)
{
	angle += speed * deltaTime;
	angle = static_cast<int>(angle) % 360;
	auto factor = std::cos(angle * Geometry::DEG_TO_RAD);
	t.position = initialPosition + distance * factor;
}
