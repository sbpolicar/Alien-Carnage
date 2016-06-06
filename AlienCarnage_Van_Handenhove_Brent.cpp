//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - Game.cpp - version v2_18a March 2016  
// Copyright Kevin Hoefman, 2006 - 2011 
// Copyright DAE Programming Team, 2012 - 2016 
// http://www.digitalartsandentertainment.be/ 
//-----------------------------------------------------------------

// this include must be the first include line of every cpp file (due to using precompiled header)
#include "stdafx.h"		

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "AlienCarnage_Van_Handenhove_Brent.h"				
#include "ResourceManager.h"
#include "EntityManager.h"
#include "Character.h"
#include "Camera.h"
#include "Level.h"
#include "HUD.h"
#include "Windows.h"
#include <fstream>
#include <sstream>

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())
#define ENT_MANAGER (EntityManager::GetInstance())

//-----------------------------------------------------------------
// Game methods																				
//-----------------------------------------------------------------

AlienCarnage_Van_Handenhove_Brent::AlienCarnage_Van_Handenhove_Brent()
{
	// nothing to create
}

AlienCarnage_Van_Handenhove_Brent::~AlienCarnage_Van_Handenhove_Brent()
{
	// nothing to destroy
}

void AlienCarnage_Van_Handenhove_Brent::GameInitialize(GameSettings &gameSettingsRef)
{
	gameSettingsRef.SetWindowTitle(String("Halloween Harry - Van Handenhove, Brent - 1DAE16"));
	gameSettingsRef.SetWindowWidth(1280);
	gameSettingsRef.SetWindowHeight(800);
	gameSettingsRef.EnableConsole(true);
	gameSettingsRef.EnableAntiAliasing(false);
}

void AlienCarnage_Van_Handenhove_Brent::GameStart()
{

	// Create the player, camera, level and HUD
	m_LevelPtr = new Level(String("./resources/levels/sewer.txt"));
	m_CameraPtr = new Camera(320, 200);
	m_HUDPtr = new HUD();
	m_CharacterPtr = new Character(m_LevelPtr, m_HUDPtr);

	// Set references for the Entity Manager
	ENT_MANAGER->SetPlayer(m_CharacterPtr);
	ENT_MANAGER->SetLevel(m_LevelPtr);

	// Scale up sprites using Nearest Neighbor
	GAME_ENGINE->SetBitmapInterpolationModeNearestNeighbor();
	
	// Write start of session
	StartSession();

}

void AlienCarnage_Van_Handenhove_Brent::GameEnd()
{

	// End session
	EndSession();

	// Clean up our camera, player and level
	delete m_CameraPtr;
	m_CameraPtr = nullptr;
	delete m_CharacterPtr;
	m_CharacterPtr = nullptr;
	delete m_LevelPtr;
	m_LevelPtr = nullptr;
	delete m_HUDPtr;
	m_HUDPtr = nullptr;

	// Entities
	ENT_MANAGER->Destroy();

	// Resources
	RESOURCE_MANAGER->Destroy();

}

void AlienCarnage_Van_Handenhove_Brent::StartSession()
{

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	std::string sessionData = "<Session>\n	<Start>\n	";

	// Date
	std::stringstream date;
	date << std::setfill('0');
	date << std::to_string(sysTime.wYear) << ":" << std::setw(2) << std::to_string(sysTime.wMonth) << ":" << std::setw(2) << std::to_string(sysTime.wDay);

	sessionData += "	<Date>" + date.str() + "</Date>\n	";

	// Time
	std::stringstream time;
	time << std::setfill('0');
	time << std::setw(2) << std::to_string(sysTime.wHour) << ":" << std::setw(2) << std::to_string(sysTime.wMinute) << ":" << std::setw(2) << std::to_string(sysTime.wSecond);

	sessionData += "	<Time>" + time.str() + "</Time>\n	";

	// Player info
	sessionData += "	<Health>" + std::to_string(m_CharacterPtr->GetHealth()) + "</Health>\n	";
	sessionData += "	<Lives>" + std::to_string(m_CharacterPtr->GetLives()) + "</Lives>\n	";
	sessionData += "	<Fuel>" + std::to_string(m_CharacterPtr->GetFuel()) + "</Fuel>\n	";
	sessionData += "	<PulseAmmo>" + std::to_string(m_CharacterPtr->GetPulseAmmo()) + "</PulseAmmo>\n	";
	sessionData += "	<Credits>" + std::to_string(m_CharacterPtr->GetCredits()) + "</Credits>\n	";
	sessionData += "	<Score>" + std::to_string(m_CharacterPtr->GetScore()) + "</Score>\n	";
	sessionData += "	<Captives>" + std::to_string(ENT_MANAGER->GetCaptiveCount()) + "</Captives>\n";

	// End
	sessionData += "	</Start>\n";

	WriteSessionData(sessionData);

}

