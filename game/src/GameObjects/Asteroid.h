#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
namespace GameObject
{
	struct AsteroidTransform {
		Utils::Vector2f position;
		Utils::Vector2i size;
		Utils::Vector2f movement;
	};

	class Asteroid : public GameObject {
	public:
		Asteroid(Core::GameManagers& manager, const AsteroidTransform& trans);
		~Asteroid();

		void Update(float deltaTime);

		void Draw();

		void OnCollision();
	private:

		void RegisterCollider();
	private:
		AsteroidTransform transform;
		Texture2D* texture;
		Collider colider;
		int health = 3;
	};
}