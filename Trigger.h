#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "ContactListener.h"
#include "Entity.h"

class Trigger : public Entity, ContactListener
{
public:

	Trigger(DOUBLE2 pos, DOUBLE2 doorPos);
	virtual ~Trigger( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Trigger( const Trigger& ) = delete;
	Trigger& operator=( const Trigger& ) = delete;

	void Activate();
	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private:
	
	// Physics
	const DOUBLE2 m_ActSize = DOUBLE2(20, 24);

	// Trigger door
	int m_TriggerID = 0;
	bool m_Activated = false;
	Entity* m_EntDoorPtr = nullptr;
	Sprite* m_SpriteDoorPtr = nullptr;

};
