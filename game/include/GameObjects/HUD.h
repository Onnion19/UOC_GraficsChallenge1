#pragma once

#include "GameObjects/GameOjbect.h"
#include "Utils/SafeCallbackToken.h"
#include "Resources/Color.h"
#include "Utils/Vector2.hpp"
#include "Components/Transform.h"
#include <array>
namespace Components
{
	class Sprite;
}

namespace GameObject
{
	class HUD : public GameObject {
		inline static const ResourceID HealthIconID{ "HPIcon" };
		static constexpr auto HealthIconPath{ "resources/UI/MarioLifesIcon.png" };
		static constexpr Utils::Vector2i iconSize{ 80,80 };
		static constexpr Utils::Vector2f iconMarginPercent{0.05f, 0.1f};

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
		Utils::Vector2i screenSize; 

		Components::Sprite* healthIcon;
		std::array<Components::Transform, 3> iconTransforms;

		Resources::Color healthColor{ WHITE };
		std::string scoreText{ scoreTextPrefix };
		Utils::SafeCallbackObject healthCallback;
		Utils::SafeCallbackObject scoreCallback;

	};
}