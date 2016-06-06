#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Entity;
class Level;
class Character;
class Vendor;
class EnemyManager;
class Enemy;

// Entity types for collisions
enum EntityType
{
	PLAYER, ENEMY, WORLD, PICKUP
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
	
	void SetPlayer(Character* playerPtr);
	Character* GetPlayer();

	void SetLevel(Level* levelPtr);
	Level* GetLevel();

	int GetCaptiveCount();
	void AddCaptive();
	void RemoveCaptive();

	void AddVendor(Vendor* vendorPtr);
	//? Returns nullptr if not touching any vendors
	Vendor* TouchingVendor(PhysicsActor * actOtherPtr);

	void AddEnemy(const std::string& type, const DOUBLE2& spawnPos);
	std::vector<Enemy*> GetNearbyEnemies(const double& range);

	void AddEntity(Entity* entPtr);
	void RemoveEntity(Entity* entPtr);

	void Tick(double dTime);
	void Paint();

private: 

	EntityManager();
	static EntityManager* m_EntityManagerPtr;

	// Entities vector
	std::vector<Entity*> m_EntitiesArr;
	
	// Amount of captives in the level
	int m_CaptiveCount = 0;

	// References
	Character* m_CharacterPtr = nullptr;
	Level* m_LevelPtr = nullptr;

	// Enemy respawning
	EnemyManager* m_EnemyManagerPtr = nullptr;

	// Vendors
	std::vector<Vendor*> m_VendorsArr;

};
