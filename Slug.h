#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Enemy.h"

class Slug : public Enemy
{
public:

	Slug(DOUBLE2 pos);
	virtual ~Slug( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Slug( const Slug& ) = delete;
	Slug& operator=( const Slug& ) = delete;

private: 

	const DOUBLE2 m_MoveSpeed = DOUBLE2(20, 0);

};
