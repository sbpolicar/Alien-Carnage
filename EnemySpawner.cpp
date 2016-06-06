//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemySpawner.h"
#include "Enemy.h"
#include "Squid.h"
#include "Spider.h"
#include "Turtle.h"
#include "Slug.h"
#include "Zombie.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

EnemySpawner::EnemySpawner(EnemyType type, DOUBLE2 pos)
	: m_EnemyType(type), m_SpawnPosition(pos)
{

}

EnemySpawner::~EnemySpawner()
{

}

void EnemySpawner::EnemyDied()
{

	m_CurrentEnemyAlive = false;

}

void EnemySpawner::Spawn()
{

	// enums to-do----------------------------------------------------

	Enemy* currentEnemyRef = nullptr;

	switch (m_EnemyType)
	{
	case EnemySpawner::SQUID:
		currentEnemyRef = new Squid(m_SpawnPosition);
		break;
	case EnemySpawner::TURTLE:
		currentEnemyRef = new Turtle(m_SpawnPosition);
		break;
	case EnemySpawner::SPIDER:
		currentEnemyRef = new Spider(m_SpawnPosition);
		break;
	case EnemySpawner::ZOMBIE:
		currentEnemyRef = new Zombie(m_SpawnPosition);
		break;
	case EnemySpawner::SLUG:
		currentEnemyRef = new Slug(m_SpawnPosition);
		break;
	default:
		break;
	}

	// Now tell the spawned enemy this is its spawner
	// That way, it can safely notify us when it dies
	if (currentEnemyRef != nullptr)
	{
		currentEnemyRef->SetSpawner(this);
	}

	// If the enemy type doesn't exist, this will still set to true
	// This doesn't matter, as it will stop the spawner from trying to spawn an unexisting 
	// enemytype, thus reducing unnecessary checks
	m_CurrentEnemyAlive = true;
	
}

void EnemySpawner::Tick(double dTime)
{
	
	// We're only interested in checking for respawns if the last one died
	if (!m_CurrentEnemyAlive)
	{
		m_AccuTime += dTime;
		if (m_AccuTime >= m_RespawnTime)
		{
			m_AccuTime -= m_RespawnTime;
			Spawn();
		}

	}

};
