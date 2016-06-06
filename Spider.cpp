//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Spider.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "Character.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Spider::Spider(DOUBLE2 pos) : Enemy(pos, DOUBLE2(45, 16))
{

	// Create the sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Move"), String("./resources/images/characters/spider/anim_move.png"), 0, 0, 1, 6, 6);
	m_SpritePtr->AddAnimation(String("Attack"), String("./resources/images/characters/spider/anim_attack.png"), 0, 0, 1, 1, 1);
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/characters/spider/anim_idle.png"), 0, 0, 1, 1, 1);
	// Set animation
	m_SpritePtr->SetAnimation(String("Move"));

	// Override inherited stuff
	m_MoveSpeed = DOUBLE2(35, 0);
	m_ScoreReward = 600;
	m_Health = 50;
	m_ActPtr->SetLinearVelocity(m_MoveSpeed);

	m_ActPtr->SetUserPointer(this);
	m_ActPtr->SetGravityScale(-1.0);

	m_EdgeDetectionOffset = DOUBLE2(m_EdgeDetectionOffset.x, -8);

}

Spider::~Spider()
{
	
}

void Spider::HandleMovement(double dTime)
{

	// A spider will move left or right depending on where the player is, trying to get above the player to attack them
	// If it happens to be at a boundary (eg next to an edge) it will simply stop until the player causes it to move the other way again
	// When the player gets under the spider, it will attack by biting down

	if (!m_IsBiting)
	{

		// We only care about the x positions of the player and the spider
		DOUBLE2 playerPos = ENT_MANAGER->GetPlayer()->GetPosition();
		DOUBLE2 spiderPos = m_ActPtr->GetPosition();
		int wallSide = DetectEdges();

		m_ActPtr->SetLinearVelocity(DOUBLE2(0.0, 0.0));

		if (((playerPos.x + m_PlayerTrackingLenience >= spiderPos.x) // x position needs to be close enough
			&& (playerPos.x - m_PlayerTrackingLenience <= spiderPos.x))
			&& (abs(playerPos.y - spiderPos.y) <= m_BiteYRange)) // y position needs to be close enough too
		{
			// Bite the player
			m_ActPtr->SetLinearVelocity(DOUBLE2(0, 0));
			m_SpritePtr->SetAnimation(String("Attack"));
			m_IsBiting = true;
			ENT_MANAGER->GetPlayer()->TakeDamage(m_BiteDamage);
			// Bite animation becomes active
			m_BitingAnimationStopped = false;
		}
		else if (abs(spiderPos.x - playerPos.x) <= m_PlayerTrackingLenience)
		{
			// Something's up, stop moving
			// Could be that we're near an edge, or that the player is already at the same X position
			m_ActPtr->SetLinearVelocity(DOUBLE2(0, 0));
			m_SpritePtr->SetAnimation(String("Idle"));
		}
		else if (playerPos.x < spiderPos.x && (wallSide != int(EdgeLocation::LEFT)))
		{
			// Player is to the left, move left
			m_SpritePtr->SetAnimation(String("Move"));
			m_SpritePtr->Flipped(true);
			m_ActPtr->SetLinearVelocity(-m_MoveSpeed);
			m_CurrentState = EnemyState::MOVINGLEFT;
		}
		else if (playerPos.x > spiderPos.x && (wallSide != int(EdgeLocation::RIGHT)))
		{
			// Player is to the right, move right
			m_SpritePtr->SetAnimation(String("Move"));
			m_SpritePtr->Flipped(false);
			m_ActPtr->SetLinearVelocity(m_MoveSpeed);
			m_CurrentState = EnemyState::MOVINGRIGHT;
		}
		else
		{
			// If none of that worked, the player is probably hiding behind a wall or something, like the coward he is
			// Idle and stare at them creepily whilst taunting and dissing their mother
			m_ActPtr->SetLinearVelocity(DOUBLE2(0, 0));
			m_SpritePtr->SetAnimation(String("Idle"));
		}
	}

	else 
	{
		// Count until "unfreeze"
		m_DelayAfterBitingAccuTime += dTime;

		// Only keep the animation for a bit, we want the player to see that he has a chance to run away now
		if (!m_BitingAnimationStopped && m_DelayAfterBitingAccuTime >= m_DelayBitingAnimationGone)
		{
			m_BitingAnimationStopped = true;
			m_SpritePtr->SetAnimation(String("Idle"));
		}
		// Counting for when the spider "unfreezes" out of its attack, ready to strike again
		if (m_DelayAfterBitingAccuTime >= m_DelayAfterBiting)
		{
			m_IsBiting = false;
			m_DelayAfterBitingAccuTime = 0.0;
		}

	}

}

void Spider::Tick(double dTime)
{

	// Spider overrides this because spiders do not deal damage if you just run into them (only from biting)

	// Tick sprite
	m_SpritePtr->Tick(dTime);

	// Movement
	HandleMovement(dTime);

	// Check if we need to detroy ourselves
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}