//-----------------------------------------------------------------
// Game Engine Object
// C++ Header - Game.h - version v2_18a March 2016 
// Copyright Kevin Hoefman, 2006 - 2011 
// Copyright DAE Programming Team, 2012 - 2016 
// http://www.digitalartsandentertainment.be/ 
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "Resource.h"	
#include "AbstractGame.h"

class Character;
class Camera;
class Level;
class HUD;

//-----------------------------------------------------------------
// Game Class																
//-----------------------------------------------------------------
class Game : public AbstractGame
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	Game();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~Game();

	// C++11 make the class non-copyable
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	//---------------------------
	// General Methods
	//---------------------------

	virtual void GameInitialize(GameSettings &gameSettingsRef);
	virtual void GameStart();
	virtual void GameEnd();
	virtual void GameTick(double deltaTime);
	virtual void GamePaint();

	// -------------------------
	// Public Member functions
	// -------------------------
	
private:
	// -------------------------
	// Private Member functions
	// -------------------------

	// -------------------------
	// Private Datamembers
	// -------------------------

	// Core stuff
	Character* m_CharacterPtr = nullptr;
	Camera* m_CameraPtr = nullptr;
	Level* m_LevelPtr = nullptr;
	HUD* m_HUDPtr = nullptr;

	// Debugging
	bool m_DebugMode = false;

};
