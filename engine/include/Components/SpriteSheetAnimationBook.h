#pragma once

#include "Components/SpriteSheetAnimation.h"
#include "Utils/StringHash.h"
#include <unordered_map>

namespace Components {

	using SpriteSheetAnimationID = StringHash;
	class SpriteSheetAnimationBook {
	public:
		SpriteSheetAnimationBook() = default;

		void AddSpriteSheet(SpriteSheetAnimationID id, Utils::Handle<SpriteSheetAnimation>&& animation);
		void RemoveSpriteSheet(SpriteSheetAnimationID id);
		void SelectAnimation(SpriteSheetAnimationID id);
		void ClearSelection();
		void Update(float deltatime);
		void Draw(const Transform& transform);

	private:

		SpriteSheetAnimation* activeAnimation = nullptr;
		std::unordered_map<SpriteSheetAnimationID, Utils::Handle<SpriteSheetAnimation>> spriteSheetAnimationsMap;
	};
}

