//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Enemy.h"
#include "Sprite.h"
#include "Character.h"
#include "EntityManager.h"
#include "Level.h"
#include "Coin.h"
#include "Effect.h"
#include "EnemySpawner.h"
#include "ResourceManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())
#define ENT_MANAGER (EntityManager::GetInstance())

Enemy::Enemy(DOUBLE2 pos, DOUBLE2 actSize) : Entity(pos, actSize, BodyType::DYNAMIC)
{
	
	// Main physicsactor
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetUserData(int(EntityType::ENEMY));
	m_ActPtr->SetFixedRotation(true);

	// Sounds
	m_SndDeathPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/baddie_death.wav"));

	// Sides for wall and edge detection
	m_EdgeDetectionOffset = DOUBLE2(actSize.x / 2, actSize.y / 2);
	m_WallDetectionOffset = DOUBLE2(actSize.x / 2, 0);

	// Sides for edge detection
	m_ActWallPtr = new PhysicsActor(m_WallDetectionOffset, 0, BodyType::KINEMATIC);
	m_ActWallPtr->AddBoxFixture(6, 4, 0.0, 0.0);
	m_ActWallPtr->SetSensor(true);
	m_ActWallPtr->SetUserData(int(EntityType::ENEMY));
	m_ActEdgePtr = new PhysicsActor(m_EdgeDetectionOffset, 0, BodyType::KINEMATIC);
	m_ActEdgePtr->AddBoxFixture(2, 4, 0.0, 0.0);
	m_ActEdgePtr->SetSensor(true);
	m_ActEdgePtr->SetUserData(int(EntityType::ENEMY));

	// Start moving 
	m_ActPtr->SetLinearVelocity(m_MoveSpeed);

	// Sound for hitting
	m_SndHitPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/zhit_h.wav"));

	// Effect on death
	m_SpriteDeathEffectPtr = new Sprite();
	m_SpriteDeathEffectPtr->AddAnimation(String("Explode"), String("./resources/images/effects/enemy_death.png"), 0, 0, 1, 8, 8);
	m_SpriteDeathEffectPtr->SetAnimation(String("Explode"));

}

Enemy::~Enemy()
{

	// If we got destroyed because we died (not because game shuts down), then our health will be 0
	// Reward the player
	if (m_Health <= 0)
	{
		// Create the death effect
		new Effect(m_ActPtr->GetPosition(), m_SpriteDeathEffectPtr, 1.0);
		// Play the death sound
		m_SndDeathPtr->Play();
		// Spawn a coin on death
		new Coin(m_ActPtr->GetPosition());
		// Give the player points
		ENT_MANAGER->GetPlayer()->GiveScore(m_ScoreReward);
		// Tell the spawner we died
		m_EnemySpawnerPtr->EnemyDied();
	}

	// Clean up physicsactor and sprite
	delete m_ActPtr;
	m_ActPtr = nullptr;

	delete m_SpritePtr;
	m_SpritePtr = nullptr;
	delete m_SpriteDeathEffectPtr;
	m_SpriteDeathEffectPtr = nullptr;

	// Delete our edge & wall detecting actors
	delete m_ActWallPtr;
	m_ActWallPtr = nullptr;
	delete m_ActEdgePtr;
	m_ActEdgePtr = nullptr;

}

bool Enemy::Hit(double xPos, double amount)
{

	// Play the sound
	m_SndHitPtr->Play();

	m_Health -= amount;
	
	// Check if we died
	if (m_Health <= 0)
	{
		// We died
		FlagDestroy();
	}

	// By default, this returns false, meaning that the projectile will not destroy itself
	// For example, the turtle's shield will make projectiles stop and get destroyed, so that would return true
	return false;

}

void Enemy::SetSpawner(EnemySpawner * enemySpawnerRef)
{

	if (enemySpawnerRef != nullptr)
	{
		m_EnemySpawnerPtr = enemySpawnerRef;
	}

}

DOUBLE2 Enemy::GetPosition()
{

	return DOUBLE2(m_ActPtr->GetPosition());

}

void Enemy::PreSolve(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr, bool& enableContactRef)
{

	// Allows stuff to move through the enemy
	if (actOtherPtr->GetUserData() != int(EntityType::WORLD))
	{
		enableContactRef = false;
	}

}

