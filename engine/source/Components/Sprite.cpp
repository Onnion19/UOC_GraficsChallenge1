#include "Components/Sprite.h"
#include "Render/RenderUtils.h"

Components::Sprite::Sprite(const Utils::ResourceHandle<Resources::Texture>& handle) : texture(handle)
{
	textureQuad = { 0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height) };
}

Components::Sprite::Sprite(Utils::ResourceHandle<Resources::Texture>&& handle) : texture(std::move(handle))
{
	textureQuad = { 0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height) };
}

void Components::Sprite::Render(const Components::Transform& transform) const
{
	RenderUtils::RenderImage(texture.get(), textureQuad, transform);
}
