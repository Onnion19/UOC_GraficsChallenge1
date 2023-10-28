#pragma once

#include "GameObjects/GameOjbect.h"
#include "Utils/SafeCallbackToken.h"
namespace GameObject
{
	class HUD : public GameObject {
		inline static const std::string scoreTextPrefix{ "Score: " };
		inline static const std::string healthTextPrefix{ "Lives: " };
	public:
		HUD(Core::GameManagers& manager);

		void OnScoreUpdate(unsigned int newScore);
		void OnHealthUpdate(unsigned int newHealth);
		void Draw() const;

	private:
		unsigned int score = 0;
		unsigned int health = 3;
		std::string scoreText{ scoreTextPrefix };
		std::string healthText{ healthTextPrefix };
		Color healthColor = GREEN;
		Utils::SafeCallbackObject healthCallback;
		Utils::SafeCallbackObject scoreCallback;

	};
}