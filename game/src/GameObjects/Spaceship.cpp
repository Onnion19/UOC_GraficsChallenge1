#include "raylib.h"
#include "Spaceship.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/GameplayManager.h"

GameObject::Spaceship::Spaceship(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), position(pos), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	RegisterCollider();
}

GameObject::Spaceship::Spaceship(const Spaceship& b)
	: GameObject(b.gManager)
	, position(b.position)
	, physics(gManager.GetManager<Core::PhysicsManager>())
{
	RegisterCollider();
}

GameObject::Spaceship& GameObject::Spaceship::operator=(const Spaceship& b)
{
	position = b.position;
	gManager = b.gManager;
	physics = gManager.GetManager<Core::PhysicsManager>();
	RegisterCollider();

	return *this;
}

GameObject::Spaceship::~Spaceship()
{
	UnregisterCollider();
}

const Utils::Vector2i& GameObject::Spaceship::GetPosition() const
{
	return position;
}


void GameObject::Spaceship::SetPosition(const Utils::Vector2f& pos)
{
	collider.UpdateColliderBounds(Geometry::Rectangle{ pos, pos + size });

	if (physics.CheckCollisionOnCollider(collider))
	{
		collider.UpdateColliderBounds(Geometry::Rectangle{ position, position + size });
		return;
	}

	position = pos;
}


void GameObject::Spaceship::Update(float deltatime)
{

	// Movement logic
	Utils::Vector2i movement{};

	if (IsKeyDown(KEY_A))	movement.x = -1;
	else if (IsKeyDown(KEY_D))	movement.x = 1;


	if (IsKeyDown(KEY_W))movement.y = -1;
	else if (IsKeyDown(KEY_S))	movement.y = 1;

	const Utils::Vector2f deltaMovement = Utils::Vector2f{ speed.x * movement.x , speed.y * movement.y } *deltatime;

	const auto desiredPosition = position + deltaMovement;
	SetPosition(desiredPosition);

	// Invulerability logic
	invulnerableTime -= deltatime;
	invulnerableTime = std::max(invulnerableTime, -1.f);
}

void GameObject::Spaceship::OnCollision()
{
	if (invulnerableTime > 0)return;

	gManager.GetManager<GameplayManager>().UpdateHealth(-1);
	StartInvulnerability();
}


void GameObject::Spaceship::Draw()
{
	// Add the effect of flickering when invulnerable
	if(invulnerableTime<0 || static_cast<int>(invulnerableTime*10)%2)
		DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), size.x, size.y, WHITE);
}

void GameObject::Spaceship::StartInvulnerability(float time)
{
	invulnerableTime = time;
}


void GameObject::Spaceship::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Rectangle>(position, position + size);
}

void GameObject::Spaceship::UnregisterCollider()
{
	if (!collider.Valid()) return;
	physics.UnregisterCollider(collider);
}
