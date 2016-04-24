//-----------------------------------------------------------------
// Student data
// Name:
// Group: 1DAE..
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Captive.h"
#include "Sprite.h"
#include "Character.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Captive::Captive(DOUBLE2 pos) : Entity(pos, DOUBLE2(30, 34), BodyType::STATIC)
{
	
	// Actor is a sensor listening for contact
	m_ActPtr->SetSensor(true);
	m_ActPtr->AddContactListener(this);

	// The sprite
	m_SpritePtr = new Sprite();

	// Male or female? Random to keep it interesting
	if (rand() % 2 == 0)
	{
		// Male
		m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/captive/captive_male_idle.png"));
		m_SpritePtr->AddAnimation(String("Rescue"), String("./resources/images/entities/captive/captive_male_rescue.png"), 0, 0, 1, 6, 6);
	}
	else
	{
		// Female
		m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/captive/captive_female_idle.png"));
		m_SpritePtr->AddAnimation(String("Rescue"), String("./resources/images/entities/captive/captive_female_rescue.png"), 0, 0, 1, 9, 9);
	}

	// Start idle
	m_SpritePtr->SetAnimation(String("Idle"));

}

Captive::~Captive()
{

	// Delete actor and sprite
	delete m_ActPtr;
	m_ActPtr = nullptr;
	delete m_SpritePtr;
	m_SpritePtr = nullptr;

}

void Captive::Tick(double dTime)
{

	m_SpritePtr->Tick(dTime);

	// Tick for removal after the rescue animation
	if (m_Rescued)
	{
		m_AccuTime += dTime;
		if (m_AccuTime >= m_LiveTimeAfterRescue)
		{
			DestroyEntity();
		}
	}

}

void Captive::Paint()
{

	m_SpritePtr->Paint(m_ActPtr->GetPosition());

}

void Captive::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{
	
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER) && !m_Rescued)
	{
		// Play the rescue animation
		m_SpritePtr->SetAnimation(String("Rescue"));
		m_Rescued = true;
		// Give the player score
		ENT_MANAGER->GetPlayer()->GiveScore(m_ScoreReward);
	}

}
