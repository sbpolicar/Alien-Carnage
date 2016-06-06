	//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "HUD.h"
#include "Sprite.h"
#include "Enemy.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include <fstream>

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())

HUD::HUD()
{

	// Create the font for game sessions
	m_FontSessionPtr = new Font(String("Arial"), 3);

	// Load bitmaps
	m_BmpBackgroundPtr = new Bitmap(String("./resources/images/ui/hud/background.png")); // BG
	m_BmpHealthPtr = new Bitmap(String("./resources/images/ui/hud/health.png")); // Health
	m_BmpFuelPtr = new Bitmap(String("./resources/images/ui/hud/fuel.png")); // Fuel (Flamethrower ammo and for jetpack)
	m_BmpPulseAmmoPtr = new Bitmap(String("./resources/images/ui/hud/pulseammo.png")); // Pulseammo (Pulsegun ammo)
	m_BmpWeaponsPtr = new Bitmap(String("./resources/images/ui/hud/weapons.png")); // Weapons
	m_BmpRadarPtr = new Bitmap(String("./resources/images/ui/hud/radar.png")); // Radar
	m_BmpLivesPtr = new Bitmap(String("./resources/images/ui/hud/lives.png")); // Lives
	m_BmpLivesPtr->SetTransparencyColor(COLOR(12, 12, 28));
	m_BmpNumbersFontPtr = new Bitmap(String("./resources/images/ui/hud/hudnumbers.png")); // Numbers for score, credits etc
	m_BmpNumbersFontPtr->SetTransparencyColor(COLOR(4, 0, 8));
	// Menus and screens
	m_BmpHelpMenuPtr = new Bitmap(String("./resources/images/ui/help.png")); // Help screen
	m_BmpGameOverPtr = new Bitmap(String("./resources/images/ui/game_over.png")); // Game over screen
	m_BmpVictoryPtr = new Bitmap(String("./resources/images/ui/victory.png")); // Game victory screen
	// Talking messages
	m_SpriteHQTalkerPtr = new Sprite();
	m_SpriteHQTalkerPtr->AddAnimation(String("Start"), String("./resources/images/ui/hud/transmission_start.png"), 0, 0, 1, 2, 8);
	m_SpriteHQTalkerPtr->AddAnimation(String("Talk"), String("./resources/images/ui/hud/transmission_loop.png"), 0, 0, 1, 9, 5);
	m_SpriteHQTalkerPtr->SetAnimation(String("Start"));
	m_BmpFontPtr = new Bitmap(String("./resources/images/ui/hud/message_font.png")); // Font for messages
	// Radar
	m_SpriteRadarPtr = new Sprite();
	m_SpriteRadarPtr->AddAnimation(String("Pulse"), String("./resources/images/ui/hud/radar.png"), 0, 0, 1, 5, 5);
	m_SpriteRadarPtr->SetAnimation(String("Pulse"));

	// Make sure our scale is right
	m_DrawPos.y *= m_HUDScale;

	// Get session
	GetSession(0);

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
	delete m_BmpPulseAmmoPtr;
	m_BmpPulseAmmoPtr = nullptr;
	delete m_BmpWeaponsPtr;
	m_BmpWeaponsPtr = nullptr;
	delete m_BmpRadarPtr;
	m_BmpRadarPtr = nullptr;
	delete m_BmpNumbersFontPtr;
	m_BmpNumbersFontPtr = nullptr;
	delete m_BmpLivesPtr;
	m_BmpLivesPtr = nullptr;

	delete m_SpriteHQTalkerPtr;
	m_SpriteHQTalkerPtr = nullptr;
	delete m_SpriteRadarPtr;
	m_SpriteRadarPtr = nullptr;

	delete m_BmpHelpMenuPtr;
	m_BmpHelpMenuPtr = nullptr;
	delete m_BmpGameOverPtr;
	m_BmpGameOverPtr = nullptr;
	delete m_BmpVictoryPtr;
	m_BmpVictoryPtr = nullptr;

	delete m_FontSessionPtr;
	m_FontSessionPtr = nullptr;
	delete m_BmpFontPtr;
	m_BmpFontPtr = nullptr;

}

