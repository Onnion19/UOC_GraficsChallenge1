#pragma once 

#include "Utils/Vector2.hpp"
#include <variant>
#include "Utils/StringHash.h"
#include "Core/Tag.h"
#include "Core/Physics.h"

class GameplayManager;
namespace Components {
	class Transform;
	class SpriteSheetAnimationBook;
}

namespace GameObject {
	class GameObject;
}


namespace MarioMovement {
	namespace FlipBook {
		inline const StringHash marioMoveLeft{ "RunLeft" };
		inline const StringHash marioMoveRight{ "RunRight" };
		inline const StringHash marioIdle{ "idle" };
		inline const StringHash marioClimbUp{ "climbUp" };
		inline const StringHash marioClimbDown{ "climbDown" };
		inline const StringHash marioClimbIdle{ "climbIdle" };
		inline const StringHash marioDeath{ "death" };
		inline const StringHash marioDeathIdle{ "deathIdle" };
	}


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

		bool applyGravity = true;

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

namespace Components {
	class PlayerController
	{
		inline static const Core::Tag enemyTag{ "Enemy" };
		inline static const Core::Tag DKTag{ "DK" };
	public:
		PlayerController(Transform& t, const MarioMovement::MovementData& mv, const Utils::Vector2i& screenSize, GameplayManager& manager);
		~PlayerController();
		void SetCollider(const Collider& collider);
		void OnCollision(GameObject::GameObject* other);

		void Update(float deltaTime, SpriteSheetAnimationBook& spriteAnimation);
		void ResetToWalk();
		void Die();
		void UpdateCollider();
	private:
		Transform& transform; 
		Collider col;
		GameplayManager& manager;
		bool isDead = false;
		MarioMovement::MovementData movementData;
		Utils::Vector2i screenSize;
		std::variant<MarioMovement::WalkBehavior, MarioMovement::JumpBehavior, MarioMovement::ClimbBehavior, MarioMovement::DeathBehavior> movementBehavior;
		bool canClimb = false;
	};
}