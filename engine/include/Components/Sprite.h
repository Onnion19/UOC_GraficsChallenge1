#pragma once 

#include "Utils/Handlers.h"
#include "Resources/Texture.h"
#include "Components/Transform.h"

namespace Components {
	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(const Utils::ResourceHandle<Resources::Texture>& handle);
		Sprite(Utils::ResourceHandle<Resources::Texture>&& handle);

		void Render(const Transform& transform) const;

	private:
		Utils::ResourceHandle<Resources::Texture> texture;
		Rectangle textureQuad;
	};
}