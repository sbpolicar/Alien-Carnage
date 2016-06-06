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

Entity::Entity()
{

	// In rare cases (eg coins, effects), the physicsactor needs to do something special
	// Or there is no physics actor at all
	// For that, we have an empty constructor so the entity can determine its own actor behavior

	ENT_MANAGER->AddEntity(this);

}

Entity::Entity(DOUBLE2 pos, DOUBLE2 actSize, BodyType actBodyType)
{

	// In some cases our entity handles its own sprites
	// Create our physicsactor
	m_ActPtr = new PhysicsActor(pos, 0, actBodyType);
	m_ActPtr->AddBoxFixture(actSize.x, actSize.y, 0.0, 0.0);
	m_ActPtr->SetUserData(int(EntityType::WORLD));

	ENT_MANAGER->AddEntity(this);

}

Entity::Entity(DOUBLE2 pos, DOUBLE2 actSize, BodyType actBodyType, Sprite* spritePtr)
{

	// Create our physicsactor
	m_ActPtr = new PhysicsActor(pos, 0, actBodyType);
	m_ActPtr->AddBoxFixture(actSize.x, actSize.y, 0.0, 0.0);
	m_ActPtr->SetUserData(int(EntityType::WORLD));

	// Copy the sprite
	m_SpritePtr = spritePtr->Copy();

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