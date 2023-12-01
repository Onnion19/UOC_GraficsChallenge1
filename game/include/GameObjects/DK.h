#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Core/Physics.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"

namespace Components {
	class Transform;
	class CyclicMRU;
}

namespace GameObject
{
	class DK : public GameObject {
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
		Collider collider;
	};
}