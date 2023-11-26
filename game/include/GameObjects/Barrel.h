#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"
namespace GameObject
{

	class Barrel : public GameObject {
		static constexpr int InitialHP = 3;
		inline static const Core::Tag AsteroidTag = Core::Tag{ "Barrel" };
		inline static const ResourceID asteroidTextureID{ "asteroid" };
	public:
		Barrel(Core::GameManagers& manager, Utils::Vector2f position, int size, Utils::Vector2f movement);
		Barrel(const Barrel& other);
		Barrel& operator=(const Barrel& other);
		~Barrel();
	};
}