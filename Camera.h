#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Character;
class Level;
class HUD;

class Camera 
{
public:

	Camera(int w, int h);
	virtual ~Camera( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Camera( const Camera& ) = delete;
	Camera& operator=( const Camera& ) = delete;

	MATRIX3X2 GetViewMatrix(Level* lvlPtr, Character* charPtr);

private: 

	// Clamping and tracking
	void TrackPlayer(DOUBLE2& posRef, Character* charPtr);
	void ClampToLevel(DOUBLE2& posRef, Level* levelPtr);

	// Vars
	const DOUBLE2 m_Offset = DOUBLE2(48.0, 28.0); // Our HUD and game aren't actually the same size, the actual game drawn is smaller than our screen (Original res = 320x200)
	int m_Width;
	int m_Height;

};
