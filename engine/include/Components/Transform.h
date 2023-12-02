#pragma once 

#include "Utils/Vector2.hpp"

namespace Components
{
	class Transform
	{
	public:
		Utils::Vector2f position;
		float rotation = 0.f; 
		Utils::Vector2f size;
	};
}