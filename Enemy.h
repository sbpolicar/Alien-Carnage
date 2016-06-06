#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Sprite;
class EnemySpawner;

class Enemy : public Entity, ContactListener
{
public:

	Enemy(DOUBLE2 pos, DOUBLE2 actSize);
	virtual ~Enemy();

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Enemy( const Enemy& ) = delete;
	Enemy& operator=( const Enemy& ) = delete;

	virtual bool Hit(double xPos, double amount);
	void SetSpawner(EnemySpawner* enemySpawnerRef);

	DOUBLE2 GetPosition();

	void Tick(double dTime);
	void Paint();

	void PreSolve(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr, bool& enableContactRef);
	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);
	void EndContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

protected: 
	
	enum EnemyState
	{
		MOVINGLEFT, MOVINGRIGHT, ATTACKING
	};

	enum EdgeLocation
	{
		LEFT, RIGHT, NONE
	};

	virtual int DetectEdges();
	virtual void HandleMovement(double dTime);

	// Movement
	EnemyState m_CurrentState = EnemyState::MOVINGRIGHT;
	DOUBLE2 m_MoveSpeed = DOUBLE2(40, 0);

	// Edge detection, same way as for our character 
	DOUBLE2 m_EdgeDetectionOffset = DOUBLE2(0, 2);
	DOUBLE2 m_WallDetectionOffset = DOUBLE2(0, 0);
	PhysicsActor* m_ActWallPtr = nullptr;
	PhysicsActor* m_ActEdgePtr = nullptr;

	// Health, score, respawning
	double m_Health = 100.0;
	int m_ScoreReward = 500;

	// Sound
	FmodSound* m_SndDeathPtr = nullptr;
	FmodSound* m_SndHitPtr = nullptr;

	// Death effect
	Sprite* m_SpriteDeathEffectPtr = nullptr;
	
	// Damage
	bool m_IsTouchingPlayer = false;
	double m_Damage = 10;
	const double m_DamageInterval = 1;
	double m_DamageAccuTime = m_DamageInterval;

	// Spawner reference, to notify it when it dies
	EnemySpawner* m_EnemySpawnerPtr = nullptr;

};
