#include "Bullet.h"
#include "Utils/Geometry.h"
#include "Core/Physics.h"

GameObject::Bullet::Bullet(Core::GameManagers& manager, const BulletTransform& trans) : GameObject::GameObject(manager), transform(trans)
{
	RegisterCollider();
}

GameObject::Bullet::~Bullet()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Bullet::Update(float deltaTime)
{
	transform.position += transform.movement * deltaTime;
	auto bot_right = transform.position + transform.size;
	colider.UpdateColliderBounds<Geometry::Rectangle>({ Geometry::Point{ transform.position }, Geometry::Point{ bot_right } });
}

void GameObject::Bullet::Draw()
{
	if (!colider.Valid()) return;
	DrawRectangle(static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), transform.size.x, transform.size.y, BLUE);
}

void GameObject::Bullet::OnCollision()
{
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Bullet::RegisterCollider()
{
	auto bot_right = transform.position + transform.size;
	colider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(*this, Geometry::Point{ transform.position }, Geometry::Point{ bot_right });

}
