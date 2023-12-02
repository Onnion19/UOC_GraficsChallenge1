#include "GameObjects/PowerUp.h"
#include "Scenes/SceneManager.h"

GameObject::PowerUp::PowerUp(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position)
	: InteractiveItem(managers, texture, atlasSize, cell, position)
{}


void GameObject::PowerUp::OnCollisionImpl(GameObject* other)
{
	assert(other && other->GetTag() == Core::Tag{ "Player" });
	UnregisterCollider();
	gManager.GetManager<GameplayManager>().UpdateScore(300);
}

GameObject::Peach::Peach(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture, const Utils::Vector2i& atlasSize, const Utils::Vector2i& cell, const Utils::Vector2f& position)
	: InteractiveItem(managers, texture, atlasSize, cell, position) {}

void GameObject::Peach::OnCollisionImpl(GameObject* other)
{
	assert(other && other->GetTag() == Core::Tag{ "Player" });
	UnregisterCollider();
	gManager.GetManager<GameplayManager>().EndGame();
	gManager.GetManager<SceneManager>().LoadScene(ResourceID{ "EndScene" });
}
