#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"
#include "Resources/Texture.h"

namespace Components {
	class Transform;
	class Atlas;
}

namespace GameObject
{
	class PowerUp : public GameObject {
	public: 
		PowerUp(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell,const Utils::Vector2f& position);
		~PowerUp();
		void Draw() const;
		void OnCollision(GameObject *other);
	private:
		void RegisterCollider();
		void UnregisterCollider();
	private: 
		Components::Transform* transform; 
		Components::Atlas* atlas; 
		Utils::Vector2i atlasCell;
		Collider collider;
	};
}