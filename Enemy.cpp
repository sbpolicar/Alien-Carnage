//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Enemy.h"
#include "Sprite.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Enemy::Enemy(DOUBLE2 pos, DOUBLE2 actSize, Sprite* spriteRef)
{
}

Enemy::~Enemy()
{

	// Clean up physicsactor
	delete m_ActPtr;
	m_ActPtr = nullptr;

}

void Enemy::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	// Check if we touched the player, and if so damage him
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		std::cout << "enemy touched player" << std::endl;
	}

}
