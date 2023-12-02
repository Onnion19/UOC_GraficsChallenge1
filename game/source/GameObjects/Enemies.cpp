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
	animationComponent = &GetOrAddComponent<Components::SpriteSheetAnimation>(animation);
	mru = &GetOrAddComponent<Components::MRU>();
	Respawn(position, speed, animation);
}

GameObject::Enemy::Enemy(Enemy&& other) : GameObject(other)
{
	physicsManager = other.physicsManager;
	gameplayManager = other.gameplayManager;
	transform = &GetOrAddComponent<Components::Transform>(std::move(*other.transform));
	animationComponent = &GetOrAddComponent<Components::SpriteSheetAnimation>(std::move(*other.animationComponent));
	mru = &GetOrAddComponent<Components::MRU>(*other.mru);
	screenSize = other.screenSize;
	SetTag(other.GetTag());
	RegisterCollider();

}

GameObject::Enemy::~Enemy()
{
	RemoveComponent<Components::MRU>();
	RemoveComponent<Components::SpriteSheetAnimation>();
	RemoveComponent<Components::Transform>();
	UnregisterCollider();
}

void GameObject::Enemy::Respawn(const Utils::Vector2f& position, const Utils::Vector2f& speed, const Components::SpriteSheetAnimation& animation)
{
	*animationComponent = animation;
	transform->position = position;
	transform->size = { 50.f,50.f };
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
	auto dir = Geometry::GetAngle(transform->position - playerTransform->position);

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
	UpdateCollider();
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
	timers.reserve(poolSizeHint);
	pool.reserve(poolSizeHint);
	auto texture = managers.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(AtlasTextureID, AtlasTexturePath);
	atlas = &GetOrAddComponent<Components::Atlas>(std::move(texture), AtlasSize.x, AtlasSize.y);

	barrelAnimation = std::make_unique<Components::SpriteSheetAnimation>(*atlas, BarrelAnimationCellsStart, BarrelAnimationCellsEnd, 10);
	fireAnimation = std::make_unique<Components::SpriteSheetAnimation>(*atlas, FireAnimationStart, FireAnimationEnd, 10);
	cakeAnimation = std::make_unique<Components::SpriteSheetAnimation>(*atlas, CakeAnimationStart, CakeAnimationEnd, 10);
}

GameObject::EnemiesPool::~EnemiesPool()
{
	RemoveComponent<Components::Atlas>();
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
		break;
	}

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

Utils::SafeCallbackObject GameObject::EnemiesPool::ScheduleSpawnEnemy(ScheduleEnemyFunctor&& functor, ScheduleTimer&& timer)
{
	auto& [inserted_function, inserted_timer] = timers.emplace_back(std::move(functor), std::move(timer));

	// this is a bug, lambda storing reference of `functor` which is stored into a non stable adressing container.
	// This can end into undefined behavior if the vector needs to reallocate the contents.
	// For now, doing a copy of it as std::function is a type erased and with luck it will allocate the memory on the heap.
	auto lambda = [=]() {
		auto [type, position, speed] = inserted_function();
		SpawnEnemy(type, position, speed);
		};


	return std::visit([&](auto&& timer) -> Utils::SafeCallbackObject {return timer.Start(lambda); }, inserted_timer);
}

void GameObject::EnemiesPool::Update(float deltatime)
{

	std::for_each(pool.begin(), pool.end(), [=](Enemy& enemy) { enemy.Update(deltatime); });
	std::for_each(timers.begin(), timers.end(), [=](decltype(timers)::value_type& data)
		{
			std::visit([=](auto&& timer) {timer.Update(deltatime); }, data.second);
		});



}

void GameObject::EnemiesPool::Draw() const
{
	std::for_each(pool.begin(), pool.end(), [](const Enemy& enemy) { enemy.Draw(); });
}

