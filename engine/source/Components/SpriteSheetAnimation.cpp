#include "Components/SpriteSheetAnimation.h"

Components::SpriteSheetAnimation::SpriteSheetAnimation(const Atlas& atlas, const Utils::Vector2i& initialSpriteAtlas, const Utils::Vector2i& lastSpriteAtlas, int framesSpeed, bool repeat)
	: sprites(BuildSpritesVectorFromRange(atlas, initialSpriteAtlas, lastSpriteAtlas))
	, atlas(&atlas)
	, currentSprite(0)
	, speed(framesSpeed)
	, loop(repeat)
	, currentFrame(0)
{}

Components::SpriteSheetAnimation::SpriteSheetAnimation(const Atlas& atlas, const std::vector<Utils::Vector2i>& sprites, int framesSpeed, bool repeat)
	: sprites(sprites)
	, atlas(&atlas)
	, currentSprite(0)
	, speed(framesSpeed)
	, loop(repeat)
	, currentFrame(0)
{
}

Components::SpriteSheetAnimation::SpriteSheetAnimation(const SpriteSheetAnimation& other)
	: sprites(other.sprites)
	, atlas(other.atlas)
	, currentSprite(0)
	, speed(other.speed)
	, loop(other.loop)
	, currentFrame(0)
{
}

Components::SpriteSheetAnimation& Components::SpriteSheetAnimation::operator=(const SpriteSheetAnimation& other)
{
	sprites = other.sprites;
	atlas = other.atlas;
	currentSprite = 0;
	speed = other.speed;
	loop = other.loop;
	currentFrame = 0;

	return *this;
}

void Components::SpriteSheetAnimation::Update()
{
	++currentFrame;
	if (currentFrame % speed == 0)
	{
		NextSprite();
		currentFrame = 0;
	}
}

void Components::SpriteSheetAnimation::Draw(const Transform& transform) const
{
	atlas->Draw(sprites[currentSprite], transform);
}

const Utils::Vector2i& Components::SpriteSheetAnimation::GetCurrentSprite() const
{
	return sprites[currentSprite];
}

void Components::SpriteSheetAnimation::Restart()
{
	currentFrame = 0;
}

std::vector<Utils::Vector2i> Components::SpriteSheetAnimation::BuildSpritesVectorFromRange(const Atlas& atlas, const Utils::Vector2i& initialSpriteAtlas, const Utils::Vector2i& lastSpriteAtlas)
{
	const auto atlasSize = atlas.GetAtlasSize();
	std::vector<Utils::Vector2i> spriteList;
	spriteList.reserve(15);
	Utils::Vector2i current = initialSpriteAtlas;
	while (current != lastSpriteAtlas)
	{
		spriteList.push_back(current);
		current.x = (current.x + 1) % atlasSize.x;
		if (current.x == 0)
			current.y++;
	}

	spriteList.push_back(lastSpriteAtlas);
	return spriteList;
}

void Components::SpriteSheetAnimation::NextSprite()
{
	if (loop)
	{
		currentSprite = (currentSprite + 1u) % sprites.size();
	}
	else
	{
		currentSprite = std::min(currentSprite + 1u, static_cast<unsigned>(sprites.size()) - 1u);

	}
}
