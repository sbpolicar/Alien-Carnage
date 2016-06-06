//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Projectile.h"
#include "Sprite.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "Level.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())

Projectile::Projectile(DOUBLE2 pos, Sprite* spritePtr, DOUBLE2 vel, DOUBLE2 hitSize, double lifeTime) : Entity(pos, hitSize, BodyType::KINEMATIC, spritePtr)
{

	// Initiate the physicsactor
	m_ActPtr->SetGravityScale(0.0); // Projectiles aren't affected by gravity
	m_ActPtr->SetLinearVelocity(vel);
	m_ActPtr->SetSensor(true);

	// Listen for contact
	m_ActPtr->AddContactListener(this);

	// Lifetime and self-destroy
	m_LifeTime = lifeTime;

}

Projectile::Projectile(DOUBLE2 pos, Sprite* spriteRef, DOUBLE2 vel, DOUBLE2 hitSize, double lifeTime, double damage) : Entity(pos, hitSize, BodyType::KINEMATIC, spriteRef)
{
	
	// Initiate the physicsactor
	m_ActPtr->SetGravityScale(0.0); // Projectiles aren't affected by gravity
	m_ActPtr->SetLinearVelocity(vel);
	m_ActPtr->SetSensor(true);

	// Listen for contact
	m_ActPtr->AddContactListener(this);

	// Lifetime and self-destroy
	m_LifeTime = lifeTime;

	// Damage
	m_Damage = damage;

}

Projectile::~Projectile()
{

}

void Projectile::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	// Check if the other actor is an enemy / the world / an entity or whatever else should take damage
	// So anything but the player, really
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		// It's the player, ignore
		return;
	}
	else if (actOtherPtr->GetUserData() == int(EntityType::ENEMY))
	{
		// It's an enemy, deal damage
		Enemy* enemyPtr = static_cast<Enemy*>(actOtherPtr->GetUserPointer());
		if (enemyPtr != nullptr)
		{
			// Check if we should destroy ourselves or pass through
			if (enemyPtr->Hit(m_ActPtr->GetPosition().x, m_Damage))
			{
				// Destroy the projectile
				FlagDestroy();
			}
		}
	}

}

void Projectile::Tick(double dTime)
{

	m_SpritePtr->Tick(dTime);

	// Collision with level destroys the projectile
	if (ENT_MANAGER->GetLevel()->IsOnGround(m_ActPtr))
	{
		FlagDestroy();
	}

	// Lifetime destroy
	m_TimeLived += dTime;
	if (m_TimeLived >= m_LifeTime)
	{
		DestroyEntity();
	}

	// Flagdestroy check
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}
