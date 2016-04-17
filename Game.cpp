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
	gameSettingsRef.SetWindowWidth(1152);
	gameSettingsRef.SetWindowHeight(576);
	gameSettingsRef.EnableConsole(true);
	gameSettingsRef.EnableAntiAliasing(false);
}

void Game::GameStart()
{

	// Create the player, camera and level
	m_Level = new Level(String("./resources/levels/sewer.txt"));
	m_Player = new Character(m_Level);
	m_Camera = new Camera(288, 144);

	// Scale up sprites using Nearest Neighbor
	GAME_ENGINE->SetBitmapInterpolationModeNearestNeighbor();

}

void Game::GameEnd()
{

	// Clean up our camera, player and level
	delete m_Camera;
	m_Camera = nullptr;
	delete m_Player;
	m_Player = nullptr;
	delete m_Level;
	m_Level = nullptr;

	// Entities
	ENT_MANAGER->Destroy();

}

void Game::GameTick(double deltaTime)
{

	ENT_MANAGER->Tick(deltaTime);
	m_Player->Tick(deltaTime);

	// Debugging Physics mode
	if (GAME_ENGINE->IsKeyboardKeyPressed('P'))
	{
		m_DebugMode = !m_DebugMode;
		GAME_ENGINE->EnablePhysicsDebugRendering(m_DebugMode);
	}

}

void Game::GamePaint()
{
	
	// Rescale everything to be bigger
	MATRIX3X2 matScale = MATRIX3X2::CreateIdentityMatrix() * MATRIX3X2::CreateScalingMatrix(m_Level->GetScale());
	GAME_ENGINE->SetWorldMatrix(matScale);

	// Apply the view matrix
	GAME_ENGINE->SetViewMatrix(m_Camera->GetViewMatrix(m_Level, m_Player));

	// Paint level background
	m_Level->PaintBackground();

	// Paint entities
	ENT_MANAGER->Paint();
	// Paint the player
	m_Player->Paint();

	// Paint level foreground
	m_Level->PaintForeground();
	
	// Reset view matrix for HUD
	GAME_ENGINE->SetViewMatrix(matScale);

	// Lastly, apply the view matrix again for Debug Renderer
	GAME_ENGINE->SetViewMatrix(m_Camera->GetViewMatrix(m_Level, m_Player));

}