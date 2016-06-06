#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Character;

class Captive : public Entity, ContactListener
{
public:

	Captive(DOUBLE2 pos);
	virtual ~Captive( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Captive( const Captive& ) = delete;
	Captive& operator=( const Captive& ) = delete;

	void Tick(double dTime);

	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr);

private: 

	// Config
	double m_LiveTimeAfterRescue = 1.0;
	const int m_ScoreReward = 1000;

	bool m_Rescued = false;
	double m_AccuTime = 0.0;

	// Sound
	FmodSound* m_SndRescuedPtr = nullptr;

};
