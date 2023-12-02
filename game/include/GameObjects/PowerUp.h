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
	template<typename T> 
	class InteractiveItem : public GameObject
	{
	public: 
		InteractiveItem(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position);
		~InteractiveItem();
		void Draw() const;
		void OnCollision(GameObject* other);
	protected: 
		void RegisterCollider();
		void UnregisterCollider();
	private:
		Components::Transform* transform;
		Components::Atlas* atlas;
		Utils::Vector2i atlasCell;
		Collider collider;
	};

	class PowerUp : public InteractiveItem<PowerUp> {
	public: 
		PowerUp(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position);
		~PowerUp() = default;
		void OnCollisionImpl(GameObject *other);
	};


	class Peach : public InteractiveItem<Peach> {
	public:
		Peach(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position);
		~Peach() = default;
		void OnCollisionImpl(GameObject* other);
	};


	template<typename T>
	inline InteractiveItem<T>::InteractiveItem(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position)
		: GameObject(managers)
	{
		transform = &GetOrAddComponent<Components::Transform>();
		transform->position = position;
		transform->size = { 50,50 };

		atlas = &GetOrAddComponent<Components::Atlas>(texture, atlasSize.x, atlasSize.y);
		atlasCell = cell;
		RegisterCollider();
	}

	template<typename T>
	inline InteractiveItem<T>::~InteractiveItem()
	{
		UnregisterCollider();
		RemoveComponent< Components::Atlas>();
		RemoveComponent< Components::Transform>();
	}
	template<typename T>
	inline void InteractiveItem<T>::Draw() const
	{
		if (!collider.Valid()) return;
		atlas->Draw(atlasCell, *transform);
	}

	template<typename T>
	inline void InteractiveItem<T>::OnCollision(GameObject* other)
	{
		static_cast<T*>(this)->OnCollisionImpl(other);
	}
	template<typename T>
	inline void InteractiveItem<T>::RegisterCollider()
	{
		collider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Circle>(*this, this, transform->position, transform->size.x / 2.f);
		SetTag("PowerUp");
	}
	template<typename T>
	inline void InteractiveItem<T>::UnregisterCollider()
	{
		gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(collider);
	}
}