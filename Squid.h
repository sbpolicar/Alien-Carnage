#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Enemy.h"

class Squid : public Enemy
{
public:

	Squid(DOUBLE2 pos);
	virtual ~Squid();

	// Make objects non-copSquidable
	// https://msdn.microsoft.com/en-us/librarSquid/dn457344.aspx 
	Squid( const Squid& ) = delete;
	Squid& operator=( const Squid& ) = delete;

private: 

	const DOUBLE2 m_MoveSpeed = DOUBLE2(25, 0);

};
