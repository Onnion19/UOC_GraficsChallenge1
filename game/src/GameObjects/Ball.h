#pragma once

#include "Utils/Vector2.hpp"
#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"

namespace GameObject {

	struct BallProperties
	{
		Color color = BLUE; 
		float radius = 10.f;
	};


	class Ball : public GameObject
	{
	public: 
		Ball(Core::GameManagers& manager, const BallProperties& prop);
		Ball(const Ball& b);
		Ball& operator=(const Ball& b);
		~Ball();

		const Utils::Vector2i& GetPosition()const;
		void SetPosition(const Utils::Vector2i& pos);

		void Draw();
	private:
		void RegisterCollider();
		void UnregisterCollider();
	private:
		BallProperties properties;
		Core::PhysicsManager& physics;
		Utils::Vector2i position;
		Collider collider;
	};
}