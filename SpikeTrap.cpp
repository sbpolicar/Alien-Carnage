//-----------------------------------------------------------------
// Student data
// Name: Brent Van Handenhove
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Sprite.h"
#include "SpikeTrap.h"
#include "Character.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

SpikeTrap::SpikeTrap(DOUBLE2 pos) : Entity(pos, DOUBLE2(47, 26), BodyType::STATIC)
{

	// Actor
	m_ActPtr->SetSensor(true);
	m_ActPtr->AddContactListener(this);

	// Sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/spiketrap/inactive.png"));
	m_SpritePtr->AddAnimation(String("Deploy"), String("./resources/images/entities/spiketrap/spike_trap.png"), 0, 0, 1, 4, 8);
	m_SpritePtr->SetAnimation(String("Idle"));

}

SpikeTrap::~SpikeTrap()
{

}

void SpikeTrap::BeginContact(PhysicsActor * actThisPtr, PhysicsActor * actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		m_TouchingPlayer = true;
	}

}

void SpikeTrap::EndContact(PhysicsActor * actThisPtr, PhysicsActor * actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		m_TouchingPlayer = false;
	}

}


void SpikeTrap::Tick(double dTime)
{

	// Sprite tick
	m_SpritePtr->Tick(dTime);

	// Waiting and deploying

	m_WaitAccuTime += dTime;

	if (!m_Deployed)
	{
		if (m_WaitAccuTime >= m_WaitTime)
		{
			m_Deployed = true;
			m_WaitAccuTime = 0.0;
			m_SpritePtr->SetAnimation(String("Deploy"));
			// If the player was above us at this point, damage him
			if (m_TouchingPlayer)
			{
				ENT_MANAGER->GetPlayer()->TakeDamage(m_Damage);
			}
		}
	}
	else
	{
		if (m_WaitAccuTime >= m_DeployTime)
		{
			m_Deployed = false;
			m_WaitAccuTime = 0.0;
			m_SpritePtr->SetAnimation(String("Idle"));
		}
	}

	// Destroy if needed
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}
