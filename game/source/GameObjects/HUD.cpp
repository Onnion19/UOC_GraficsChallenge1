#include "GameObjects/HUD.h"
#include "Core/WindowManager.h"
#include "Utils/GameplayManager.h"
#include "Components/Sprite.h"

GameObject::HUD::HUD(Core::GameManagers& manager) : GameObject::GameObject(manager)
{
	// HP Icons
	auto hpIcon = manager.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(HealthIconID, HealthIconPath);
	healthIcon = &GetOrAddComponent<Components::Sprite>(std::move(hpIcon));
	const auto screenSize = manager.GetManager<WindowManager>().GetCurrentWindow()->GetWindowSize();
	for (int i = 0; i < iconTransforms.size(); i++)
	{
		auto posx = screenSize.x - (1 - iconMarginPercent.x * (i + 1)) - iconSize.x * (i+1);
		iconTransforms[i].position = {posx, screenSize.y * iconMarginPercent.y };
		iconTransforms[i].size = iconSize;
	}

	auto& gameplayManager = gManager.GetManager<GameplayManager>();
	healthCallback = gameplayManager.RegisterHealthCallback(*this);
	scoreCallback = gameplayManager.RegisterScoreCallback(*this);
}

GameObject::HUD::~HUD()
{
	healthCallback.Release();
	scoreCallback.Release();
	RemoveComponent<Components::Sprite>();
}

void GameObject::HUD::OnScoreUpdate(unsigned int newScore)
{
	score = newScore;
	// expensive allocation :( 
	scoreText = scoreTextPrefix.data() + std::to_string(score);
}

void GameObject::HUD::OnHealthUpdate(unsigned int newHealth)
{
	health = newHealth;
}

void GameObject::HUD::Update(float deltatime)
{
}

void GameObject::HUD::Draw() const
{
	for (int i = 0; i < health; i++)
	{
		healthIcon->Render(iconTransforms[i]);
	}
	DrawText(scoreText.c_str(), 5, 45, 20, Resources::Blue);
}
