#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Brent Van Handenhove
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class SpikeTrap : public Entity, ContactListener
{
public:

	SpikeTrap(DOUBLE2 pos);
	virtual ~SpikeTrap( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	SpikeTrap( const SpikeTrap& ) = delete;
	SpikeTrap& operator=( const SpikeTrap& ) = delete;

	void Tick(double dTime);

	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);
	void EndContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private: 

	// Damage
	bool m_TouchingPlayer = false;
	const double m_Damage = 15;

	// Deploys every few seconds to surprise the player
	bool m_Deployed = false;
	const double m_WaitTime = 2.0;
	const double m_DeployTime = 0.5;
	double m_WaitAccuTime = 0.0;

};
