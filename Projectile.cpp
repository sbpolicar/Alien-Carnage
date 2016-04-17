//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Projectile.h"
#include "Sprite.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Projectile::Projectile(DOUBLE2 pos, Sprite* spriteRef, DOUBLE2 vel, DOUBLE2 hitSize, double lifeTime) : Entity(pos, hitSize, BodyType::KINEMATIC, spriteRef)
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

Projectile::~Projectile()
{
}

void Projectile::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	// Check if the other actor is an enemy / the world / an entity or whatever else should take damage
	// WIP
	std::cout << "HIT" << std::endl;

	// Destroy this projectile

}

void Projectile::Tick(double dTime)
{

	m_SpritePtr->Tick(dTime);

	m_TimeLived += dTime;
	if (m_TimeLived >= m_LifeTime)
	{
		DestroyEntity();
	}

}

void Projectile::Paint()
{

	// Draw the projectile sprite
	m_SpritePtr->Paint(m_ActPtr->GetPosition());

}

