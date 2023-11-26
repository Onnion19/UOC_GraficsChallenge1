#pragma once 
#include "Utils/Vector2.hpp"
#include "Components/Atlas.h"
namespace Components {
	class SpriteSheetAnimation {
	public:
		SpriteSheetAnimation() = default;
		SpriteSheetAnimation(const Atlas& atlas, const Utils::Vector2i& initialSpriteAtlas, const Utils::Vector2i& lastSpriteAtlas, int framesSpeed, bool repeat = true);
		SpriteSheetAnimation(const std::vector<Utils::Vector2i>& sprites, int framesSpeed, bool repeat = true);

		void Update();
		const Utils::Vector2i& GetCurrentSprite() const;

	private: 
		std::vector<Utils::Vector2i> BuildSpritesVectorFromRange(const Atlas& atlas, const Utils::Vector2i& initialSpriteAtlas, const Utils::Vector2i& lastSpriteAtlas);
		void NextSprite();
	private:
		std::vector<Utils::Vector2i> sprites;
		unsigned currentSprite;
		int speed;
		int currentFrame;
		bool loop;



	};
}