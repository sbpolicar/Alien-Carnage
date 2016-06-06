#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Checkpoint : public Entity, ContactListener
{
public:

	Checkpoint(DOUBLE2 pos);
	virtual ~Checkpoint( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Checkpoint( const Checkpoint& ) = delete;
	Checkpoint& operator=( const Checkpoint& ) = delete;
	
	void Disactivate();
	DOUBLE2 GetPosition();

	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private: 

	void Activate();
	bool m_Activated = false;

	// Sound for when activating
	FmodSound* m_SndActivatePtr = nullptr;

};
