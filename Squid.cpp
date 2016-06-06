//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Squid.h"
#include "Sprite.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Squid::Squid(DOUBLE2 pos) : Enemy(pos, DOUBLE2(25, 28))
{

	// Create the sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Move"), String("./resources/images/characters/squid/anim_move.png"), 0, 0, 1, 5, 5);
	// Set animation
	m_SpritePtr->SetAnimation(String("Move"));

	m_ActPtr->SetUserPointer(this);

}

Squid::~Squid()
{
	
}