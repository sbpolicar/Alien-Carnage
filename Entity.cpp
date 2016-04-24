//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Entity.h"
#include "Sprite.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Entity::Entity(DOUBLE2 pos, DOUBLE2 actSize, BodyType actBodyType, Sprite* spriteRef)
{

	// Create our physicsactor
	m_ActPtr = new PhysicsActor(pos, 0, actBodyType);
	m_ActPtr->AddBoxFixture(actSize.x, actSize.y, 0.0, 0.0);

	// Copy the sprite
	m_SpritePtr = spriteRef->Copy();

	ENT_MANAGER->AddEntity(this);

}


Entity::Entity(DOUBLE2 pos, DOUBLE2 actSize, BodyType actBodyType)
{

	// Create our physicsactor
	m_ActPtr = new PhysicsActor(pos, 0, actBodyType);
	m_ActPtr->AddBoxFixture(actSize.x, actSize.y, 0.0, 0.0);
	
	ENT_MANAGER->AddEntity(this);

}

Entity::~Entity()
{

	// Clean up
	delete m_SpritePtr;
	m_SpritePtr = nullptr;

	delete m_ActPtr;
	m_ActPtr = nullptr;

}

void Entity::FlagDestroy()
{

	m_ShouldDestroy = true;

}

void Entity::DestroyEntity()
{

	// Tell the entity manager to destroy us
	ENT_MANAGER->RemoveEntity(this);

}

void Entity::Tick(double dTime)
{
	
	// Tick for our sprite
	m_SpritePtr->Tick(dTime);

	// Destroy if needed
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}

void Entity::Paint()
{
	
	// Draw our entity sprite
	m_SpritePtr->Paint(m_ActPtr->GetPosition());

}