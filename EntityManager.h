#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Entity;

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

	void AddEntity(Entity* ent);
	void RemoveEntity(Entity* ent);
	void Tick(double dTime);
	void Paint();

private: 

	EntityManager();
	static EntityManager* m_EntityManagerPtr;

	std::vector<Entity*> m_EntitiesArr;

};