void HUD::GetSession(int number)
{
	
	if (number < 0)
	{
		// Can't go below game session 0
		return;
	}

	// Open the sessions file
	std::ifstream sessionStream;
	sessionStream.open("./resources/GameSessions.txt");

	std::string line;
	std::stringstream requestedSession;
	bool correctSession = false;

	if (!sessionStream.fail())
	{
		int currentSession = 0;

		// As long as we don't reach the end of the file, keep looping
		while (!sessionStream.eof())
		{
			getline(sessionStream, line);
			// Check if this is the start of a session
			if (line.find("<Session>") != line.npos)
			{
				if (number == currentSession)
				{
					correctSession = true;
				}
				++currentSession;
			}

			if (correctSession)
			{
				// Make it look pretty
				if (line.find("<Start>") != line.npos)
				{
					requestedSession << "--Start of Session--" << std::endl;
				}
				else if (line.find("<End>") != line.npos)
				{
					requestedSession << std::endl << "--End of Sesssion--" << std::endl;
				}
				else if (line.find("Session>") != line.npos || line.find("</End>") != line.npos || line.find("</Start>") != line.npos)
				{
					// We dont want these in the display
				}
				else
				{
					// -3 for the \t and >
					requestedSession << line.substr(line.find("<") + 1, line.find(">") - 3);
					requestedSession << ": " << line.substr(line.find(">") + 1, line.find("</") - line.find(">") - 1) << std::endl;
				}
			}

			if (correctSession && line.find("</Session>") != line.npos)
			{
				// Reached end of this session, stop
				break;
			}
			
		}
	}

	sessionStream.close();

	m_CurrentSession = number;
	m_CurrentSessionString = requestedSession.str();

}

void HUD::TickGameSession()
{

	if (GAME_ENGINE->IsKeyboardKeyPressed('I'))
	{
		// Hide or show game session info
		m_ShowGameSession = !m_ShowGameSession;
	}

	// Previous and next game session
	if (GAME_ENGINE->IsKeyboardKeyPressed(VK_PRIOR))
	{
		GetSession(m_CurrentSession - 1);
	}
	else if (GAME_ENGINE->IsKeyboardKeyPressed(VK_NEXT))
	{
		GetSession(m_CurrentSession + 1);
	}

}

void HUD::CheckWeaponSwapped()
{

	m_CurrentWeapon = ENT_MANAGER->GetPlayer()->GetCurrentWeapon();

}

void HUD::DisplayHelp()
{

	if (m_DisplayHelp)
	{
		GAME_ENGINE->DrawBitmap(m_BmpHelpMenuPtr);
	}

}

void HUD::DisplayGameOver()
{

	if (ENT_MANAGER->GetPlayer()->GetLives() <= 0)
	{
		GAME_ENGINE->DrawBitmap(m_BmpGameOverPtr);
	}

}

void HUD::DisplayVictory()
{

	if (m_GameVictory == true)
	{
		GAME_ENGINE->DrawBitmap(m_BmpVictoryPtr);
	}

}

double HUD::GetTimeLeft()
{

	return m_GameTime - m_GameAccuTime;

}

void HUD::Message(Character::Messages messageType)
{

	m_CurrentMessage = messageType;

}

void HUD::Win()
{

	m_GameVictory = true;

}

