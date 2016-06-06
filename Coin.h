#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Coin : public Entity, ContactListener
{
public:

	Coin(DOUBLE2 pos);
	virtual ~Coin( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Coin( const Coin& ) = delete;
	Coin& operator=( const Coin& ) = delete;

	void Tick(double dTime);

	void PreSolve(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr, bool& enableContactRef);
	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private: 

	// Sounds
	FmodSound* m_SndSpawnPtr = nullptr;
	FmodSound* m_SndPickupPtr = nullptr;
	FmodSound* m_SndBouncePtr = nullptr;

	// Fake bouncing sound
	double m_BounceAccuTime = 0.0;
	const double m_BounceDelay = 1;

};
