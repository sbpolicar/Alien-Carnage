#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class EnemySpawner
{

public:

	enum EnemyType
	{
		SQUID, TURTLE, SPIDER, ZOMBIE, SLUG
	};

	EnemySpawner(EnemyType type, DOUBLE2 pos);
	virtual ~EnemySpawner();

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	EnemySpawner(const EnemySpawner&) = delete;
	EnemySpawner& operator=(const EnemySpawner&) = delete;

	void EnemyDied();
	void Spawn();
	void Tick(double dTime);

private:

	// Type and location
	EnemyType m_EnemyType = SQUID;
	DOUBLE2 m_SpawnPosition = DOUBLE2(0, 0);

	// Spawn timer
	const double m_RespawnTime = 8.0;
	double m_AccuTime = m_RespawnTime;

	// Keep track of enemy's state, enemy will notify its spawner if it dies so we can start counting
	// for the next respawn
	bool m_CurrentEnemyAlive = false;

};