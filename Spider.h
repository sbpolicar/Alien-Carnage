#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Enemy.h"

class Spider : public Enemy
{
public:

	Spider(DOUBLE2 pos);
	virtual ~Spider();

	// Make objects non-copSquidable
	// https://msdn.microsoft.com/en-us/librarSquid/dn457344.aspx 
	Spider( const Spider& ) = delete;
	Spider& operator=( const Spider& ) = delete;
	
	void Tick(double dTime);

private: 
	
	void HandleMovement(double dTime);

	// Bite damage
	const double m_BiteDamage = 15.0;

	// How many pixels 'lenience' regarding the position?
	// If we do == it would only attack if the player is EXACTLY below the spider, so we will be a bit lenient
	const double m_PlayerTrackingLenience = 4.0;
	// How big is the range of biting Y-wise (eg 40 pixels high)
	const int m_BiteYRange = 40;

	// When biting, there is a delay before the spider can move again
	// This is both to let it finish the animation and give the player a chance to run away
	bool m_IsBiting = false;
	bool m_BitingAnimationStopped = false;
	const double m_DelayAfterBiting = 0.8;
	const double m_DelayBitingAnimationGone = 0.3;
	double m_DelayAfterBitingAccuTime = 0.0;

};
