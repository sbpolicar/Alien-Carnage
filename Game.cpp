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
#include "Game.h"				
#include "Character.h"
#include "Camera.h"
#include "Level.h"
#include "EntityManager.h"
#include "HUD.h"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

//-----------------------------------------------------------------
// Game methods																				
//-----------------------------------------------------------------

Game::Game()
{
	// nothing to create
}

Game::~Game()
{
	// nothing to destroy
}

void Game::GameInitialize(GameSettings &gameSettingsRef)
{
	gameSettingsRef.SetWindowTitle(String("Halloween Harry - Van Handenhove, Brent - 1DAE16"));
	gameSettingsRef.SetWindowWidth(1280);
	gameSettingsRef.SetWindowHeight(800);
	gameSettingsRef.EnableConsole(true);
	gameSettingsRef.EnableAntiAliasing(false);
}

void Game::GameStart()
{

	// Create the player, camera, level and HUD
	m_LevelPtr = new Level(String("./resources/levels/sewer.txt"));
	m_PlayerPtr = new Character(m_LevelPtr);
	m_CameraPtr = new Camera(320, 200);
	m_HUDPtr = new HUD(m_PlayerPtr);

	// Scale up sprites using Nearest Neighbor
	GAME_ENGINE->SetBitmapInterpolationModeNearestNeighbor();

}

void Game::GameEnd()
{

	// Clean up our camera, player and level
	delete m_CameraPtr;
	m_CameraPtr = nullptr;
	delete m_PlayerPtr;
	m_PlayerPtr = nullptr;
	delete m_LevelPtr;
	m_LevelPtr = nullptr;
	delete m_HUDPtr;
	m_HUDPtr = nullptr;

	// Entities
	ENT_MANAGER->Destroy();

}

void Game::GameTick(double deltaTime)
{

	// Tick for our entities, player and HUD
	ENT_MANAGER->Tick(deltaTime);
	m_PlayerPtr->Tick(deltaTime);
	m_HUDPtr->Tick(deltaTime);

	// Debugging Physics mode
	if (GAME_ENGINE->IsKeyboardKeyPressed('P'))
	{
		m_DebugMode = !m_DebugMode;
		GAME_ENGINE->EnablePhysicsDebugRendering(m_DebugMode);
	}

	// debugging code
	if (GAME_ENGINE->IsKeyboardKeyPressed('T'))
	{
		m_PlayerPtr->GiveCredits(1);
		m_PlayerPtr->GiveFuel(-1);
		m_PlayerPtr->GiveLives(1);
		m_PlayerPtr->TakeDamage(1);
	}

}

void Game::GamePaint()
{
	
	// Rescale everything to be bigger
	MATRIX3X2 matScale = MATRIX3X2::CreateScalingMatrix(m_LevelPtr->GetScale());
	GAME_ENGINE->SetWorldMatrix(matScale);

	// Apply the view matrix
	GAME_ENGINE->SetViewMatrix(m_CameraPtr->GetViewMatrix(m_LevelPtr, m_PlayerPtr));

	// Paint level background
	m_LevelPtr->PaintBackground();
	// Paint entities
	ENT_MANAGER->Paint();
	// Paint the player
	m_PlayerPtr->Paint();
	// Paint level foreground
	m_LevelPtr->PaintForeground();

	// Reset view matrix for HUD
	GAME_ENGINE->SetViewMatrix(MATRIX3X2::CreateIdentityMatrix());

	// Paint the HUD
	m_HUDPtr->Paint();

	// Lastly, apply the view matrix again for Debug Renderer
	GAME_ENGINE->SetViewMatrix(m_CameraPtr->GetViewMatrix(m_LevelPtr, m_PlayerPtr));
	GAME_ENGINE->SetWorldMatrix(matScale);

}