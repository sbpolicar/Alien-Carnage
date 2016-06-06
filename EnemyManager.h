#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "EnemySpawner.h"

class EnemyManager 
{
public:

	EnemyManager( );
	virtual ~EnemyManager( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	EnemyManager( const EnemyManager& ) = delete;
	EnemyManager& operator=( const EnemyManager& ) = delete;

	void AddEnemy(const std::string& type, DOUBLE2 spawnPos);
	void Tick(double dTime);

private: 
	
	std::vector<EnemySpawner*> m_EnemySpawnersArr;

};
