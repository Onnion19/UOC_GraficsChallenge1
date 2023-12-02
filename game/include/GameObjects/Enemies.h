#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Core/Physics.h"
#include "Utils/GameplayManager.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"
#include "Utils/Timers.h"
#include <array>
#include <tuple>
#include <functional>

namespace Components {
	class SpriteSheetAnimation;
	class Transform;
	class Atlas;
	struct MRU;
}

enum class EnemyType {
	Barrel, Fire, Cake
};
namespace GameObject
{

	

	class Enemy : public GameObject
	{
	public:
		Enemy(Core::GameManagers& managers, const Utils::Vector2f& position, const Utils::Vector2f& speed, const Components::SpriteSheetAnimation& animation);
		Enemy(Enemy&& other);
		~Enemy();

		void Respawn(const Utils::Vector2f& position, const Utils::Vector2f& speed, const Components::SpriteSheetAnimation& animation);

		bool IsAlive()const;
		void OnCollision(GameObject* other);

		void Update(float deltaTime);
		void Draw() const;
	private:
		void RegisterCollider();
		void UpdateCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager* physicsManager;
		GameplayManager* gameplayManager;
		Utils::Vector2i screenSize;

		Components::SpriteSheetAnimation* animationComponent;
		Components::Transform* transform;
		Components::MRU* mru;
		Collider collider;
	};


	class EnemiesPool : public GameObject {
		static inline const ResourceID AtlasTextureID{ "EnemiesTexture" };
		static constexpr auto AtlasTexturePath{ "Resources/Enemies/EnemiesAtlas.png" };
		static constexpr Utils::Vector2i AtlasSize{ 4,2 };
		static constexpr Utils::Vector2i BarrelAnimationCellsStart{ 0,0 };
		static constexpr Utils::Vector2i BarrelAnimationCellsEnd{ 3,0 };
		static constexpr Utils::Vector2i FireAnimationStart{ 1,1 };
		static constexpr Utils::Vector2i FireAnimationEnd{ 3,1 };
		static constexpr Utils::Vector2i CakeAnimationStart{ 0,1 };
		static constexpr Utils::Vector2i CakeAnimationEnd{ 0,1 };

		

	public:
		using SpawnEnemyProperties = std::tuple<EnemyType, Utils::Vector2f/*position*/, Utils::Vector2f/*speed*/>;
		using ScheduleEnemyFunctor = std::function<SpawnEnemyProperties()>;
		using ScheduleTimer = std::variant< Utils::RepeatingTimerWithVariation, Utils::RepeatingTimer>;
		EnemiesPool(Core::GameManagers& managers, unsigned poolSizeHint = 20);
		~EnemiesPool();

		void SpawnEnemy(EnemyType type, const Utils::Vector2f& position, const Utils::Vector2f& speed);

		[[nodiscard]] Utils::SafeCallbackObject ScheduleSpawnEnemy(ScheduleEnemyFunctor&& functor, ScheduleTimer&& timer);
		void Update(float deltatime);
		void Draw() const;

	private:
		Components::Atlas* atlas;
		Utils::Handle<Components::SpriteSheetAnimation> barrelAnimation;
		Utils::Handle <Components::SpriteSheetAnimation> fireAnimation;
		Utils::Handle <Components::SpriteSheetAnimation> cakeAnimation;
		std::vector<Enemy> pool;

		std::vector<std::pair<ScheduleEnemyFunctor, ScheduleTimer>> timers;

	};
}