void HUD::TickTransmission(double dTime)
{

	// Only tick if there is a message
	if (m_CurrentMessage == Character::Messages::NONE)
	{
		return;
	}

	m_SpriteHQTalkerPtr->Tick(dTime);
	m_MessageAccuTime += dTime;

	// Only display for a few seconds
	if (m_MessageAccuTime < m_MessageDisplayTime - m_MessageSignalTime)
	{
		if (m_MessageAccuTime >= m_MessageSignalTime && !m_TransmissionTalking)
		{
			m_SpriteHQTalkerPtr->SetAnimation(String("Talk"));
			m_TransmissionTalking = true;
		}
	}
	else if (m_MessageAccuTime >= m_MessageDisplayTime)
	{
		m_CurrentMessage = Character::Messages::NONE;
		m_MessageAccuTime = 0.0;
		m_SpriteHQTalkerPtr->SetAnimation(String("Start"));
		m_TransmissionTalking = false;
	}
	else if (m_MessageAccuTime >= m_MessageDisplayTime - m_MessageSignalTime && m_TransmissionTalking)
	{
		m_SpriteHQTalkerPtr->SetAnimation(String("Start"));
		m_TransmissionTalking = false;
	}

}

void HUD::Tick(double dTime)
{
	
	// Debugging Physics mode
	// Put before pausing so we can pause the game at any time and get a good look at the physics in play
	if (GAME_ENGINE->IsKeyboardKeyPressed('P') && !m_DisplayHelp)
	{
		m_DebugMode = !m_DebugMode;
		GAME_ENGINE->EnablePhysicsDebugRendering(m_DebugMode);
	}
	else if (m_DisplayHelp && m_DebugMode)
	{
		// Hide debug mode if we hit F1, not a clean fix but enough for our needs
		m_DebugMode = false;
	}

	// Dont do these things if the game is paused
	if (!GAME_ENGINE->IsPaused())
	{
		// Game sessions
		TickGameSession();

		// Time played
		m_GameAccuTime += dTime;

		// Weapon swapping
		CheckWeaponSwapped();

		// Transmissions
		TickTransmission(dTime);

	}

	// Listen for help key pressed
	if (GAME_ENGINE->IsKeyboardKeyPressed(VK_F1))
	{
		m_DisplayHelp = !m_DisplayHelp;
		GAME_ENGINE->SetPaused(m_DisplayHelp);
	}

	// Radar toggling
	if (GAME_ENGINE->IsKeyboardKeyPressed('R'))
	{
		m_RadarEnabled = !m_RadarEnabled;
	}
	// Radar ticking
	if (m_RadarEnabled)
	{
		m_SpriteRadarPtr->Tick(dTime);
	}

}

void HUD::DrawNumbers(const DOUBLE2& posRef, std::string& number, size_t size)
{

	// No more than allowed digits are drawn
	std::stringstream numbersStream;
	numbersStream << std::setfill('0') << std::setw(size) << number;

	number = numbersStream.str();

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
	int value = ENT_MANAGER->GetPlayer()->GetScore();

	// Paint, max 7 digits
	DrawNumbers(m_DrawPos + DOUBLE2(150, 36), std::to_string(value), 7);

}

void HUD::PaintCredits()
{

	// Convert value to string
	int value = ENT_MANAGER->GetPlayer()->GetCredits();

	// Paint, max 7 digits
	DrawNumbers(m_DrawPos + DOUBLE2(202, 21), std::to_string(value), 4);

}

void HUD::PaintCaptives()
{

	// Convert value to string
	int value = ENT_MANAGER->GetCaptiveCount();

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
	r.top = r.bottom - ENT_MANAGER->GetPlayer()->GetHealth() / interval;

	// Adjust the drawing position so it always has the anchor at the bottom
	DOUBLE2 drawPos = m_DrawPos;
	drawPos.y += r.top;

	// Draw
	GAME_ENGINE->DrawBitmap(m_BmpHealthPtr, drawPos + DOUBLE2(296, 4), r);

}

