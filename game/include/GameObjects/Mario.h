#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include <vector>
#include "Resources/Texture.h"
#include "Resources/music.h"
#include "Utils/Timers.h"
#include "Utils/GameplayManager.h"

#include <variant>
namespace Components {
	class Atlas;
	class SpriteSheetAnimationBook;
	class Transform;
}

namespace GameObject {

	namespace MarioMovement {
		struct MovementData {
			float horizontalSpeed = 0.f;
			float jumpStrength = 0.f;
			float gravity = 0.f;
			float climbSpeed = 0.f;
		};


		struct MovementBehaviorBase {
			MovementBehaviorBase() = default;
			MovementBehaviorBase(const MovementData& mv, const Utils::Vector2i& sz, Components::Transform* t) : movementData(mv), screenSize(sz), transform(t) {}
			MovementData movementData{};
			Utils::Vector2i screenSize{};
			Components::Transform* transform = nullptr;

		};

		struct WalkBehavior : public MovementBehaviorBase {
			WalkBehavior() = default;
			WalkBehavior(const MovementData& mv, const Utils::Vector2i& sz, Components::Transform* t) : MovementBehaviorBase(mv, sz, t) {}
			int operator()(float deltatime, Components::SpriteSheetAnimationBook* animation);


		};

		struct JumpBehavior : public WalkBehavior {
			JumpBehavior() = default;
			JumpBehavior(const MovementData& md, const Utils::Vector2i& sz, Components::Transform* t);
			int operator()(float deltatime, Components::SpriteSheetAnimationBook* animation);

			float verticalForce = 0.f;

		};

		struct ClimbBehavior : public MovementBehaviorBase {
			ClimbBehavior() = default;
			ClimbBehavior(const MovementData& mv, const Utils::Vector2i& sz, Components::Transform* t) : MovementBehaviorBase(mv, sz, t) {}
			int operator()(float deltatime, Components::SpriteSheetAnimationBook* animation);

		};

		struct DeathBehavior : public MovementBehaviorBase {
			DeathBehavior() = default;
			DeathBehavior(const MovementData& mv, const Utils::Vector2i& sz, Components::Transform* t) : MovementBehaviorBase(mv, sz, t) {}
			int operator()(float deltatime, Components::SpriteSheetAnimationBook* animation);

			float deathAnim = 0;
			bool idle = false;
		};

		// All those animations should probably be part of a animation map where contains sub animations inside and can choose which one to activate and render.
	}


	class Mario : public GameObject
	{
	public: 
		static constexpr float speed = 200.f;
		static constexpr auto marioTexturePath{ "resources/Characters/MarioAtlas.png" };
		inline static const ResourceID marioTextureID{ "MarioAtlas" };
		inline static const StringHash marioMoveLeft{ "RunLeft" };
		inline static const StringHash marioMoveRight{ "RunRight" };
		inline static const StringHash marioIdle{ "idle" };
		inline static const StringHash marioClimbUp{ "climbUp" };
		inline static const StringHash marioClimbDown{ "climbDown" };
		inline static const StringHash marioClimbIdle{ "climbIdle" };
		inline static const StringHash marioDeath{ "death" };
		inline static const StringHash marioDeathIdle{ "deathIdle" };
		static constexpr MarioMovement::MovementData MarioMovementData{ 200.f, 190.f,370.1f,90.f };
		inline static const Core::Tag enemyTag{ "DK" };
	public:

		Mario(Core::GameManagers& manager, const Utils::Vector2f& initialPosition);
		~Mario();
		const Utils::Vector2f& GetPosition()const;
		
		void Revive(const Utils::Vector2f& position);

		void Update(float detltaTime);
		void OnCollision(GameObject* owner);
		void Draw();

	private:
		void SetPosition(const Utils::Vector2f& pos);
		void Die();
		void RegisterAnimations();
		void RegisterCollider();
		void UpdateCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager& physics;
		GameplayManager& gameplayManager;
		Collider collider;

		bool collidedThisFrame = false;
		std::variant<MarioMovement::WalkBehavior, MarioMovement::JumpBehavior, MarioMovement::ClimbBehavior, MarioMovement::DeathBehavior> movementBehavior;

		Components::Transform* transform;
		Components::SpriteSheetAnimationBook* spriteAnimation;

		Utils::Vector2f initialPosition;
		Utils::SingleTimer deathTimer;
		Utils::SafeCallbackObject callback;
	};
}