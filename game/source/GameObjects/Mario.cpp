#include "GameObjects/Mario.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/GameplayManager.h"
#include "Components/Transform.h"
#include "Components/Atlas.h"
#include "Components/SpriteSheetAnimation.h"
#include "Components/WallComponent.h"
#include "Core/WindowManager.h"


int GameObject::MarioMovement::WalkBehavior::operator()(float deltatime) {

	int movement = 0;
	if (IsKeyDown(KEY_D)) {
		transform->position.x = std::clamp(transform->position.x + movementData.horizontalSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(movementData.horizontalSpeed);
	}
	else if (IsKeyDown(KEY_A))
	{
		transform->position.x = std::clamp(transform->position.x - movementData.horizontalSpeed * deltatime, 0.f, static_cast<float>(screenSize.x));
		movement = static_cast<int>(-movementData.horizontalSpeed);
	}
	return movement;
}

GameObject::MarioMovement::JumpBehavior::JumpBehavior(const MovementData& md, const Utils::Vector2i& sz, Components::Transform* t)
	: WalkBehavior(md, sz, t)
{
	verticalForce = movementData.jumpStrength;
}

int GameObject::MarioMovement::JumpBehavior::operator()(float deltatime) {
	WalkBehavior::operator()(deltatime);

	transform->position.y = std::clamp(transform->position.y - (verticalForce * deltatime), 0.f, static_cast<float>(screenSize.y));
	verticalForce -= movementData.gravity * deltatime;
	return static_cast<int>(verticalForce);
}

int GameObject::MarioMovement::ClimbBehavior::operator()(float deltatime) {
	return 0;
}



GameObject::Mario::Mario(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	auto& resourceManager = gManager.GetManager<ResourceManager>();
	auto texture = resourceManager.GetOrLoad<Resources::Texture>(marioTextureID, marioTexturePath);

	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = pos;
	transform->size = { 50,50 };
	atlasComponent = &GetOrAddComponent<Components::Atlas>(std::move(texture), 6, 3);
	spriteAnimation = &GetOrAddComponent<Components::SpriteSheetAnimation>(*atlasComponent, Utils::Vector2i{ 0,0 }, Utils::Vector2i{ 5,2 }, 10);
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

void GameObject::Mario::SetPosition(const Utils::Vector2f& pos)
{
	transform->position.x = std::clamp(pos.x, 0.f, 1920.f);
	transform->position.y = std::clamp(pos.y, 0.f, 1800.f);

}

void GameObject::Mario::Update(float deltatime)
{
	physics.CheckCollisionOnCollider(collider);

	// Update the movement
	auto movement = std::visit([=](auto&& arg) -> int {return arg(deltatime); }, movementBehavior);

	if (IsKeyPressed(KEY_SPACE) && std::holds_alternative<MarioMovement::WalkBehavior>(movementBehavior))
	{
		movementBehavior = MarioMovement::JumpBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
	}


	spriteAnimation->Update();

	UpdateCollider();
	collidedThisFrame = false;
}

void GameObject::Mario::OnCollision(GameObject* other)
{
	if (!other)return;

	if (auto wall = other->GetComponent<Components::WallComponent>())
	{
		if (std::holds_alternative<MarioMovement::JumpBehavior>(movementBehavior))
		{
			transform->position.y = wall->GetSurfacePos() - transform->size.y / 2;
			movementBehavior = MarioMovement::WalkBehavior{ MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), transform };
		}
		collidedThisFrame = true;
	}
}

void GameObject::Mario::Draw()
{
	auto& cell = spriteAnimation->GetCurrentSprite();
	atlasComponent->Draw(cell, *transform);
}

void GameObject::Mario::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Circle>(*this, this, transform->position, transform->size.x / 1.3);
}

void GameObject::Mario::UpdateCollider()
{
	collider.UpdateColliderBounds(Geometry::Circle{ transform->position, transform->size.x / 2.5f });
}

void GameObject::Mario::UnregisterCollider()
{
	physics.UnregisterCollider(collider);
}

