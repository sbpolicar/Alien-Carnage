//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EntityManager.h"
#include "ResourceManager.h"
#include "EnemyManager.h"
#include "Entity.h"
#include "Enemy.h"
#include "Vendor.h"
#include "Character.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

EntityManager* EntityManager::m_EntityManagerPtr = nullptr;

EntityManager::EntityManager()
{

	m_EnemyManagerPtr = new EnemyManager();

}

EntityManager::~EntityManager()
{

	// Destruct all our entities
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		delete m_EntitiesArr[i];
	}

	m_EntitiesArr.clear();
	
	delete m_EnemyManagerPtr;
	m_EnemyManagerPtr = nullptr;

}

EntityManager* EntityManager::GetInstance()
{

	// If there isn't already an instance, create one
	if (m_EntityManagerPtr == nullptr)
	{
		m_EntityManagerPtr = new EntityManager();
	}

	return m_EntityManagerPtr;

}

void EntityManager::Destroy()
{

	// If there is an instance, delete it
	if (m_EntityManagerPtr != nullptr)
	{
		delete m_EntityManagerPtr;
		m_EntityManagerPtr = nullptr;
	}

}

void EntityManager::AddEntity(Entity* entPtr)
{
	
	// Avoid adding the same entity twice for any reason at all
	bool keepAdding = true;
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] == entPtr) { keepAdding = false; return; }
	}
	if (!keepAdding) { return; }

	// Find a nullptr entry or add a new entry
	bool foundEmptySpace = false;

	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		// Found a nullptr, add it here
		if (m_EntitiesArr[i] == nullptr)
		{
			m_EntitiesArr[i] = entPtr;
			std::cout << "entity added at existing index " << i << std::endl;
			return;
		}
	}

	// Couldn't find a nullptr entry, push back a new one
	if (!foundEmptySpace)
	{
		m_EntitiesArr.push_back(entPtr);
		std::cout << "entity added at new index " << m_EntitiesArr.size() << std::endl;
	}

}

void EntityManager::RemoveEntity(Entity* entPtr)
{
	
	// Find the entity, destruct it and remove it from the array
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] == entPtr)
		{
			delete m_EntitiesArr[i];
			m_EntitiesArr[i] = nullptr;
			return;
		}
	}

}

void EntityManager::SetPlayer(Character* playerPtr)
{

	m_CharacterPtr = playerPtr;

}

Character* EntityManager::GetPlayer()
{

	return m_CharacterPtr;

}

void EntityManager::SetLevel(Level* levelPtr)
{

	m_LevelPtr = levelPtr;

}

Level* EntityManager::GetLevel()
{

	return m_LevelPtr;

}

void EntityManager::AddCaptive()
{

	++m_CaptiveCount;

}

void EntityManager::RemoveCaptive()
{

	--m_CaptiveCount;

}

int EntityManager::GetCaptiveCount()
{

	return m_CaptiveCount;

}

void EntityManager::AddVendor(Vendor* vendorPtr)
{

	if (vendorPtr != nullptr)
	{
		m_VendorsArr.push_back(vendorPtr);
	}

}

Vendor* EntityManager::TouchingVendor(PhysicsActor* actOtherPtr)
{

	for (Vendor* vend : m_VendorsArr)
	{
		if (vend->IsOverlapping(actOtherPtr))
		{
			return vend;
		}
	}

	return nullptr;

}

void EntityManager::AddEnemy(const std::string& type, const DOUBLE2& spawnPos)
{

	// Pass it on to the enemy spawner manager, more organized
	m_EnemyManagerPtr->AddEnemy(type, spawnPos);

}

std::vector<Enemy*> EntityManager::GetNearbyEnemies(const double& range)
{

	// Gets enemies near the player

	DOUBLE2 charPos = m_CharacterPtr->GetPosition();
	std::vector<Enemy*> enemiesArr;

	// Loop through all entities, find the enemies, do a distance check and add them to the vector
	// Then return the vector
	for (Entity* ent : m_EntitiesArr)
	{
		// Check if the entity is an enemy
		Enemy* enemy = dynamic_cast<Enemy*>(ent);
		if (enemy != nullptr)
		{
			DOUBLE2 enemyPos = enemy->GetPosition();
			// Distance check
			double dist = sqrt(pow(enemyPos.y - charPos.y, 2) + pow(enemyPos.x - charPos.x, 2));
			if (dist <= range)
			{
				// Enemy is within range, add to vector
				enemiesArr.push_back(enemy);
			}
		}
	}

	return enemiesArr;

}

void EntityManager::Tick(double dTime)
{

	// Tick for all entities
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] != nullptr)
		{
			m_EntitiesArr[i]->Tick(dTime);
		}
	}

	// Tick for the enemy respawning system
	m_EnemyManagerPtr->Tick(dTime);

}

void EntityManager::Paint()
{
	 
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] != nullptr)
		{
			m_EntitiesArr[i]->Paint();
		}
	}

}
