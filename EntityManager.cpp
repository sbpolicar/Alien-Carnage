//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "EntityManager.h"
#include "Entity.h"
#include "Character.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

EntityManager* EntityManager::m_EntityManagerPtr = nullptr;

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{

	// Destruct all our entities
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		delete m_EntitiesArr[i];
	}

	m_EntitiesArr.clear();

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

void EntityManager::AddEntity(Entity* ent)
{

	// Bug: This is called twice, no idea why
	// Temp fix until I find the cause
	bool keepAdding = true;
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] == ent) { keepAdding = false; return; }
	}
	if (!keepAdding) { return; }
	// end of temp fix

	// Find a nullptr entry or add a new entry
	bool foundEmptySpace = false;

	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		// Found a nullptr, add it here
		if (m_EntitiesArr[i] == nullptr)
		{
			m_EntitiesArr[i] = ent; 
			std::cout << "entity added at existing index " << i << std::endl;
			return;
		}
	}

	// Couldn't find a nullptr entry, push back a new one
	if (!foundEmptySpace)
	{
		m_EntitiesArr.push_back(ent);
		std::cout << "entity added at new index " << m_EntitiesArr.size() << std::endl;
	}

}

void EntityManager::RemoveEntity(Entity* ent)
{
	
	// Find the entity, destruct it and remove it from the array
	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] == ent)
		{
			delete m_EntitiesArr[i];
			m_EntitiesArr[i] = nullptr;
			std::cout << "entity removed" << std::endl;
			return;
		}
	}

}

void EntityManager::SetPlayer(Character* playerRef)
{

	m_CharacterPtr = playerRef;

}

Character* EntityManager::GetPlayer()
{

	return m_CharacterPtr;

}

void EntityManager::Tick(double dTime)
{

	for (size_t i = 0; i < m_EntitiesArr.size(); ++i)
	{
		if (m_EntitiesArr[i] != nullptr)
		{
			m_EntitiesArr[i]->Tick(dTime);
		}
	}

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
