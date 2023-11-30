#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include <vector>
#include "Resources/Texture.h"
#include "Resources/music.h"

namespace Components {
	class Atlas;
	class SpriteSheetAnimation;
	class Transform;
}

namespace GameObject {

	class Mario : public GameObject
	{
		static constexpr float speed = 200.f;
		static constexpr auto marioTexturePath{ "resources/Characters/MarioAtlas.png" };
		inline static const ResourceID marioTextureID{ "MarioAtlas" };
	public:

		Mario(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
		const Utils::Vector2f& GetPosition()const;
		void SetPosition(const Utils::Vector2f& pos);

		void Update(float detltaTime);
		void OnCollision(GameObject* owner);
		void Draw();

	private:
		void RegisterCollider();
		void UpdateCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager& physics;
		Collider collider;

		bool collidedThisFrame = false;

		Components::Transform* transform;
		Components::Atlas* atlasComponent;
		Components::SpriteSheetAnimation* spriteAnimation;
	};
}