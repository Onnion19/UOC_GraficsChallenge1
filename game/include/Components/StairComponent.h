#pragma once 
class Collider;

namespace Components
{
	class StairComponent {
	public:
		StairComponent(const Collider& collider);
		int GetStairPos() const;
	private:
		int stairPos = 0;
	};
}