void Enemy::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	// Check if we touched the player, and if so start damaging him
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		m_IsTouchingPlayer = true;
	}

}

void Enemy::EndContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	// Check if this is the player
	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		m_IsTouchingPlayer = false; // Stop damaging
		m_DamageAccuTime = m_DamageInterval; // Reset this so next touch will instantly do damage again
	}

}

int Enemy::DetectEdges()
{

	Level* levelRef = ENT_MANAGER->GetLevel();

	if ((levelRef->IsOnGround(m_ActWallPtr) || !levelRef->IsOnGround(m_ActEdgePtr)) && m_CurrentState != EnemyState::ATTACKING)
	{
		if (m_CurrentState == EnemyState::MOVINGRIGHT)
		{
			// Right side is bumping into a wall or near an edge
			return int(EdgeLocation::RIGHT);
		}
		else if (m_CurrentState == EnemyState::MOVINGLEFT)
		{
			// Left side is bumping into a wall or near an edge
			return int(EdgeLocation::LEFT);
		}
	}

	// No wall
	return int(EdgeLocation::NONE);

}

void Enemy::HandleMovement(double dTime)
{

	// By default, enemies just move from left to right
	// Each enemy can have unique behaviour by overriding this function

	int wallSide = DetectEdges();

	if ((wallSide == int(EdgeLocation::LEFT) && m_CurrentState == EnemyState::MOVINGLEFT) || (m_CurrentState == EnemyState::MOVINGLEFT && m_ActPtr->GetLinearVelocity().x > 0))
	{
		// There is a wall on our left, switch to move right
		// Or somehow we are moving to the right, in which case update animations and state
		m_SpritePtr->Flipped(false);
		m_ActPtr->SetLinearVelocity(m_MoveSpeed);
		m_CurrentState = EnemyState::MOVINGRIGHT;
	}
	else if ((wallSide == int(EdgeLocation::RIGHT) && m_CurrentState == EnemyState::MOVINGRIGHT) || (m_CurrentState == EnemyState::MOVINGRIGHT && m_ActPtr->GetLinearVelocity().x < 0))
	{
		// There is a wall on our right, switch to move left
		// Or somehow we are moving to the left, in which case update animations and state
		m_SpritePtr->Flipped(true);
		m_ActPtr->SetLinearVelocity(m_MoveSpeed*-1);
		m_CurrentState = EnemyState::MOVINGLEFT;
	}

}

void Enemy::Tick(double dTime)
{

	// Tick sprite
	m_SpritePtr->Tick(dTime);

	// Damage the player if we're touching him
	if (m_IsTouchingPlayer)
	{
		m_DamageAccuTime += dTime;
		if (m_DamageAccuTime >= m_DamageInterval)
		{
			m_DamageAccuTime -= m_DamageInterval;
			ENT_MANAGER->GetPlayer()->TakeDamage(m_Damage); // Deal damage every interval (as long as player touches the enemy)
		}
	}

	HandleMovement(dTime);

	// Check if we need to destroy ourselves
	if (m_ShouldDestroy)
	{
		DestroyEntity();
	}

}

void Enemy::Paint()
{

	// Paint sprite
	m_SpritePtr->Paint(m_ActPtr->GetPosition());

	// Reposition our edge detectors
	// If we do this in tick, they visually 'lag'
	// The detectors rotate with the sprite flipping (to reduce amount of needed detectors)
	if (m_CurrentState == EnemyState::MOVINGRIGHT)
	{
		m_ActWallPtr->SetPosition(m_ActPtr->GetPosition() + m_WallDetectionOffset);
		m_ActEdgePtr->SetPosition(m_ActPtr->GetPosition() + m_EdgeDetectionOffset);
	}
	else if (m_CurrentState == EnemyState::MOVINGLEFT)
	{
		m_ActWallPtr->SetPosition(m_ActPtr->GetPosition() + DOUBLE2(-m_WallDetectionOffset.x, m_WallDetectionOffset.y));
		m_ActEdgePtr->SetPosition(m_ActPtr->GetPosition() + DOUBLE2(-m_EdgeDetectionOffset.x, m_EdgeDetectionOffset.y));
	}

}
