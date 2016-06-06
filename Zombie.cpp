//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Zombie.h"
#include "Sprite.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Zombie::Zombie(DOUBLE2 pos) : Enemy(pos, DOUBLE2(26, 36))
{

	// Sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Walk"), String("./resources/images/characters/zombie/anim_walk.png"), 0, 0, 1, 4, 6);
	m_SpritePtr->SetAnimation(String("Walk"));

	m_ActPtr->SetUserPointer(this);

}

Zombie::~Zombie()
{

}
