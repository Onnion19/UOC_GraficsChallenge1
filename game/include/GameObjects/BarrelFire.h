#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"
namespace GameObject
{
	struct AsteroidTransform {
		Utils::Vector2f position = { 0.f,0.f };
		Utils::Vector2i size = { 20,20 };
		Utils::Vector2f movement{ 15.f, 54.f };
	};

	class BarrelFire : public GameObject {};
}