#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#include "Entity.h"
#include "ContactListener.h"

class Character;

class Vendor : public Entity
{
public:

	Vendor(DOUBLE2 pos, int type);
	virtual ~Vendor();

	// Make objects non-copVendorable
	// https://msdn.microsoft.com/en-us/librarVendor/dn457344.aspx 
	Vendor( const Vendor& ) = delete;
	Vendor& operator=( const Vendor& ) = delete;

	bool IsOverlapping(PhysicsActor* actOtherPtr);
	bool Buy(Character* characterPtr);

private: 
	
	enum VendorType
	{
		FUEL, PULSEAMMO, GRENADES, MISSILES
	};

	VendorType m_Type = FUEL;
	const int m_Cost = 5;

};
