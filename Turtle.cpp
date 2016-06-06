//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Turtle.h"
#include "Sprite.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Turtle::Turtle(DOUBLE2 pos) : Enemy(pos, DOUBLE2(34, 35))
{
	
	// Create the sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Move"), String("./resources/images/characters/turtle/anim_move.png"), 0, 0, 1, 4, 5);
	// Set animation
	m_SpritePtr->SetAnimation(String("Move"));

	// Override inherited stuff
	m_MoveSpeed = DOUBLE2(20, 0);
	m_ScoreReward = 750;
	m_Health = 150;	
	m_ActPtr->SetLinearVelocity(m_MoveSpeed);

	m_ActPtr->SetUserPointer(this);

}

Turtle::~Turtle()
{

}

bool Turtle::Hit(double xPos, double amount)
{

	// Play the sound
	m_SndHitPtr->Play();

	// Turtle only takes damage from the front, not from behind
	if ((m_CurrentState == EnemyState::MOVINGRIGHT && xPos >= m_ActPtr->GetPosition().x)
		|| (m_CurrentState == EnemyState::MOVINGLEFT && xPos <= m_ActPtr->GetPosition().x))
	{
		m_Health -= amount;

		// Check if we died
		if (m_Health <= 0)
		{
			// We died
			FlagDestroy();
		}

		return false;
	}

	return true;

}
