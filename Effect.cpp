//-----------------------------------------------------------------
// Student data
// Name: Brent Van Handenhove
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Effect.h"
#include "Sprite.h"
#include "Character.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Effect::Effect(DOUBLE2 pos, Sprite* spritePtr, double lifetime) : Entity()
{

	// Position
	m_Position = pos;

	// Sprite
	m_SpritePtr = spritePtr->Copy();

	// Lifetime
	m_LifeTime = lifetime;

}

Effect::Effect(Sprite * spritePtr, double lifetime)
{

	m_TrackPlayer = true;

	// Sprite
	m_SpritePtr = spritePtr->Copy();

	// Lifetime
	m_LifeTime = lifetime;

}

Effect::~Effect()
{

}

void Effect::Tick(double dTime)
{

	m_SpritePtr->Tick(dTime);

	// Lifetime destroy
	m_TimeLived += dTime;
	if (m_TimeLived >= m_LifeTime)
	{
		DestroyEntity();
	}

	// Flagdestroy check, just in case (normally won't need this)
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}

void Effect::Paint()
{

	// If the effect didnt get a position with it, we're going to assume it's meant to follow the player
	// This is because all effects get a position if they need one, so any that don't are just meant to track
	// Such as the spawning 'glitter' effect

	if (m_TrackPlayer)
	{
		m_SpritePtr->Paint(ENT_MANAGER->GetPlayer()->GetPosition());
	}
	else
	{
		m_SpritePtr->Paint(m_Position);
	}

}
