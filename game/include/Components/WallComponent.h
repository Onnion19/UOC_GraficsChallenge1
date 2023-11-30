#pragma once 


class Collider;

namespace Components
{
	class WallComponent {
	public:
		WallComponent(const Collider& collider);
		int GetSurfacePos() const;
	private:
		int surfacePosY;
	};
}