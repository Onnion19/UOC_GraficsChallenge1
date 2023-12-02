#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"

namespace Core {
	class PhysicsManager;
}

namespace Components {
	class Transform;
	struct CyclicMRU;
	class SpriteSheetAnimation;
}

namespace GameObject
{
	class DK : public GameObject {
		static constexpr auto DK_AtlasPath{ "resources/Enemies/DonkeyKong.png" };
		static inline const ResourceID DK_AtlasID{ "DK_Atlas" };
		static constexpr Utils::Vector2i DK_AtlasSize{ 4,1 };
	public:
		DK(Core::GameManagers& managers, const Utils::Vector2f& initialPosition, float distance, float speed);
		~DK();

		void Update(float deltaTime);
		void Draw()const;
	private: 
		void RegisterCollider();
		void UpdateCollider();
		void UnregisterCollider();
	private:
		Core::PhysicsManager* manager;
		Components::CyclicMRU* mru;
		Components::Transform* transform;
		Components::SpriteSheetAnimation* animation;
		Collider collider;
	};
}