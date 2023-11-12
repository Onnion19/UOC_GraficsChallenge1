#include "Components/Sprite.h"
#include "raylib.h"

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
	Rectangle renderQuad{ transform.position.x, transform.position.y, static_cast<float>(transform.size.x),static_cast<float> (transform.size.y) };
	DrawTexturePro(*texture, textureQuad, renderQuad, { transform.size.x / 2.f, transform.size.y / 2.f }, transform.rotation, WHITE);
}