void HUD::PaintAmmo()
{

	// How much ammo per pixel? (100 / 172)
	double interval = 100.0 / 172.0;

	// Part of the ammo bar we draw
	RECT2 r;

	switch (m_CurrentWeapon)
	{
	case Character::WeaponType::FLAMETHROWER:
		// Part of the fuel bar we draw
		r.left = 0;
		r.top = 0;
		r.bottom = 9;
		r.right = 172 - (100 - ENT_MANAGER->GetPlayer()->GetFuel()) / interval;

		// Draw
		GAME_ENGINE->DrawBitmap(m_BmpFuelPtr, m_DrawPos + DOUBLE2(114, 5), r);
		break;
	case Character::WeaponType::PULSEGUN:
		// Part of the ammo bar we draw
		r.left = 0;
		r.top = 0;
		r.bottom = 9;
		r.right = ENT_MANAGER->GetPlayer()->GetPulseAmmo() * 3;

		// Draw
		GAME_ENGINE->DrawBitmap(m_BmpPulseAmmoPtr, m_DrawPos + DOUBLE2(114, 5), r);
		break;
	case Character::WeaponType::MISSILES:
		break;
	}

}

void HUD::PaintTime()
{

	// Calculate time left
	int timeLeft = int(m_GameTime - m_GameAccuTime);
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

	int lives = ENT_MANAGER->GetPlayer()->GetLives();
	double livesWidth = m_BmpLivesPtr->GetWidth() / 6; // 5 lives, then death
	double livesHeight = m_BmpLivesPtr->GetHeight();

	// What part of image
	RECT2 r = { (7 - lives) * livesWidth, 0, (6 - lives) * livesWidth, livesHeight };
	// Drawpos
	DOUBLE2 drawPos = m_DrawPos + DOUBLE2(312, 5);

	// Draw
	GAME_ENGINE->DrawBitmap(m_BmpLivesPtr, drawPos, r);

}

void HUD::PaintRadar()
{

	// We'll need to do a check to make the enemies show up properly depending on their relative position to the player on the small radar area
	// And clamp to make sure we never go outside of those bounds

	DOUBLE2 radarPos = m_DrawPos + DOUBLE2(40, 24);

	// Background
	m_SpriteRadarPtr->Paint(radarPos);

	// Loop through enemies and depending on distance draw them on our radar
	std::vector<Enemy*> enemiesArr = ENT_MANAGER->GetNearbyEnemies(m_RadarRange);

	for (Enemy* enemy : enemiesArr)
	{
		// Calculate drawing position
		DOUBLE2 drawPos = radarPos;
		DOUBLE2 posDiff = ENT_MANAGER->GetPlayer()->GetPosition() - enemy->GetPosition();
		posDiff.x = posDiff.x / m_RadarRange * 40;
		posDiff.y = posDiff.y / m_RadarRange * 73;
		drawPos -= posDiff;

		// Clamp the position
		if (posDiff.x > -41 && posDiff.x < 34
			&& posDiff.y > -20 && posDiff.y < 20)
		{
			// Draw
			GAME_ENGINE->SetColor(COLOR(255, 255, 255, 150));
			GAME_ENGINE->FillEllipse(drawPos, 2, 2);
		}
	}

	// Reset color
	GAME_ENGINE->SetColor(COLOR(255, 255, 255, 255));

}

void HUD::PaintWeapon()
{

	double weaponDisplayWidth = m_BmpWeaponsPtr->GetWidth() / 4.0;

	// Get the right weapon image
	RECT2 weaponRect = { 0, 0, weaponDisplayWidth, double(m_BmpWeaponsPtr->GetHeight()) };
	switch (m_CurrentWeapon)
	{
	case Character::PULSEGUN:
		weaponRect.left += weaponDisplayWidth;
		weaponRect.right += weaponDisplayWidth;
		break;
	case Character::GRENADES:
		weaponRect.left += weaponDisplayWidth * 2;
		weaponRect.right += weaponDisplayWidth * 2;
		break;
	case Character::MISSILES:
		weaponRect.left += weaponDisplayWidth * 3;
		weaponRect.right += weaponDisplayWidth * 3;
		break;
	}

	// Paint the weapon image
	GAME_ENGINE->DrawBitmap(m_BmpWeaponsPtr, m_DrawPos + DOUBLE2(3, 3), weaponRect);

}

