//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Trigger.h"
#include "Sprite.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Trigger::Trigger(DOUBLE2 pos, DOUBLE2 doorPos) : Entity(pos, m_ActSize, BodyType::STATIC)
{
	
	// The sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("On"), String("./resources/images/entities/lever/on.png"));
	m_SpritePtr->AddAnimation(String("Off"), String("./resources/images/entities/lever/off.png"));
	m_SpritePtr->SetAnimation(String("Off"));

	// Physicsactor for the trigger
	m_ActPtr->AddBoxFixture(20, 24, 0.0, 0.0);
	m_ActPtr->SetSensor(true);
	m_ActPtr->AddContactListener(this);

	// Create the door
	m_SpriteDoorPtr = new Sprite();
	m_SpriteDoorPtr->AddAnimation(String("Idle"), String("./resources/images/entities/door.png"));
	m_SpriteDoorPtr->SetAnimation(String("Idle"));

	m_EntDoorPtr = new Entity(doorPos, DOUBLE2(16, 48), BodyType::STATIC, m_SpriteDoorPtr);

}

Trigger::~Trigger()
{
	
	// Remove door sprite
	delete m_SpriteDoorPtr;
	m_SpriteDoorPtr = nullptr;
	
	Entity::~Entity();
		
}

void Trigger::Activate()
{
	
	// Only activate once (may add support for multiple triggers later, no need for it right now)
	if (!m_Activated)
	{
		// Delete the door
		m_EntDoorPtr->FlagDestroy();
		m_SpritePtr->SetAnimation(String("On"));
		m_Activated = !m_Activated;
	}

}

void Trigger::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{
	
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		// Only activate if the player runs into this trigger
		Activate();
	}

}

void Trigger::Paint()
{

	m_SpritePtr->Paint(m_ActPtr->GetPosition());

}