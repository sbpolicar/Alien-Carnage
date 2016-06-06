#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "ContactListener.h"
#include "Entity.h"

class Sprite;

class Projectile : public Entity, ContactListener
{
public:

	Projectile(DOUBLE2 pos, Sprite* spritePtr, DOUBLE2 vel, DOUBLE2 hitSize, double lifeTime);
	Projectile(DOUBLE2 pos, Sprite* spritePtr, DOUBLE2 vel, DOUBLE2 hitSize, double lifeTime, double damage);
	virtual ~Projectile( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Projectile( const Projectile& ) = delete;
	Projectile& operator=( const Projectile& ) = delete;

	void Tick(double dTime);
	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private: 

	// Lifetime
	double m_LifeTime = 0.0;
	double m_TimeLived = 0.0;
	
	// Movement
	DOUBLE2 m_Velocity = DOUBLE2(0, 0); // non initialize = 0,0 default

	// Damaging
	double m_Damage = 0;

};
