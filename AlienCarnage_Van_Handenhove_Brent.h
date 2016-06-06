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
class AlienCarnage_Van_Handenhove_Brent : public AbstractGame
{
public:
	//---------------------------
	// Constructor(s)
	//---------------------------
	AlienCarnage_Van_Handenhove_Brent();

	//---------------------------
	// Destructor
	//---------------------------
	virtual ~AlienCarnage_Van_Handenhove_Brent();

	// C++11 make the class non-copyable
	AlienCarnage_Van_Handenhove_Brent(const AlienCarnage_Van_Handenhove_Brent&) = delete;
	AlienCarnage_Van_Handenhove_Brent& operator=(const AlienCarnage_Van_Handenhove_Brent&) = delete;

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

	void StartSession();
	void EndSession();
	void WriteSessionData(std::string data);

	// -------------------------
	// Private Datamembers
	// -------------------------

	// Core stuff
	Character* m_CharacterPtr = nullptr;
	Camera* m_CameraPtr = nullptr;
	Level* m_LevelPtr = nullptr;
	HUD* m_HUDPtr = nullptr;

};
