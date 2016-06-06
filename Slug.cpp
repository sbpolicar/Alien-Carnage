//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Slug.h"
#include "Sludge.h"
#include "Sprite.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Slug::Slug(DOUBLE2 pos) : Enemy(pos, DOUBLE2(39, 11))
{

	// Create the sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Move"), String("./resources/images/characters/slug/anim_move.png"), 0, 0, 1, 5, 8);
	// Set animation
	m_SpritePtr->SetAnimation(String("Move"));

	m_ActPtr->SetUserPointer(this);

}

Slug::~Slug()
{

	// On death, spawn sludge
	if (m_Health <= 0)
	{
		new Sludge(m_ActPtr->GetPosition());
	}

}
