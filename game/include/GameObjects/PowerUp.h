#pragma once

#include "GameObjects/GameOjbect.h"
#include "Core/Collider.h"
#include "Utils/Vector2.hpp"
#include "Utils/Handlers.h"
#include "Resources/Texture.h"
#include "Components/Atlas.h"
#include "Components/Transform.h"
#include "Core/Physics.h"
#include "Utils/GameplayManager.h"
#include <cassert>

namespace GameObject
{
	class InteractiveItem : public GameObject
	{
	public:
		InteractiveItem(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position, const Utils::Vector2f& size);
		~InteractiveItem();
		void Draw() const;
		virtual void OnCollision(GameObject* other) {};
	protected:
		void RegisterCollider();
		void UnregisterCollider();
	private:
		Components::Transform* transform;
		Components::Atlas* atlas;
		Utils::Vector2i atlasCell;
		Collider collider;
	};

	class PowerUp : public InteractiveItem {
	public:
		PowerUp(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position, const Utils::Vector2f& size);
		~PowerUp() = default;
		void OnCollision(GameObject* other) override;
	};


	class Peach : public InteractiveItem {
	public:
		Peach(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position, const Utils::Vector2f& size);
		~Peach() = default;
		void OnCollision(GameObject* other) override;
	};
}