//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Sprite.h"
#include "Finish.h"
#include "Character.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Finish::Finish(DOUBLE2 pos) : Entity(pos, DOUBLE2(48, 48), BodyType::STATIC)
{

	// Actor sensor
	m_ActPtr->SetSensor(true);
	m_ActPtr->AddContactListener(this);

	// Sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Closed"), String("./resources/images/entities/finish/closed.png"));
	m_SpritePtr->AddAnimation(String("Open"), String("./resources/images/entities/finish/open.png"));
	m_SpritePtr->AddAnimation(String("Opening"), String("./resources/images/entities/finish/opening.png"), 0, 0, 1, 5, 5);
	m_SpritePtr->SetAnimation(String("Closed"));

}

Finish::~Finish()
{

}

void Finish::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	if (m_Unlocked)
	{
		if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
		{
			// Player has won
			ENT_MANAGER->GetPlayer()->Win();
			GAME_ENGINE->SetPaused(true);
		}
	}

}

void Finish::Tick(double dTime)
{

	if (!m_Unlocked)
	{
		if (ENT_MANAGER->GetCaptiveCount() <= 0)
		{
			// Unlock the finish
			m_Unlocked = true;
			m_SpritePtr->SetAnimation(String("Opening"));
		}
	}
	else if (!m_Open)
	{
		if (m_OpenAccuTime > m_OpeningTime)
		{
			m_SpritePtr->SetAnimation(String("Open"));
			m_Open = true;
		}
		else
		{
			m_OpenAccuTime += dTime;
		}
	}

}
