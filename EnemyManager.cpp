//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EnemyManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{

	// Clean up spawners
	for (EnemySpawner* spawner : m_EnemySpawnersArr)
	{
		delete spawner;
	}
	m_EnemySpawnersArr.clear();

}

void EnemyManager::AddEnemy(const std::string& type, DOUBLE2 spawnPos)
{

	EnemySpawner::EnemyType enemyType = EnemySpawner::EnemyType::SQUID; // Default a squid

	if (type == "squid")
	{
		enemyType = EnemySpawner::EnemyType::SQUID;
	}
	else if (type == "turtle")
	{
		enemyType = EnemySpawner::EnemyType::TURTLE;
	}
	else if (type == "spider")
	{
		enemyType = EnemySpawner::EnemyType::SPIDER;
	}
	else if (type == "slug")
	{
		enemyType = EnemySpawner::EnemyType::SLUG;
	}
	else if (type == "zombie")
	{
		enemyType = EnemySpawner::EnemyType::ZOMBIE;
	}
	else
	{
		std::cout << "Tried to use an unknown enemy type, defaulting to Squid at " << spawnPos << std::endl;
	}

	// Add a new spawner to the vector
	m_EnemySpawnersArr.push_back(new EnemySpawner(enemyType, spawnPos));

}

void EnemyManager::Tick(double dTime)
{

	// Tick for all our spawners
	// So they can determine how long remaining to spawn etc
	for (EnemySpawner* spawner : m_EnemySpawnersArr)
	{
		spawner->Tick(dTime);
	}

}
