#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include <vector>
#include "GameObjects/Bullet.h"

namespace GameObject {

	class Spaceship : public GameObject
	{
		static constexpr float invulerabilityTime = 3.f; // safe time after being hit
		static constexpr float rotationSpeed = 120.f;
		static constexpr float speed = 200.f;
	public:
		Spaceship(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
		Spaceship(const Spaceship& b);
		Spaceship& operator=(const Spaceship& b);
		~Spaceship();

		Utils::Vector2i GetPosition()const;
		void SetPosition(const Utils::Vector2f& pos);

		void Update(float detltaTime);
		void OnCollision();
		void Draw();

		void StartInvulnerability(float time = invulerabilityTime);
	private:
		void SpawnBullet();
		void RegisterCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager& physics;
		Utils::Vector2f position;
		float rotation = 0.f;
		const Utils::Vector2i size{ 100,100 };
		Collider collider;
		Texture2D* texture;
		float invulnerableTime;
		std::vector<Bullet> bullets;

	};
}