#pragma once 

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
namespace GameObject {

	class Stair : public GameObject {
	public:
		Stair(Core::GameManagers& managers, const Utils::Vector2f& pos, const Utils::Vector2f& size);
		~Stair();

	private:
		void RegisterCollider(const Utils::Vector2f& pos, const Utils::Vector2f& size);
	private:
		Collider collider;

	};
}