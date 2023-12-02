#include "Components/PlayerController.h"
#include "Components/Transform.h"
#include "Components/SpriteSheetAnimationBook.h"
#include "Components/WallComponent.h"
#include "Components/StairComponent.h"
#include "GameObjects/GameOjbect.h"
#include "Utils/GameplayManager.h"
#include <algorithm>
#include <iostream>

int MarioMovement::WalkBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation) {

	int movement = 0;
	if (IsKeyDown(KEY_D)) {
		transform->position.x = std::clamp(transform->position.x + movementData.horizontalSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(movementData.horizontalSpeed);
		animation->SelectAnimation(FlipBook::marioMoveRight);
	}
	else if (IsKeyDown(KEY_A))
	{
		transform->position.x = std::clamp(transform->position.x - movementData.horizontalSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(-movementData.horizontalSpeed);
		animation->SelectAnimation(FlipBook::marioMoveLeft);
	}
	else
	{
		animation->SelectAnimation(FlipBook::marioIdle);
	}
	if (applyGravity)
		transform->position.y += movementData.gravity * deltatime;
	return movement;
}

MarioMovement::JumpBehavior::JumpBehavior(const MovementData& md, const Utils::Vector2i& sz, Components::Transform* t)
	: WalkBehavior(md, sz, t)
{
	verticalForce = movementData.jumpStrength;
	applyGravity = false;
}

int  MarioMovement::JumpBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation) {
	WalkBehavior::operator()(deltatime, animation);

	transform->position.y = std::clamp(transform->position.y - (verticalForce * deltatime), 0.f, static_cast<float>(screenSize.y));
	verticalForce -= movementData.gravity * deltatime;
	std::cout << "posY = " << transform->position.y << "\tforce: " << verticalForce << std::endl;
	return 0;
}

int  MarioMovement::ClimbBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation) {
	int movement = 0;
	if (IsKeyDown(KEY_W)) {
		transform->position.y = std::clamp(transform->position.y - movementData.climbSpeed * deltatime, 0.f, static_cast<float>(screenSize.y));
		animation->SelectAnimation(FlipBook::marioClimbUp);
	}
	else if (IsKeyDown(KEY_S))
	{
		transform->position.y = std::clamp(transform->position.y + movementData.climbSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		animation->SelectAnimation(FlipBook::marioClimbDown);
	}
	else
	{
		animation->SelectAnimation(FlipBook::marioIdle);
	}
	return movement;
}

int MarioMovement::DeathBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation)
{
	if (idle) return 0;
	animation->SelectAnimation(FlipBook::marioDeath);
	deathAnim += deltatime;
	if (deathAnim > 2.f)
	{
		animation->SelectAnimation(FlipBook::marioDeathIdle);
		idle = true;
	}
	return 0;
}

/** PLAYER CONTROLLER ***/

Components::PlayerController::PlayerController(Transform& t, const MarioMovement::MovementData& mv, const Utils::Vector2i& screenSize, GameplayManager& manager, ResourceManager& resourceManager)
	: transform(t)
	, movementData(mv)
	, screenSize(screenSize)
	, manager(manager)
	, rManager(resourceManager)
{
	ResetToWalk();

	walkSound = rManager.GetOrLoad<Resources::Sound>(walkSoundID, WalkSoundPath);
	jumpSound = rManager.GetOrLoad<Resources::Sound>(jumpSoundID, jumpSoundPath);
	dieSound = rManager.GetOrLoad<Resources::Sound>(dieSoundID, dieSoundPath);
}

Components::PlayerController::~PlayerController()
{
}

void Components::PlayerController::SetCollider(const Collider& collider)
{
	col = collider;
}

void Components::PlayerController::OnCollision(GameObject::GameObject* other)
{
	if (!other)return;
	if (isDead) return;

	if (other->GetTag() == DKTag)
	{
		Die();
		return;
	}

	if (auto wall = other->GetComponent<Components::WallComponent>())
	{
		if (auto walkBehavior = std::get_if<MarioMovement::WalkBehavior>(&movementBehavior))
		{
			transform.position.y = wall->GetSurfacePos() - transform.size.y / 2.f;
		}
		else if (auto jumptBehavior = std::get_if<MarioMovement::JumpBehavior>(&movementBehavior)) {
			if (jumptBehavior->verticalForce < 0)
			{
				ResetToWalk();
			}
		}
		else if (auto climb = std::get_if<MarioMovement::ClimbBehavior>(&movementBehavior))
		{
			if (std::abs(transform.position.y - wall->GetSurfacePos()) < 30)
			{
				ResetToWalk();
				transform.position.y = wall->GetSurfacePos() - transform.size.y / 2.f;
				canClimb = false;
			}
		}

	}
	else if (auto stair = other->GetComponent<Components::StairComponent>()) {
		canClimb = std::abs(transform.position.x - stair->GetStairPos()) < 15;
	}
	UpdateCollider();

}

void Components::PlayerController::UpdateCollider()
{
	col.UpdateColliderBounds(Geometry::Circle{ transform.position, transform.size.x / 2.f });
}

bool Components::PlayerController::IsDead() const
{
	return isDead;
}

void Components::PlayerController::Update(float deltaTime, SpriteSheetAnimationBook& spriteAnimation)
{
	// Update the movement
	if (IsKeyPressed(KEY_SPACE) && std::holds_alternative<MarioMovement::WalkBehavior>(movementBehavior))
	{
		movementBehavior = MarioMovement::JumpBehavior{ movementData, screenSize, &transform };
		PlaySound(*jumpSound);
	}
	if (canClimb && (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)))
	{
		movementBehavior = MarioMovement::ClimbBehavior{ movementData, screenSize, &transform };
	}

	auto move = std::visit([&](auto&& arg) -> int {return arg(deltaTime, &spriteAnimation); }, movementBehavior);
	if (move != 0)
	{
		if (!IsSoundPlaying(*walkSound))
		{
			PlaySound(*walkSound);
		}
	}
	else
	{
		StopSound(*walkSound);
	}

	UpdateCollider();
}

void Components::PlayerController::ResetToWalk()
{
	isDead = false;
	movementBehavior = MarioMovement::WalkBehavior{ movementData, screenSize, &transform };
}

void Components::PlayerController::Die()
{
	if (isDead)return;
	PlaySound(*dieSound);
	isDead = true;
	movementBehavior = MarioMovement::DeathBehavior{ movementData,screenSize , &transform };
	manager.UpdateHealth(-1);
}
