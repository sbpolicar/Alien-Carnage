#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Enemy.h"

class Zombie : public Enemy
{
public:

	Zombie(DOUBLE2 pos);
	virtual ~Zombie();

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Zombie( const Zombie& ) = delete;
	Zombie& operator=( const Zombie& ) = delete;

private: 

	double m_Health = 120.0;
	const DOUBLE2 m_MoveSpeed = DOUBLE2(18, 0);

};
