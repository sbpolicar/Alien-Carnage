#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "ContactListener.h"

class Sprite;

class Enemy : ContactListener
{
public:

	Enemy(DOUBLE2 pos, DOUBLE2 actSize, Sprite* spriteRef);
	virtual ~Enemy();

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Enemy( const Enemy& ) = delete;
	Enemy& operator=( const Enemy& ) = delete;

	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);
	virtual void Tick(double dTime) = 0;
	virtual void Paint() = 0;

private: 
	
	// Physics
	PhysicsActor* m_ActPtr = nullptr;

};
