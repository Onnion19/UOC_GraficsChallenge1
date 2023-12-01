#pragma once 

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
namespace GameObject {

	class Wall : public GameObject {
	public: 
		Wall(Core::GameManagers& managers, const Utils::Vector2f& pos, const Utils::Vector2f& size);
		~Wall();

	private: 
		void RegisterCollider(const Utils::Vector2f& pos, const Utils::Vector2f& size);
		void UnregisterCollider();
	private: 

		Collider collider;
		
	};
}