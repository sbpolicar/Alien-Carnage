#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Character;

class Level 
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

	DOUBLE2 GetSpawnPosition();
	bool IsOnGround(PhysicsActor* actOtherPtr);

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
	void CleanUpWorld();

	// World
	PhysicsActor* m_ActWorldPtr = nullptr;
	Bitmap* m_BmpForegroundPtr = nullptr;
	Bitmap* m_BmpBackgroundPtr = nullptr;	
	// World rescale to fit a bigger window without having to manually rescale all sprites and whatnot
	const double m_Scale = 4.0;

	// Level properties
	int m_Width = 0;
	int m_Height = 0;
	DOUBLE2 m_SpawnPosition = DOUBLE2(0, 0);
	
};
