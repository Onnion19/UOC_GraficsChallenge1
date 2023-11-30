#pragma once

#include "GameObjects/GameOjbect.h"
#include "Resources/Texture.h"
#include <string_view>
#include "Core/Physics.h"
#include <vector>

namespace Components {
	class Sprite;
	struct Transform;
};

namespace GameObject {

	class Scenario : public GameObject {
	public: 
		Scenario(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture);
		Scenario(Core::GameManagers& managers, Utils::ResourceHandle<Resources::Texture>&& texture);

		void Draw();
	private: 
		void SetupTransform();
	private: 
		Components::Transform* transformComponent;
		Components::Sprite* spriteComponent;
		std::vector<Collider> colliders;
	};
}