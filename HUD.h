#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Sprite;
#include "Character.h"

class HUD 
{
public:

	HUD();
	virtual ~HUD( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	HUD( const HUD& ) = delete;
	HUD& operator=( const HUD& ) = delete;

	double GetTimeLeft();
	void Message(Character::Messages messageType);
	void Win();

	void Tick(double dTime);
	void Paint();

private:
	
	// Methods
	void DrawNumbers(const DOUBLE2& posRef, std::string& number, size_t size);
	void PaintCaptives();
	void PaintCredits();
	void PaintScore();
	void PaintHealth();
	void PaintAmmo();
	void PaintTime();
	void PaintLives();
	void PaintRadar();
	void PaintWeapon();
	void TickTransmission(double dTime);
	void PaintMessage();
	void CheckWeaponSwapped();

	// Menus and stuff
	void DisplayHelp();
	void DisplayGameOver();
	void DisplayVictory();

	// Game sessions
	void TickGameSession();
	void PaintGameSession();
	void GetSession(int number);

	bool m_ShowGameSession = false;
	int m_CurrentSession = 0;
	std::string m_CurrentSessionString = "";
	Font* m_FontSessionPtr = nullptr;

	// Config
	DOUBLE2 m_DrawPos = DOUBLE2(0.0, 144.0);
	const double m_HUDScale = 4.0;
	const DOUBLE2 m_NumberSize = DOUBLE2(9, 9);

	// Timers and counters
	const double m_GameTime = 600.0;
	double m_GameAccuTime = 0.0;

	// Debugging
	bool m_DebugMode = false;

	// Bitmaps
	Bitmap* m_BmpBackgroundPtr = nullptr;
	Bitmap* m_BmpNumbersFontPtr = nullptr;
	Bitmap* m_BmpHealthPtr = nullptr;
	Bitmap* m_BmpFuelPtr = nullptr;
	Bitmap* m_BmpPulseAmmoPtr = nullptr;
	Bitmap* m_BmpWeaponsPtr = nullptr;
	Bitmap* m_BmpRadarPtr = nullptr;
	Bitmap* m_BmpLivesPtr = nullptr;
	Bitmap* m_BmpFontPtr = nullptr;
	// For menus and screens
	Bitmap* m_BmpHelpMenuPtr = nullptr;
	Bitmap* m_BmpGameOverPtr = nullptr;
	Bitmap* m_BmpVictoryPtr = nullptr;

	// Victory
	bool m_GameVictory = false;

	// Weapon HUD
	Character::WeaponType m_CurrentWeapon = Character::WeaponType::FLAMETHROWER;

	// Radar
	bool m_RadarEnabled = false;
	Sprite* m_SpriteRadarPtr = nullptr;
	const double m_RadarRange = 300;

	// Displaying messages on the HUD (talking to our 'HQ')
	Sprite* m_SpriteHQTalkerPtr = nullptr;
	double m_MessageAccuTime = 0.0;
	bool m_TransmissionTalking = false;
	const double m_MessageSignalTime = 0.5;
	const double m_MessageDisplayTime = 4.0;
	Character::Messages m_CurrentMessage;

	// Help menu
	bool m_DisplayHelp = false;

};
