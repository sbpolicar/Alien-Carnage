//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "HUD.h"
#include "Sprite.h"
#include "Character.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

HUD::HUD(Character* characterRef) : m_CharacterPtr(characterRef)
{

	// Load bitmaps
	m_BmpBackgroundPtr = new Bitmap(String("./resources/images/ui/hud/background.png")); // BG
	m_BmpHealthPtr = new Bitmap(String("./resources/images/ui/hud/health.png")); // Health
	m_BmpFuelPtr = new Bitmap(String("./resources/images/ui/hud/fuel.png")); // Fuel | ammo
	m_BmpWeaponsPtr = new Bitmap(String("./resources/images/ui/hud/weapons.png")); // Weapons
	m_BmpRadarPtr = new Bitmap(String("./resources/images/ui/hud/radar.png")); // Radar
	m_BmpNumbersFontPtr = new Bitmap(String("./resources/images/ui/hud/hudnumbers.png"));
	m_BmpNumbersFontPtr->SetTransparencyColor(COLOR(4, 0, 8));

	// make sure our scale is right
	m_DrawPos.y *= m_HUDScale;

}

HUD::~HUD()
{

	// Clean up pointers
	delete m_BmpBackgroundPtr;
	m_BmpBackgroundPtr = nullptr;
	delete m_BmpHealthPtr;
	m_BmpHealthPtr = nullptr;
	delete m_BmpFuelPtr;
	m_BmpFuelPtr = nullptr;
	delete m_BmpWeaponsPtr;
	m_BmpWeaponsPtr = nullptr;
	delete m_BmpRadarPtr;
	m_BmpRadarPtr = nullptr;
	delete m_BmpNumbersFontPtr;
	m_BmpNumbersFontPtr = nullptr;

}

void HUD::Tick(double dTime)
{
	
	// Time played
	m_GameAccuTime += dTime;

	// RADAR
	// Radar wave
	m_RadarAccuTime += 3*dTime;

	if (m_RadarAccuTime >= 3.0)
	{
		m_RadarAccuTime = 0.0;
	}

}

void HUD::DrawNumbers(const DOUBLE2& posRef, std::string& number, size_t size)
{

	// No more than allowed digits are drawn
	if (number.size() > size)
	{
		number = number.substr(number.size()-7);
	}
	else
	{
		// If there are less than allowed digits, add 0's in front of it equal to the missing amount of digits to reach minimum
		size_t currentSize = number.size();
		for (size_t i = currentSize; i < size; i++)
		{
			number = "0" + number;
		}
	}

	DOUBLE2 drawPos = posRef;

	// draw each number
	for (size_t i = 0; i < number.size(); ++i)
	{
		// Calculate the number on the bitmap
		RECT2 r;
		r.left = (int)(number.at(i) - '0') * m_NumberSize.x;
		r.top = 0;
		r.right = r.left + m_NumberSize.x;
		r.bottom = m_NumberSize.y;

		// Draw it
		GAME_ENGINE->DrawBitmap(m_BmpNumbersFontPtr, drawPos, r);
		
		// Next digit will be drawn to the right a bit with adjusted kerning
		drawPos.x += m_NumberSize.x - 1;
	}

}

void HUD::PaintScore()
{

	// Convert value to string
	int value = m_CharacterPtr->GetScore();

	// Paint, max 7 digits
	DrawNumbers(m_DrawPos + DOUBLE2(150, 36), std::to_string(value), 7);

}

void HUD::PaintCredits()
{

	// Convert value to string
	int value = m_CharacterPtr->GetCredits();

	// Paint, max 7 digits
	DrawNumbers(m_DrawPos + DOUBLE2(202, 21), std::to_string(value), 4);

}

void HUD::PaintCaptives()
{

	// Convert value to string
	int value = m_CharacterPtr->GetCaptives();

	// Paint, max 7 digits
	DrawNumbers(m_DrawPos + DOUBLE2(227, 36), std::to_string(value), 2);

}

void HUD::PaintHealth()
{

	// How much health for 1 pixel in the health bar (100/40)
	double interval = 2.5;

	// Part of the health image we draw
	RECT2 r;
	r.left = 0;
	r.right = 19;
	r.bottom = 40;
	r.top = r.bottom - m_CharacterPtr->GetHealth() / interval;

	// Adjust the drawing position so it always has the anchor at the bottom
	DOUBLE2 drawPos = m_DrawPos;
	drawPos.y += r.top;

	// Draw
	GAME_ENGINE->DrawBitmap(m_BmpHealthPtr, drawPos + DOUBLE2(296, 4), r);

}

void HUD::PaintFuel()
{

	// How much fuel per pixel? (100 / 172)
	double interval = 100.0 / 172.0;

	// Part of the fuel bar we draw
	RECT2 r;
	r.left = 0;
	r.top = 0;
	r.bottom = 9;
	r.right = 172 - (100 - m_CharacterPtr->GetFuel()) / interval;

	// Draw
	GAME_ENGINE->DrawBitmap(m_BmpFuelPtr, m_DrawPos + DOUBLE2(114, 5), r);

}

void HUD::PaintTime()
{

	// Calculate time left
	int timeLeft = m_GameTime - m_GameAccuTime;
	// Only above 0
	if (timeLeft < 0)
	{
		timeLeft = 0;
	}

	// Paint minutes
	int minutes = timeLeft / 60;
	DrawNumbers(m_DrawPos + DOUBLE2(111, 21), std::to_string(minutes), 2);

	// Paint seconds
	int seconds = timeLeft % 60;
	DrawNumbers(m_DrawPos + DOUBLE2(136, 21), std::to_string(seconds), 2);

}

void HUD::PaintLives()
{



}

void HUD::PaintRadar()
{

	// Background
	GAME_ENGINE->DrawBitmap(m_BmpRadarPtr, m_DrawPos + DOUBLE2(3, 3));

	// Wave


}

void HUD::Paint()
{

	// Matrix for rescalable HUD
	MATRIX3X2 matHUD = MATRIX3X2::CreateTranslationMatrix(-DOUBLE2(m_DrawPos.x, m_DrawPos.y));

	// Firstly, draw fake borders to emulate those in the original game
	GAME_ENGINE->SetWorldMatrix(MATRIX3X2::CreateScalingMatrix(m_HUDScale));
	// Set the matrix, now paint
	GAME_ENGINE->FillRect(0, 0, 16, 200); // LEFT
	GAME_ENGINE->FillRect(304, 0, 320, 200); // RIGHT
	GAME_ENGINE->FillRect(0, 192, 320, 200); // BOTTOM

	// Continue with the actual HUD
	matHUD = matHUD * MATRIX3X2::CreateScalingMatrix(m_HUDScale) * MATRIX3X2::CreateTranslationMatrix(DOUBLE2(m_DrawPos.x, m_DrawPos.y));
	// Set matrix
	GAME_ENGINE->SetWorldMatrix(matHUD);

	// The background
	GAME_ENGINE->DrawBitmap(m_BmpBackgroundPtr, DOUBLE2(m_DrawPos.x, m_DrawPos.y));

	// Text elements
	PaintScore();
	PaintCaptives();
	PaintCredits();
	PaintTime();

	// Bars
	PaintHealth();
	PaintFuel();

	// Left-hand-side element (weapons, radar, face) TODO: Swapping between these depending on situation, for now just radar
	PaintRadar();

	// Bottom text (messages)

}