#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Sprite;
class Character;

class HUD 
{
public:

	HUD(Character* characterRef);
	virtual ~HUD( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	HUD( const HUD& ) = delete;
	HUD& operator=( const HUD& ) = delete;

	void Tick(double dTime);
	void Paint();

private:
	
	// Methods
	void DrawNumbers(const DOUBLE2& posRef, std::string& number, size_t size);
	void PaintCaptives();
	void PaintCredits();
	void PaintScore();
	void PaintHealth();
	void PaintFuel();
	void PaintTime();
	void PaintLives();
	void PaintRadar();

	// Config
	DOUBLE2 m_DrawPos = DOUBLE2(0.0, 144.0);
	const double m_HUDScale = 4.0;
	Character* m_CharacterPtr = nullptr;
	const DOUBLE2 m_NumberSize = DOUBLE2(9, 9);

	// Timers and counters
	double m_GameTime = 600.0;
	double m_GameAccuTime = 0.0;
	double m_RadarAccuTime = 0.0;

	// Bitmaps
	Bitmap* m_BmpBackgroundPtr = nullptr;
	Bitmap* m_BmpNumbersFontPtr = nullptr;
	Bitmap* m_BmpHealthPtr = nullptr;
	Bitmap* m_BmpFuelPtr = nullptr;
	Bitmap* m_BmpWeaponsPtr = nullptr;
	Bitmap* m_BmpRadarPtr = nullptr;

};