void HUD::PaintMessage()
{

	// Is there a message, if not don't continue
	if (!m_CurrentMessage == Character::Messages::NONE)
	{

		// Get the correct message to display
		String message = String("");

		switch (m_CurrentMessage)
		{
		case Character::BUYFAIL:
			message = String("You need more credits!");
			break;
		case Character::SPAWN:
			message = String("Good luck!");
			break;
		case Character::VICTORY:
			message = String("That's all of them!");
			break;
		}
		
		// Draw
		m_SpriteHQTalkerPtr->Paint(m_DrawPos + DOUBLE2(40, 24));
		GAME_ENGINE->SetColor(COLOR(255, 255, 255));

		// Draw the message, letter for letter
		RECT2 r = { 0, 0, 0, double(m_BmpFontPtr->GetHeight()) };
		DOUBLE2 pos = m_DrawPos + DOUBLE2(0, 49);
		
		for (int i = 0; i < message.Length(); ++i)
		{
			char letter = message.At(i);
			int index = letter;
			bool continueDraw = false;
			int w = m_BmpFontPtr->GetWidth() / 53;

			// Upper case
			if (letter >= 'A' && letter <= 'Z')
			{
				index += -'A' + 26;
				continueDraw = true;
			}
			// Lower case
			else if (letter >= 'a' && letter <= 'z')
			{
				index -= 'a';
				continueDraw = true;
			}
			// Space
			else if (letter == 32)
			{
				pos.x += w;
			}
			// Exclamation mark
			else if (letter == 33)
			{
				index = 52;
				continueDraw = true;
			}

			if (continueDraw)
			{
				r.left = index * w;
				r.right = (index + 1) * w;

				// Draw
				GAME_ENGINE->DrawBitmap(m_BmpFontPtr, pos, r);
				// Move one to the right in position
				pos.x += w;
			}
		}

	}

}

void HUD::Paint()
{

	// Matrix for rescalable HUD
	MATRIX3X2 matHUD = MATRIX3X2::CreateTranslationMatrix(-DOUBLE2(m_DrawPos.x, m_DrawPos.y));

	// Firstly, draw fake borders to copy those in the original game
	GAME_ENGINE->SetWorldMatrix(MATRIX3X2::CreateScalingMatrix(m_HUDScale));
	// Set the matrix, now paint
	GAME_ENGINE->FillRect(0, 0, 16, 200); // LEFT
	GAME_ENGINE->FillRect(304, 0, 320, 200); // RIGHT
	GAME_ENGINE->FillRect(0, 192, 320, 200); // BOTTOM
	
	// Game sessions
	if (m_ShowGameSession)
	{
		GAME_ENGINE->SetFont(m_FontSessionPtr);
		GAME_ENGINE->SetColor(COLOR(50, 50, 50));
		GAME_ENGINE->FillRect(10, 10, 120, 100);
		GAME_ENGINE->SetColor(COLOR(255, 255, 255));
		GAME_ENGINE->DrawString(String(m_CurrentSessionString.c_str()), RECT2(10, 10, 120, 100));
	}

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
	PaintAmmo();

	// Lives are displayed above health
	PaintLives();

	// Left-hand-side element (weapons, radar, face) TODO: Swapping between these depending on situation, for now just radar
	if (m_RadarEnabled)
	{
		PaintRadar();
	}
	else
	{
		PaintWeapon();
	}
	// Transmission messages
	PaintMessage();

	// On top of everything, menus
	// Need to reset the world matrix again so we can draw at 0,0
	GAME_ENGINE->SetWorldMatrix(MATRIX3X2::CreateScalingMatrix(m_HUDScale));

	// Help screen and gameover screen
	DisplayHelp();
	DisplayGameOver();
	DisplayVictory();

}