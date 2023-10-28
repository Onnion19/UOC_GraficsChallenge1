#pragma once

#include "GameObjects/GameOjbect.h"
#include "Utils/SafeCallbackToken.h"
namespace GameObject
{
	class HUD : public GameObject {
		static constexpr std::string_view scoreTextPrefix{ "Score: " };
		static constexpr std::string_view healthTextPrefix{ "Lives: " };
	public:
		HUD(Core::GameManagers& manager);
		~HUD();
		void OnScoreUpdate(unsigned int newScore);
		void OnHealthUpdate(unsigned int newHealth);
		void Update(float deltatime);
		void Draw() const;

	private:
		unsigned int score = 0;
		unsigned int health = 3;
		float gameTime;
		Color healthColor{};
		std::string scoreText{ scoreTextPrefix };
		std::string healthText{ healthTextPrefix };
		Utils::SafeCallbackObject healthCallback;
		Utils::SafeCallbackObject scoreCallback;

	};
}