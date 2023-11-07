#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Core/Physics.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"

namespace GameObject
{
	struct BulletTransform {
		Utils::Vector2f position;
		Utils::Vector2i size;
		Utils::Vector2f movement;
		float rotation;
	};

	class Bullet : public GameObject {
		static constexpr float lifespan = 3.f;
	public:
		inline static const ResourceID bulletTextureId{ "bulletTexture" };

		Bullet(Core::GameManagers& manager, const BulletTransform& trans);
		~Bullet();

		Bullet(const Bullet&);
		Bullet& operator=(const Bullet& b);

		void Update(float deltaTime);
		bool Active()const;
		void Draw()const;

		void OnCollision();
	private:
		void Invalidate();
		void RegisterCollider();
		void UnRegisterCollider();
	private:
		BulletTransform transform;
		Utils::ResourceHandle<Texture2D> texture;
		Collider colider;
		Core::PhysicsManager& physicsManager;
		float lifespanTimer = lifespan;
	};
}