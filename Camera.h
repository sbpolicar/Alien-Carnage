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
	void Tick(Character* charPtr, double dTime);

private: 

	// Clamping and tracking
	void TrackPlayer(DOUBLE2& posRef, Character* charPtr);
	void ClampToLevel(DOUBLE2& posRef, Level* levelPtr);

	// Basic vars
	const DOUBLE2 m_Offset = DOUBLE2(16.0, 56.0); // Our HUD and game aren't actually the same size, the actual game drawn is smaller than our screen (Original res = 320x200)
	int m_Width;
	int m_Height;

	// Box where character can move freely
	DOUBLE2 m_FreedomBoxSize = DOUBLE2(20, 20);

	// Interpolating
	DOUBLE2 m_StartPosition = DOUBLE2(0, 0);
	DOUBLE2 m_CurrentPosition = DOUBLE2(0, 0);
	DOUBLE2 m_TargetPosition = DOUBLE2(0, 0);
	DOUBLE2 m_LastPos = DOUBLE2(0, 0);

	const double m_InterpolateSpeed = 5.0;

};
