#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Entity;
class Character;

// Entity types for collisions
enum class EntityType
{
	PLAYER, ENEMY, WORLD
};

class EntityManager 
{
public:

	static EntityManager* GetInstance();
	static void Destroy();
	virtual ~EntityManager( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	EntityManager( const EntityManager& ) = delete;
	EntityManager& operator=( const EntityManager& ) = delete;
	
	void SetPlayer(Character* playerRef);
	Character* GetPlayer();

	void AddEntity(Entity* ent);
	void RemoveEntity(Entity* ent);
	void Tick(double dTime);
	void Paint();

private: 

	EntityManager();
	static EntityManager* m_EntityManagerPtr;

	// Entities vector
	std::vector<Entity*> m_EntitiesArr;

	// Player reference
	Character* m_CharacterPtr = nullptr;

};
