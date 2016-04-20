#pragma once
//-----------------------------------------------------------------
// Student data
// Name:
// Group: 1DAE..
//-----------------------------------------------------------------

class Y 
{
public:
	Y( );
	virtual ~Y( );
	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Y( const Y& ) = delete;
	Y& operator=( const Y& ) = delete;

private: 

};
