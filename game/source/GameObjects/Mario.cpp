#include "GameObjects/Mario.h"
#include "Core/Physics.h"
#include "Utils/GameplayManager.h"
#include "Components/Transform.h"
#include "Components/Atlas.h"
#include "Components/SpriteSheetAnimationBook.h"
#include "Components/WallComponent.h"
#include "Components/StairComponent.h"
#include "Core/WindowManager.h"



GameObject::Mario::Mario(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), physics(gManager.GetManager<Core::PhysicsManager>()), gameplayManager(gManager.GetManager<GameplayManager>())
{
	gameplayManager.SetHealth(3);
	healthCallbackObject = gameplayManager.RegisterHealthCallback(*this);

	initialPosition = pos;
	auto size = manager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize().x * 0.0260f;
	transform = &GetOrAddComponent<Components::Transform>();
	transform->position = initialPosition;
	transform->size = { size,size };

	playerController = &GetOrAddComponent<Components::PlayerController>(*transform, MarioMovementData, gManager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize(), gameplayManager, manager.GetManager<ResourceManager>());
	RegisterCollider();
	RegisterAnimations();
	SetTag("Player");
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
	playerController->ResetToWalk();
}

void GameObject::Mario::SetPosition(const Utils::Vector2f& pos)
{
	transform->position.x = std::clamp(pos.x, 0.f, 1920.f);
	transform->position.y = std::clamp(pos.y, 0.f, 1800.f);

}

void GameObject::Mario::Update(float deltatime)
{
	playerController->Update(deltatime, *spriteAnimation);
	spriteAnimation->Update(deltatime);
	deathTimer.Update(deltatime);
	physics.CheckCollisionOnCollider(collider);
}

void GameObject::Mario::Draw()
{
	spriteAnimation->Draw(*transform);
}

void GameObject::Mario::OnHealthUpdate(unsigned int newHealth)
{
	assert(!deathTimer.IsActive());
	if (newHealth > 0)
	{
		callback = deathTimer.Start([&]() {Revive(initialPosition); }, 3.5f);
	}
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

	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioMoveRight, std::move(runRight));
	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioMoveLeft, std::move(runLeft));
	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioIdle, std::move(Idle));
	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioClimbUp, std::move(climbUp));
	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioClimbDown, std::move(climbDown));
	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioDeath, std::move(deathAnim));
	spriteAnimation->AddSpriteSheet(MarioMovement::FlipBook::marioDeathIdle, std::move(deathIdleAnim));

	spriteAnimation->SelectAnimation(MarioMovement::FlipBook::marioClimbUp);
}

void GameObject::Mario::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Circle>(*playerController, this, transform->position, transform->size.x / 2.f);
	playerController->SetCollider(collider);
}

void GameObject::Mario::UnregisterCollider()
{
	physics.UnregisterCollider(collider);
}

