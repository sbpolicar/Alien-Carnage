//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Captive.h"
#include "Sprite.h"
#include "Character.h"
#include "EntityManager.h"
#include "ResourceManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())

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
		m_SpritePtr->AddAnimation(String("Rescue"), String("./resources/images/entities/captive/captive_female_rescue.png"), 0, 0, 1, 9, 6);
		m_LiveTimeAfterRescue = 1.4;
	}

	// Start idle
	m_SpritePtr->SetAnimation(String("Idle"));

	// Add to the counter
	ENT_MANAGER->AddCaptive();

	// Sound
	m_SndRescuedPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/captive.wav"));

}

Captive::~Captive()
{
	
	// Delete actor and sprite
	delete m_ActPtr;
	m_ActPtr = nullptr;
	delete m_SpritePtr;
	m_SpritePtr = nullptr;

	// Remove from the counter
	ENT_MANAGER->RemoveCaptive();

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

void Captive::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{
	
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER) && !m_Rescued)
	{
		m_Rescued = true;

		// Play the rescue animation
		m_SpritePtr->SetAnimation(String("Rescue"));

		// Play the sound
		m_SndRescuedPtr->Play();

		// Give the player score
		ENT_MANAGER->GetPlayer()->GiveScore(m_ScoreReward);
	}

}
