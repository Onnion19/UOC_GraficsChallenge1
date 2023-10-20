#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
namespace GameObject
{
	struct BlockTransform {
		Utils::Vector2i position; 
		Utils::Vector2i size;
	};

	class Block : public GameObject {
	public: 
		Block(Core::GameManagers& manager, const BlockTransform& trans);
		~Block();

		void Draw();

		void OnCollision();
	private:

		void RegisterCollider();
	private: 
		BlockTransform transform;
		Collider colider;
	};
}