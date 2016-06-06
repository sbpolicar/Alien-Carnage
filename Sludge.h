#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Sludge : public Entity, ContactListener
{
public:

	Sludge(DOUBLE2 pos);
	virtual ~Sludge( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Sludge( const Sludge& ) = delete;
	Sludge& operator=( const Sludge& ) = delete;

	void PreSolve(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr, bool& enableContactRef);
	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);
	void EndContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

	void Tick(double dTime);

private: 

	// Lifetime of sludge
	const double m_LifeTime = 6.0;
	double m_LifeAccuTime = 0.0;

	// Damaging the player
	bool m_TouchingPlayer = false;
	const double m_Damage = 8.5;
	const double m_DamageInterval = 0.3;
	double m_DamageAccuTime = m_DamageInterval;

};
