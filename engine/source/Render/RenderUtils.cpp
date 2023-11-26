#include "Render/RenderUtils.h"

void RenderUtils::RenderImage(Resources::Texture* texture, const Rectangle& textureQuad, const Components::Transform& transform, const Color& color)
{
	const Rectangle renderQuad{ transform.position.x, transform.position.y, static_cast<float>(transform.size.x),static_cast<float> (transform.size.y) };
	DrawTexturePro(*texture, textureQuad, renderQuad, { transform.size.x / 2.f, transform.size.y / 2.f }, transform.rotation, WHITE);
}