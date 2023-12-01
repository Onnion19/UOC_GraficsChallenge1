#include "Components/SpriteSheetAnimationBook.h"
#include <cassert>
void Components::SpriteSheetAnimationBook::AddSpriteSheet(SpriteSheetAnimationID id, Utils::Handle<SpriteSheetAnimation>&& animation)
{
	spriteSheetAnimationsMap[id] = std::move(animation);
}

void Components::SpriteSheetAnimationBook::RemoveSpriteSheet(SpriteSheetAnimationID id)
{
	auto iter = spriteSheetAnimationsMap.find(id);
	if (iter == spriteSheetAnimationsMap.end())
		return;

	if (activeAnimation == iter->second.get())
		ClearSelection();

	spriteSheetAnimationsMap.erase(id);
}

void Components::SpriteSheetAnimationBook::SelectAnimation(SpriteSheetAnimationID id)
{
	auto iter = spriteSheetAnimationsMap.find(id);
	if (iter == spriteSheetAnimationsMap.end())
		return;

	activeAnimation = iter->second.get();
	assert(activeAnimation != nullptr);
}

void Components::SpriteSheetAnimationBook::ClearSelection()
{
	activeAnimation = nullptr;
}

void Components::SpriteSheetAnimationBook::Update(float deltatime)
{
	if (!activeAnimation)
		return;

	activeAnimation->Update();
}

void Components::SpriteSheetAnimationBook::Draw(const Transform& transform)
{
	if (!activeAnimation)
		return;

	activeAnimation->Draw(transform);
}
