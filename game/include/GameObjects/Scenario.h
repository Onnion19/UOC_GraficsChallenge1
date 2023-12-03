#pragma once

#include "GameObjects/GameOjbect.h"
#include "Resources/Texture.h"
#include <string_view>
#include "Core/Physics.h"
#include <vector>

namespace Components {
	class Sprite;
	class Transform;
};

namespace GameObject {

	class Scenario : public GameObject {
	public: 
		Scenario(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture);
		Scenario(Core::GameManagers& managers, Utils::ResourceHandle<Resources::Texture>&& texture);
		~Scenario();

		void Draw();
	private: 
		void SetupTransform();
		void RegisterWalls();
		void RegisterStairs();
		void RegisterFireBarrel();
	private: 
		Components::Transform* transformComponent;
		Components::Sprite* spriteComponent;
		std::vector<Utils::Handle<GameObject>> colliders;

		Utils::Handle<GameObject> fireBarrel;
		Components::Transform* barreltransformComponent;
		Components::Sprite* barrelspriteComponent;
	};
}