//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Sludge.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "Character.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Sludge::Sludge(DOUBLE2 pos) : Entity(pos, DOUBLE2(30, 16), BodyType::DYNAMIC)
{

	// Actor
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetFixedRotation(true);
	m_ActPtr->SetUserData(int(EntityType::ENEMY));

	// Sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/sludge.png"), 0, 1, 2, 8, 5);
	m_SpritePtr->SetAnimation(String("Idle"));

}

Sludge::~Sludge()
{

}

void Sludge::PreSolve(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr, bool& enableContactRef)
{

	if (actOtherPtr->GetUserData() == int(EntityType::WORLD))
	{
		enableContactRef = true;
	}
	else
	{
		enableContactRef = false;
	}

}

void Sludge::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		m_TouchingPlayer = true; // Stop damaging
		m_DamageAccuTime = m_DamageInterval; // Reset this so next touch will instantly do damage again
	}

}

void Sludge::EndContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		m_TouchingPlayer = false; // Stop damaging
	}

}

void Sludge::Tick(double dTime)
{

	// Tick sprite
	m_SpritePtr->Tick(dTime);

	// Damage the player if we're touching him
	if (m_TouchingPlayer)
	{
		m_DamageAccuTime += dTime;
		if (m_DamageAccuTime >= m_DamageInterval)
		{
			m_DamageAccuTime -= m_DamageInterval;
			ENT_MANAGER->GetPlayer()->TakeDamage(m_Damage); // Deal damage every interval (as long as player touches the enemy)
		}
	};

	// Should we destroy ourselves?
	m_LifeAccuTime += dTime;
	if (m_LifeAccuTime > m_LifeTime)
	{
		FlagDestroy();
	}

	// Check if we need to destroy ourselves
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}
