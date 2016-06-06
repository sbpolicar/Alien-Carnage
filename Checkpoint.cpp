//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Checkpoint.h"
#include "Sprite.h"
#include "Character.h"
#include "EntityManager.h"
#include "ResourceManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())

Checkpoint::Checkpoint(DOUBLE2 pos) : Entity(pos, DOUBLE2(25, 36), BodyType::STATIC)
{

	// Sensor
	m_ActPtr->SetSensor(true);
	m_ActPtr->AddContactListener(this);

	// Sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/terminal/terminal_disactivated.png"), 0, 0, 1, 4, 4);
	m_SpritePtr->AddAnimation(String("Active"), String("./resources/images/entities/terminal/terminal_activated.png"));
	m_SpritePtr->SetAnimation(String("Idle"));

	// Activation sound
	m_SndActivatePtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/terminal.wav"));

}

Checkpoint::~Checkpoint()
{

}

void Checkpoint::Disactivate()
{

	m_Activated = false;
	m_SpritePtr->SetAnimation(String("Idle"));

}

DOUBLE2 Checkpoint::GetPosition()
{

	return m_ActPtr->GetPosition();

}

void Checkpoint::BeginContact(PhysicsActor * actThisPtr, PhysicsActor * actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		Activate();
	}

}

void Checkpoint::Activate()
{

	if (!m_Activated)
	{
		m_Activated = true;
		m_SndActivatePtr->Play(); // Play the sound
		m_SpritePtr->SetAnimation(String("Active"));
		// Set this as the new checkpoint for the player
		ENT_MANAGER->GetPlayer()->SetCheckpoint(this);
	}

}
