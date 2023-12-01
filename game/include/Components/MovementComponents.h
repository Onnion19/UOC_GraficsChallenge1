#pragma once

#include "Utils/Vector2.hpp"

namespace Components {

	class Transform;

	struct MRU {
		MRU() = default;
		Utils::Vector2f speed{};
		void Update(float deltaTime, Transform& t);
	};

	struct CyclicMRU {
		CyclicMRU() = default;
		float speed = 0.f;
		Utils::Vector2f distance{};
		Utils::Vector2f initialPosition{};
		void Update(float deltaTime, Transform& t);
	private:
		float angle = 0;

	};
}