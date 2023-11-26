#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include <vector>
#include "GameObjects/Bullet.h"
#include "Resources/Texture.h"
#include "Resources/music.h"

namespace GameObject {

	class Spaceship : public GameObject
	{
		static constexpr float invulerabilityTime = 3.f; // safe time after being hit
		static constexpr float rotationSpeed = 120.f;
		static constexpr float speed = 200.f;
		static constexpr auto spaceshipTexturePath{ "resources/spaceship.png" };
		static constexpr auto shootingSoundPath{ "resources/shoot.wav" };
	public:
		inline static const ResourceID spaceshipTextureID{ "SpaceshipTexture" };
		inline static const ResourceID shootingSoundID{ "ShootingSound" };

		Spaceship(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
		Spaceship(const Spaceship& b);
		Spaceship& operator=(const Spaceship& b);
		~Spaceship();

		Utils::Vector2i GetPosition()const;
		void SetPosition(const Utils::Vector2f& pos);

		void Update(float detltaTime);
		void OnCollision(GameObject* owner);
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
		Utils::ResourceHandle<Resources::Texture> texture;
		Utils::ResourceHandle<Resources::Sound> shootingSound;
		float invulnerableTime;
		std::vector<Bullet> bullets;

	};
}