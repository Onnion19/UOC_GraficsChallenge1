#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include <vector>
#include "Resources/Texture.h"
#include "Resources/music.h"
#include "Utils/Timers.h"
#include "Utils/GameplayManager.h"
#include "Components/PlayerController.h"

#include <variant>
namespace Components {
	class Atlas;
	class SpriteSheetAnimationBook;
	class Transform;
}

namespace GameObject {

	class Mario : public GameObject
	{
	public: 
		static constexpr float speed = 200.f;
		static constexpr auto marioTexturePath{ "resources/Characters/MarioAtlas.png" };
		inline static const ResourceID marioTextureID{ "MarioAtlas" };
		static constexpr MarioMovement::MovementData MarioMovementData{ 200.f, 190.f,370.1f,90.f };

	public:

		Mario(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
		~Mario();
		const Utils::Vector2f& GetPosition()const;
		
		void Revive(const Utils::Vector2f& position);

		void Update(float detltaTime);
		void Draw();

	private:
		void SetPosition(const Utils::Vector2f& pos);
		void RegisterAnimations();
		void RegisterCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager& physics;
		GameplayManager& gameplayManager;
		Collider collider;
		Components::Transform* transform;
		Components::SpriteSheetAnimationBook* spriteAnimation;
		Components::PlayerController* playerController;
		Utils::Vector2f initialPosition;
		Utils::SingleTimer deathTimer;
		Utils::SafeCallbackObject callback;
	};
}