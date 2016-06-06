#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Brent Van Handenhove
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"

class Sprite;
class Character;

class Effect : public Entity
{
public:

	Effect(DOUBLE2 pos, Sprite* spritePtr, double lifetime);
	Effect(Sprite* spritePtr, double lifetime); // Assume tracking effect on the player
	virtual ~Effect( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Effect( const Effect& ) = delete;
	Effect& operator=( const Effect& ) = delete;

	void Tick(double dTime);
	void Paint();

private: 

	// Positioning
	bool m_TrackPlayer = false;
	DOUBLE2 m_Position = DOUBLE2(0, 0);

	// Lifetime
	double m_TimeLived = 0.0;
	double m_LifeTime = 0.0;

};
