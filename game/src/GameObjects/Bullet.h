#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
namespace GameObject
{
	struct BulletTransform {
		Utils::Vector2f position;
		Utils::Vector2i size;
		Utils::Vector2f movement;
	};

	class Bullet : public GameObject {
	public:
		Bullet(Core::GameManagers& manager, const BulletTransform& trans);
		~Bullet();

		void Update(float deltaTime);

		void Draw();

		void OnCollision();
	private:

		void RegisterCollider();
	private:
		BulletTransform transform;
		Texture2D texture;
		Collider colider;
	};
}