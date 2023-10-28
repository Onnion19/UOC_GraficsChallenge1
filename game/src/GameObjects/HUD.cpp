#include "HUD.h"
#include "Utils/GameplayManager.h"

GameObject::HUD::HUD(Core::GameManagers& manager) : GameObject::GameObject(manager)
{
	auto& gameplayManager = gManager.GetManager<GameplayManager>();
	healthCallback = gameplayManager.RegisterHealthCallback(*this);
	scoreCallback = gameplayManager.RegisterScoreCallback(*this);
}

void GameObject::HUD::OnScoreUpdate(unsigned int newScore)
{
	score = newScore;
	// expensive allocation :( 
	scoreText = scoreTextPrefix + std::to_string(score);
}

void GameObject::HUD::OnHealthUpdate(unsigned int newHealth)
{
	health = newHealth;
	// expensive allocation :( 
	healthText = healthTextPrefix + std::to_string(health);
	switch (health)
	{
	case 2: 
		healthColor = YELLOW;
		break;
	case 1: 
		healthColor = RED;
		break;
	default: 
		healthColor = GREEN;
	}
}

void GameObject::HUD::Draw() const
{
	DrawText(healthText.c_str(), 5, 20, 20, healthColor);
	DrawText(scoreText.c_str(), 5, 45, 20, BLUE);
}
