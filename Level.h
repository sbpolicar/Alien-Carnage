#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Character;
class Sprite;

#include "ContactListener.h"

class Level : public ContactListener
{
public:

	Level();
	Level(String path);
	virtual ~Level( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Level( const Level& ) = delete;
	Level& operator=( const Level& ) = delete;

	void Load(String path);

	int GetWidth();
	int GetHeight();
	double GetScale();
	void SetScale(double scale);

	DOUBLE2 GetSpawnPosition();
	bool IsOnGround(PhysicsActor* actOtherPtr);

	void BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr); // Will be used for water effects

	void Tick(double dTime);
	void PaintBackground();
	void PaintForeground();

private: 

	void ParseLevel(String path, std::ifstream& fStream);
	void ParseObject(const std::string& objectRef);
	std::string FindParseValue(const std::string& valName, const std::string& objectRef);
	DOUBLE2 StringToDOUBLE2(const std::string&);
	void CreateWorld(const std::string& worldRef);
	void CreateTrigger(const std::string& entRef);
	void CreateCaptive(const std::string& entRef);
	void CreateEnemy(const std::string& entRef);
	void CreateVendor(const std::string& entRef);
	void CreateCheckpoint(const std::string& entRef);
	void CreateSpiketrap(const std::string& entRef);
	void CleanUpWorld();

	// World
	PhysicsActor* m_ActWorldPtr = nullptr;
	PhysicsActor* m_ActWaterPtr = nullptr;
	Bitmap* m_BmpForegroundPtr = nullptr;
	Bitmap* m_BmpBackgroundPtr = nullptr;	

	// Water effect stuff
	Sprite* m_SpriteWaterEffectPtr = nullptr;
	const double m_WaterEffectLifetime = 0.4;

	// Music for this zone/level
	FmodSound* m_SndMusicPtr = nullptr;
	bool m_MusicMuted = false;

	// World rescale to fit a bigger window without having to manually rescale all sprites and whatnot
	double m_Scale = 4.0;

	// Level properties
	int m_Width = 0;
	int m_Height = 0;
	DOUBLE2 m_SpawnPosition = DOUBLE2(0, 0);
	
};
