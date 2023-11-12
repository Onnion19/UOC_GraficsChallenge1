#pragma once 

#include "Utils/Vector2.hpp"

namespace Components
{
	struct Transform
	{
		Utils::Vector2f position;
		float rotation; 
		Utils::Vector2f size;
	};
}