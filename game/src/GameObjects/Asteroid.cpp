#include "Asteroid.h"
#include "Utils/Geometry.h"
#include "Core/Physics.h"

GameObject::Asteroid::Asteroid(Core::GameManagers& manager, const AsteroidTransform& trans) : GameObject::GameObject(manager), transform(trans)
{
	RegisterCollider();
}

GameObject::Asteroid::~Asteroid()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Asteroid::Update(float deltaTime)
{
	if (!colider.Valid())return;
	transform.position += transform.movement * deltaTime;
	auto bot_right = transform.position + (transform.size * health);
	colider.UpdateColliderBounds<Geometry::Rectangle>({ Geometry::Point{ transform.position }, Geometry::Point{ bot_right } });
}

void GameObject::Asteroid::Draw()
{
	if (!colider.Valid()) return;
	DrawRectangle(static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), transform.size.x * health, transform.size.y * health, BLUE);
}

void GameObject::Asteroid::OnCollision()
{
	--health;
	if (health <= 0)
		gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Asteroid::RegisterCollider()
{
	auto bot_right = transform.position + transform.size;
	colider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(*this, Geometry::Point{ transform.position }, Geometry::Point{ bot_right });

}
