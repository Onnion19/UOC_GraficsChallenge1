#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"

namespace GameObject {

	class Spaceship : public GameObject
	{
		static constexpr float invulerabilityTime = 3.f; // safe time after being hit
	public:
		Spaceship(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
		Spaceship(const Spaceship& b);
		Spaceship& operator=(const Spaceship& b);
		~Spaceship();

		const Utils::Vector2i& GetPosition()const;
		void SetPosition(const Utils::Vector2f& pos);

		void Update(float detltaTime);
		void OnCollision();
		void Draw();

		void StartInvulnerability(float time = invulerabilityTime);
	private:
		void RegisterCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager& physics;
		Utils::Vector2f position;
		const Utils::Vector2i size{ 100,100 };
		const Utils::Vector2f speed{ 45.f,45.f };
		Collider collider;
		float invulnerableTime = 0;
	};
}