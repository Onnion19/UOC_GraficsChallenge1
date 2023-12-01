#pragma once 

#include "Utils/Handlers.h"
#include "Resources/Texture.h"
#include "Components/Transform.h"
#include "Utils/Vector2.hpp"

namespace Components {
	class Atlas
	{
	private:
		struct AtlasProperties {
			float columnPixels;
			float rowPixels;

			int rows;
			int columns;
		};
	public:
		Atlas() = default;
		Atlas(const Utils::ResourceHandle<Resources::Texture>& handle, int columns, int rows);
		Atlas(Utils::ResourceHandle<Resources::Texture>&& handle, int columns, int rows);
		Atlas(const Atlas& other);
		Atlas(Atlas&& other);

		void Draw(const Utils::Vector2i& cell, const Components::Transform& transform) const;

		Utils::Vector2i GetAtlasSize()const;
	private: 
		[[nodiscard]]Rectangle GetTextureQuadFromCell(const Utils::Vector2i& cell)const;
		[[nodiscard]]bool ValidCell(const Utils::Vector2i& cell)const;
	private:
		Utils::ResourceHandle<Resources::Texture> _texture;
		AtlasProperties _properties;
	};
}