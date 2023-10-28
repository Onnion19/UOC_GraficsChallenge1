#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
namespace GameObject
{
	struct AsteroidTransform {
		Utils::Vector2f position = { 0.f,0.f };
		Utils::Vector2i size = { 20,20 };
		Utils::Vector2f movement{ 15.f, 54.f };
	};

	class Asteroid : public GameObject {
		static constexpr int InitialHP = 3;
	public:
		Asteroid(Core::GameManagers& manager, const AsteroidTransform& trans);
		Asteroid(const Asteroid& other);
		Asteroid& operator=(const Asteroid& other);
		~Asteroid();

		void Update(float deltaTime);
		void Draw();
		bool Valid()const;
		void OnCollision();
	private:
		void RegisterCollider();
		void UnregisterCollider();
	private:
		AsteroidTransform transform;
		Texture2D* texture;
		Collider colider;
		int health = InitialHP;
	};
}