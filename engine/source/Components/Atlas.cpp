#include "Components/Atlas.h"
#include "Render/RenderUtils.h"

Components::Atlas::Atlas(const Utils::ResourceHandle<Resources::Texture>& handle, int columns, int rows)
	: _texture(handle)
	, _properties({ _texture->width / static_cast<float>(columns), _texture->height / static_cast<float>(rows), rows, columns })
{}

Components::Atlas::Atlas(Utils::ResourceHandle<Resources::Texture>&& handle, int columns, int rows)
	: _texture(std::move(handle))
	, _properties({ _texture->width / static_cast<float>(columns), _texture->height / static_cast<float>(rows), rows, columns })
{}

Components::Atlas::Atlas(const Atlas& other)
	: _texture(other._texture)
	, _properties(other._properties)
{}

Components::Atlas::Atlas(Atlas&& other)
	: _texture(std::move(other._texture))
	, _properties(std::move(other._properties))
{}

void Components::Atlas::Draw(const Utils::Vector2i& cell, const Components::Transform& transform) const
{
	const auto textureQuad = GetTextureQuadFromCell(cell);
	RenderUtils::RenderImage(_texture.get(), textureQuad, transform);
}

Utils::Vector2i Components::Atlas::GetAtlasSize() const
{
	return { _properties.columns, _properties.rows };
}

bool Components::Atlas::ValidCell(const Utils::Vector2i& cell) const
{
	return cell.x >= 0 && cell.y >= 0 && cell.x < _properties.columns && cell.y < _properties.rows;
}

Rectangle Components::Atlas::GetTextureQuadFromCell(const Utils::Vector2i& cell) const
{
	if (!ValidCell(cell))
		return Rectangle();

	return {
		cell.x * _properties.columnPixels,
		cell.y * _properties.rowPixels,
		_properties.columnPixels,
		_properties.rowPixels };
}
