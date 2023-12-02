#include "GameObjects/Mario.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/GameplayManager.h"
#include "Components/Transform.h"
#include "Components/Atlas.h"
#include "Components/SpriteSheetAnimationBook.h"
#include "Components/WallComponent.h"
#include "Components/StairComponent.h"
#include "Core/WindowManager.h"


int GameObject::MarioMovement::WalkBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation) {

	int movement = 0;
	if (IsKeyDown(KEY_D)) {
		transform->position.x = std::clamp(transform->position.x + movementData.horizontalSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(movementData.horizontalSpeed);
		animation->SelectAnimation(Mario::marioMoveRight);
	}
	else if (IsKeyDown(KEY_A))
	{
		transform->position.x = std::clamp(transform->position.x - movementData.horizontalSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(-movementData.horizontalSpeed);
		animation->SelectAnimation(Mario::marioMoveLeft);
	}
	else
	{
		animation->SelectAnimation(Mario::marioIdle);
	}
	//transform->position.y += movementData.gravity * deltatime;
	return movement;
}

GameObject::MarioMovement::JumpBehavior::JumpBehavior(const MovementData& md, const Utils::Vector2i& sz, Components::Transform* t)
	: WalkBehavior(md, sz, t)
{
	verticalForce = movementData.jumpStrength;
}

int GameObject::MarioMovement::JumpBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation) {
	WalkBehavior::operator()(deltatime, animation);

	transform->position.y = std::clamp(transform->position.y - (verticalForce * deltatime), 0.f, static_cast<float>(screenSize.y));
	verticalForce -= movementData.gravity * deltatime;
	return static_cast<int>(verticalForce);
}

int GameObject::MarioMovement::ClimbBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation) {
	int movement = 0;
	if (IsKeyDown(KEY_W)) {
		transform->position.y = std::clamp(transform->position.y - movementData.climbSpeed * deltatime, 0.f, static_cast<float>(screenSize.y));
		movement = static_cast<int>(movementData.climbSpeed);
		animation->SelectAnimation(Mario::marioClimbUp);
	}
	else if (IsKeyDown(KEY_S))
	{
		transform->position.y = std::clamp(transform->position.y + movementData.climbSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(-movementData.horizontalSpeed);
		animation->SelectAnimation(Mario::marioClimbDown);
	}
	else
	{
		animation->SelectAnimation(Mario::marioIdle);
	}
	return movement;
}

int GameObject::MarioMovement::DeathBehavior::operator()(float deltatime, Components::SpriteSheetAnimationBook* animation)
{
	if (idle) return 0;
	animation->SelectAnimation(Mario::marioDeath);
	deathAnim += deltatime;
	if (deathAnim > 2.f)
	{
		animation->SelectAnimation(Mario::marioDeathIdle);
		idle = true;
	}
	return 0;
}



GameObject::Mario::Mario(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	initialPosition = pos;
	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = initialPosition;
	transform->size = { 50,50 };
	RegisterAnimations();
	RegisterCollider();
	movementBehavior = MarioMovement::WalkBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
}

GameObject::Mario::~Mario()
{
	RemoveComponent<Components::SpriteSheetAnimation>();
	RemoveComponent<Components::Atlas>();
	RemoveComponent<Components::Transform>();
	UnregisterCollider();
}

const Utils::Vector2f& GameObject::Mario::GetPosition() const
{
	return transform->position;
}

void GameObject::Mario::Revive(const Utils::Vector2f& position)
{
	SetPosition(position);
	movementBehavior = MarioMovement::WalkBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
}

void GameObject::Mario::SetPosition(const Utils::Vector2f& pos)
{
	transform->position.x = std::clamp(pos.x, 0.f, 1920.f);
	transform->position.y = std::clamp(pos.y, 0.f, 1800.f);

}

void GameObject::Mario::Update(float deltatime)
{
	physics.CheckCollisionOnCollider(collider);

	// Update the movement
	auto movement = std::visit([&](auto&& arg) -> int {return arg(deltatime, spriteAnimation); }, movementBehavior);

	if (IsKeyPressed(KEY_SPACE) && std::holds_alternative<MarioMovement::WalkBehavior>(movementBehavior))
	{
		movementBehavior = MarioMovement::JumpBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
	}
	spriteAnimation->Update(deltatime);

	UpdateCollider();
	collidedThisFrame = false;
	deathTimer.Update(deltatime);
}

void GameObject::Mario::OnCollision(GameObject* other)
{
	if (!other)return;
	if (deathTimer.IsActive()) return;
	if (other->GetTag() == enemyTag)
	{
		Die();
		return;
	}

	if (auto wall = other->GetComponent<Components::WallComponent>())
	{
		transform->position.y = wall->GetSurfacePos() - transform->size.y / 2.f;
		if (!std::holds_alternative<MarioMovement::WalkBehavior>(movementBehavior))
		{
			movementBehavior = MarioMovement::WalkBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
		}
		collidedThisFrame = true;
	}
	else if (auto stair = other->GetComponent<Components::StairComponent>()) {

		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
		{
			transform->position.x = static_cast<float>(stair->GetStairPos());
			movementBehavior = MarioMovement::ClimbBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
		}
	}

}

void GameObject::Mario::Draw()
{
	spriteAnimation->Draw(*transform);
}

void GameObject::Mario::Die()
{
	if (std::holds_alternative<MarioMovement::DeathBehavior>(movementBehavior)) return;
	movementBehavior = MarioMovement::DeathBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
	callback = deathTimer.Start([&]() {Revive(initialPosition); }, 3.4f);
}

void GameObject::Mario::RegisterAnimations()
{
	auto& resourceManager = gManager.GetManager<ResourceManager>();
	auto texture = resourceManager.GetOrLoad<Resources::Texture>(marioTextureID, marioTexturePath);

	spriteAnimation = &GetOrAddComponent<Components::SpriteSheetAnimationBook>();
	auto atlasComponent = &GetOrAddComponent<Components::Atlas>(std::move(texture), 6, 3);

	Utils::Handle<Components::SpriteSheetAnimation> runRight = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,0 }, Utils::Vector2i{ 2,0 }, 10);
	Utils::Handle<Components::SpriteSheetAnimation> runLeft = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 3,0 }, Utils::Vector2i{ 5,0 }, 10);
	Utils::Handle<Components::SpriteSheetAnimation> Idle = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 5,2 }, Utils::Vector2i{ 5,2 }, 10);
	Utils::Handle<Components::SpriteSheetAnimation> climbUp = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,1 }, Utils::Vector2i{ 2,1 }, 10);
	Utils::Handle<Components::SpriteSheetAnimation> climbDown = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 3,1 }, Utils::Vector2i{ 5,1 }, 10);
	Utils::Handle<Components::SpriteSheetAnimation> deathAnim = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,2 }, Utils::Vector2i{ 3,2 }, 10);
	Utils::Handle<Components::SpriteSheetAnimation> deathIdleAnim = std::make_unique< Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 4,2 }, Utils::Vector2i{ 4,2 }, 10);

	spriteAnimation->AddSpriteSheet(marioMoveRight, std::move(runRight));
	spriteAnimation->AddSpriteSheet(marioMoveLeft, std::move(runLeft));
	spriteAnimation->AddSpriteSheet(marioIdle, std::move(Idle));
	spriteAnimation->AddSpriteSheet(marioClimbUp, std::move(climbUp));
	spriteAnimation->AddSpriteSheet(marioClimbDown, std::move(climbDown));
	spriteAnimation->AddSpriteSheet(marioDeath, std::move(deathAnim));
	spriteAnimation->AddSpriteSheet(marioDeathIdle, std::move(deathIdleAnim));

	spriteAnimation->SelectAnimation(marioClimbUp);
}

void GameObject::Mario::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Circle>(*this, this, transform->position, transform->size.x / 2.f);
}

void GameObject::Mario::UpdateCollider()
{
	collider.UpdateColliderBounds(Geometry::Circle{ transform->position, transform->size.x / 2.f });
}

void GameObject::Mario::UnregisterCollider()
{
	physics.UnregisterCollider(collider);
}

