#include "GameObjects/Enemies.h"
#include "Components/Transform.h"
#include "Components/SpriteSheetAnimation.h"
#include "Components/MovementComponents.h"
#include <cassert>
#include "Core/WindowManager.h"

GameObject::Enemy::Enemy(Core::GameManagers& managers, const Utils::Vector2f& position, const Utils::Vector2f& speed, const Components::SpriteSheetAnimation& animation)
	: GameObject::GameObject(managers)
	, physicsManager(&managers.GetManager<Core::PhysicsManager>())
	, gameplayManager(&managers.GetManager<GameplayManager>())
{
	screenSize = managers.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	transform = &GetOrAddComponent<Components::Transform>();
	animationComponent = &GetOrAddComponent<Components::SpriteSheetAnimation>();
	mru = &GetOrAddComponent<Components::MRU>();
	Respawn(position, speed, animation);
}

GameObject::Enemy::~Enemy()
{
	RemoveComponent<Components::MRU>();
	RemoveComponent<Components::SpriteSheetAnimation>();
	RemoveComponent<Components::Transform>();
}

void GameObject::Enemy::Respawn(const Utils::Vector2f& position, const Utils::Vector2f& speed, const Components::SpriteSheetAnimation& animation)
{
	*animationComponent = animation;
	transform->position = position;
	mru->speed = speed;
	RegisterCollider();
}

bool GameObject::Enemy::IsAlive() const
{
	return collider.Valid();
}

void GameObject::Enemy::OnCollision(GameObject* other)
{
	assert(other && other->GetTag() == Core::Tag{ "Player" });

	// If player is on top, destroy it + add points
	auto playerTransform = other->GetComponent<Components::Transform>();
	assert(playerTransform);
	auto dir = Geometry::GetAngle(playerTransform->position - transform->position);

	if (dir > 70.f && dir < 110.f)
	{
		gameplayManager->UpdateScore(10);
		UnregisterCollider();
	}
}

void GameObject::Enemy::Update(float deltaTime)
{
	if (!IsAlive()) return;
	mru->Update(deltaTime, *transform);
	animationComponent->Update();
	if (transform->position.x > static_cast<float>(screenSize.x + transform->size.x) || transform->position.x < -transform->size.x)
	{
		UnregisterCollider();
	}
}

void GameObject::Enemy::Draw() const
{
	if (!IsAlive()) return;
	animationComponent->Draw(*transform);
}

void GameObject::Enemy::RegisterCollider()
{
	collider = physicsManager->RegisterCollider<Geometry::Circle>(*this, this, transform->position, transform->size.x / 2.f);
}

void GameObject::Enemy::UpdateCollider()
{
	collider.UpdateColliderBounds(Geometry::Circle{ transform->position, transform->size.x / 2.f });
}

void GameObject::Enemy::UnregisterCollider()
{
	physicsManager->UnregisterCollider(collider);
}

GameObject::EnemiesPool::EnemiesPool(Core::GameManagers& managers, unsigned poolSizeHint)
	: GameObject(managers)
{
	pool.reserve(poolSizeHint);
	auto texture = managers.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(AtlasTextureID, AtlasTexturePath);
	auto atlas = &GetOrAddComponent<Components::Atlas>(std::move(texture), AtlasSize.x, AtlasSize.y);

	barrelAnimation = std::make_unique<Components::SpriteSheetAnimation>(*atlas, BarrelAnimationCellsStart, BarrelAnimationCellsEnd, 10);
	fireAnimation = std::make_unique<Components::SpriteSheetAnimation>(*atlas, FireAnimationStart, FireAnimationEnd, 10);
	cakeAnimation = std::make_unique<Components::SpriteSheetAnimation>(*atlas, CakeAnimationStart, CakeAnimationEnd, 10);
}

void GameObject::EnemiesPool::SpawnEnemy(EnemyType type, const Utils::Vector2f& position, const Utils::Vector2f& speed)
{
	auto iter = std::find_if(pool.begin(), pool.end(), [](const Enemy& enemy) {return !enemy.IsAlive(); });
	
	Components::SpriteSheetAnimation* animation = nullptr; 
	switch (type) {
	case EnemyType::Barrel:
		animation = barrelAnimation.get();
		break;
	case EnemyType::Fire:
		animation = fireAnimation.get();
		break;
		case EnemyType::Cake:
			animation = cakeAnimation.get();
			break;
		default: 
			assert(false);
			break;	}

	if (iter != pool.end())
	{
		iter->Respawn(position, speed, *animation);
	}
	else
	{
		auto enemy = GameObjectFactory::MakeGameObject<Enemy>(position, speed, *animation);
		pool.push_back(std::move(enemy));
	}
}