void AlienCarnage_Van_Handenhove_Brent::EndSession()
{

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	std::string sessionData = "	<End>\n	";

	// Date
	std::stringstream date;
	date << std::setfill('0');
	date << std::to_string(sysTime.wYear) << ":" << std::setw(2) << std::to_string(sysTime.wMonth) << ":" << std::setw(2) << std::to_string(sysTime.wDay);

	sessionData += "	<Date>" + date.str() + "</Date>\n	";

	// Time
	std::stringstream time;
	time << std::setfill('0');
	time << std::setw(2) << std::to_string(sysTime.wHour) << ":" << std::setw(2) << std::to_string(sysTime.wMinute) << ":" << std::setw(2) << std::to_string(sysTime.wSecond);

	sessionData += "	<Time>" + time.str() + "</Time>\n	";

	// Player info
	sessionData += "	<Health>" + std::to_string(m_CharacterPtr->GetHealth()) + "</Health>\n	";
	sessionData += "	<Lives>" + std::to_string(m_CharacterPtr->GetLives()) + "</Lives>\n	";
	sessionData += "	<Fuel>" + std::to_string(m_CharacterPtr->GetFuel()) + "</Fuel>\n	";
	sessionData += "	<PulseAmmo>" + std::to_string(m_CharacterPtr->GetPulseAmmo()) + "</PulseAmmo>\n	";
	sessionData += "	<Credits>" + std::to_string(m_CharacterPtr->GetCredits()) + "</Credits>\n	";
	sessionData += "	<Score>" + std::to_string(m_CharacterPtr->GetScore()) + "</Score>\n	";
	sessionData += "	<Captives>" + std::to_string(ENT_MANAGER->GetCaptiveCount()) + "</Captives>\n	";

	// End
	sessionData += "</End>\n</Session>\n";

	WriteSessionData(sessionData);

}

void AlienCarnage_Van_Handenhove_Brent::WriteSessionData(std::string data)
{

	std::fstream fStream;
	fStream.open("./resources/GameSessions.txt", std::fstream::app);

	if (!fStream.fail())
	{
		fStream << data;
	}

	fStream.close();
	
}

void AlienCarnage_Van_Handenhove_Brent::GameTick(double deltaTime)
{

	// Pausing
	// Part of this was inserted in the GameEngine code, but this is still being called to handle Menus and whatnot in the future
	if (!GAME_ENGINE->IsPaused())
	{
		// Tick for our entities, player, HUD, level and camera
		ENT_MANAGER->Tick(deltaTime);
		m_CharacterPtr->Tick(deltaTime);
		m_LevelPtr->Tick(deltaTime);
		m_CameraPtr->Tick(m_CharacterPtr, deltaTime);

		// debugging code
		if (GAME_ENGINE->IsKeyboardKeyPressed('T'))
		{
			m_CharacterPtr->GiveCredits(1);
			m_CharacterPtr->TakeDamage(25);
		}
	}

	// Exiting
	if (GAME_ENGINE->IsKeyboardKeyPressed(VK_F10))
	{	
		GAME_ENGINE->QuitGame();	
	}

	// HUD continues to tick, for menus (so we can display a menu while the game is paused)
	m_HUDPtr->Tick(deltaTime);

}

void AlienCarnage_Van_Handenhove_Brent::GamePaint()
{
	
	// Rescale everything to be bigger
	MATRIX3X2 matScale = MATRIX3X2::CreateScalingMatrix(m_LevelPtr->GetScale());
	GAME_ENGINE->SetWorldMatrix(matScale);

	// Apply the view matrix
	GAME_ENGINE->SetViewMatrix(m_CameraPtr->GetViewMatrix(m_LevelPtr, m_CharacterPtr));

	// Paint level background
	m_LevelPtr->PaintBackground();
	// Paint entities
	ENT_MANAGER->Paint();
	// Paint the player
	m_CharacterPtr->Paint();
	// Paint level foreground
	m_LevelPtr->PaintForeground();

	// Reset view matrix for HUD
	GAME_ENGINE->SetViewMatrix(MATRIX3X2::CreateIdentityMatrix());

	// Paint the HUD
	m_HUDPtr->Paint();

	// Lastly, apply the view matrix again for Debug Renderer
	GAME_ENGINE->SetViewMatrix(m_CameraPtr->GetViewMatrix(m_LevelPtr, m_CharacterPtr));
	GAME_ENGINE->SetWorldMatrix(matScale);

}