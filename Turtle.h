#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Enemy.h"

class Turtle : public Enemy
{
public:

	Turtle(DOUBLE2 pos);
	virtual ~Turtle( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Turtle( const Turtle& ) = delete;
	Turtle& operator=( const Turtle& ) = delete;

private: 
	
	bool Hit(double xPos, double amount);

};
