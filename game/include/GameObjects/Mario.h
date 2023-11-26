#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include <vector>
#include "Resources/Texture.h"
#include "Resources/music.h"

namespace GameObject {

	class Mario : public GameObject
	{
		static constexpr float invulerabilityTime = 3.f; // safe time after being hit
		static constexpr float rotationSpeed = 120.f;
		static constexpr float speed = 200.f;
		static constexpr auto spaceshipTexturePath{ "resources/spaceship.png" };
		static constexpr auto shootingSoundPath{ "resources/shoot.wav" };
	public:
		inline static const ResourceID spaceshipTextureID{ "SpaceshipTexture" };
		inline static const ResourceID shootingSoundID{ "ShootingSound" };

		Mario(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
	//	Mario(const Mario& b);
	//	Mario& operator=(const Mario& b);
	//	~Mario();

	//	Utils::Vector2i GetPosition()const;
	//	void SetPosition(const Utils::Vector2f& pos);

	//	void Update(float detltaTime);
	//	void OnCollision(GameObject* owner);
	//	void Draw();

	//	void StartInvulnerability(float time = invulerabilityTime);
	//private:
	//	void SpawnBullet();
	//	void RegisterCollider();
	//	void UnregisterCollider();
	private:
		Core::PhysicsManager& physics;
		Collider collider;
		Utils::ResourceHandle<Resources::Texture> texture;
	};
}