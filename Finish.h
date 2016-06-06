#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Finish : public Entity, ContactListener
{
public:

	Finish(DOUBLE2 pos);
	virtual ~Finish( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Finish( const Finish& ) = delete;
	Finish& operator=( const Finish& ) = delete;

	void Tick(double dTime);
	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private: 

	bool m_Unlocked = false;
	bool m_Open = false;
	double m_OpenAccuTime = 0.0;
	const double m_OpeningTime = 1.0;

};
