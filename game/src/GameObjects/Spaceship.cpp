#include "raylib.h"
#include "Spaceship.h"
#include "Core/Physics.h"
#include "Utils/Geometry.h"
#include "Utils/GameplayManager.h"
#include "Resources/Texture.h"


GameObject::Spaceship::Spaceship(Core::GameManagers& manager, const Utils::Vector2f& pos) : GameObject(manager), position(pos), collider(), physics(gManager.GetManager<Core::PhysicsManager>())
{
	RegisterCollider();
	auto& resourceManager = gManager.GetManager<ResourceManager>();
	texture = &resourceManager.GetOrLoad<Texture2D>(spaceshipTextureID, spaceshipTexturePath);
	shootingSound = &resourceManager.GetOrLoad<Sound>(shootingSoundID, shootingSoundPath);
	bullets.reserve(150);
}

GameObject::Spaceship::Spaceship(const Spaceship& b)
	: GameObject(b.gManager)
	, position(b.position)
	, physics(gManager.GetManager<Core::PhysicsManager>())
{
	RegisterCollider();
	auto& resourceManager = gManager.GetManager<ResourceManager>();
	texture = &resourceManager.GetOrLoad<Texture2D>(spaceshipTextureID, spaceshipTexturePath);
	shootingSound = &resourceManager.GetOrLoad<Sound>(shootingSoundID, shootingSoundPath);
	bullets.reserve(150);
}

GameObject::Spaceship& GameObject::Spaceship::operator=(const Spaceship& b)
{
	position = b.position;
	gManager = b.gManager;
	physics = gManager.GetManager<Core::PhysicsManager>();
	texture = b.texture;
	RegisterCollider();

	return *this;
}

GameObject::Spaceship::~Spaceship()
{
	UnregisterCollider();
	bullets.clear();
}

Utils::Vector2i GameObject::Spaceship::GetPosition() const
{
	return position;
}


void GameObject::Spaceship::SetPosition(const Utils::Vector2f& pos)
{

	const auto x = (pos.x > 0) ? static_cast<int>(pos.x) % 1920 : 1920 - pos.x;
	const auto y = (pos.y > 0) ? static_cast<int>(pos.y) % 1080 : 1080 - pos.y;
	const Utils::Vector2f correctedPosition{ x,y };

	collider.UpdateColliderBounds(Geometry::Circle{ correctedPosition, static_cast<float>(size.x)/2.f });

	if (physics.CheckCollisionOnCollider(collider))
	{
		collider.UpdateColliderBounds(Geometry::Circle{ position, static_cast<float>(size.x)/2.f });
		return;
	}

	position = correctedPosition;
}


void GameObject::Spaceship::Update(float deltatime)
{

	// Movement logic
	float movement{};

	if (IsKeyDown(KEY_A))	rotation -= rotationSpeed * deltatime;
	else if (IsKeyDown(KEY_D))	rotation += rotationSpeed * deltatime;


	if (IsKeyDown(KEY_W))movement = -1;
	else if (IsKeyDown(KEY_S))	movement = 1;
	else movement = 0;

	if (IsKeyPressed(KEY_SPACE)) SpawnBullet();

	const auto forwardVector = Geometry::ForwardVector(rotation);
	const Utils::Vector2f deltaMovement = forwardVector * speed * movement * deltatime;

	const auto desiredPosition = position + deltaMovement;
	SetPosition(desiredPosition);

	// Invulerability logic
	invulnerableTime = std::max(invulnerableTime - deltatime, -1.f);

	// Update bullets
	std::for_each(bullets.begin(), bullets.end(), [deltatime](Bullet& b) {b.Update(deltatime); });
}

void GameObject::Spaceship::OnCollision()
{
	if (invulnerableTime > 0)return;

	gManager.GetManager<GameplayManager>().UpdateHealth(-1);
	StartInvulnerability();
}


void GameObject::Spaceship::Draw()
{
	static Rectangle textureQuad{ 0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height) };
	// Add the effect of flickering when invulnerable
	if (invulnerableTime < 0 || static_cast<int>(invulnerableTime * 10) % 2) {
		Rectangle renderQuad{ position.x, position.y, static_cast<float>(size.x),static_cast<float> (size.y) };
		DrawTexturePro(*texture, textureQuad, renderQuad, { size.x / 2.f, size.y / 2.f }, rotation, WHITE);
	}

	std::for_each(bullets.begin(), bullets.end(), [](const Bullet& b) {b.Draw(); });
}

void GameObject::Spaceship::StartInvulnerability(float time)
{
	invulnerableTime = time;
}


void GameObject::Spaceship::SpawnBullet()
{
	const Utils::Vector2f bulletOffset = Geometry::ForwardVector(rotation) * (-size.x * 0.8f);
	const Utils::Vector2f bulletSize{ 10,25 };
	const Utils::Vector2f bulletMovement = Geometry::ForwardVector(rotation) * -speed * 1.5f;
	const BulletTransform transform{ bulletOffset + position, bulletSize, bulletMovement, rotation };
	auto it = std::find_if(bullets.begin(), bullets.end(), [](const Bullet& b) {return !b.Active(); });

	if (it == bullets.end())
	{
		bullets.emplace_back(GameObjectFactory::MakeGameObject<Bullet>(transform));
	}
	else
	{
		*it = GameObjectFactory::MakeGameObject<Bullet>(transform);
	}

	// Play sound
	PlaySound(*shootingSound);
}

void GameObject::Spaceship::RegisterCollider()
{
	collider = physics.RegisterCollider<Geometry::Circle>(*this, Geometry::Point{ position }, static_cast<float>(size.x));
}

void GameObject::Spaceship::UnregisterCollider()
{
	if (!collider.Valid()) return;
	physics.UnregisterCollider(collider);
}
