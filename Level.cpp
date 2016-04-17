//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Level.h"
#include <fstream>

#define GAME_ENGINE (GameEngine::GetSingleton())

Level::Level()
{

}

Level::Level(String path)
{

	// Load the level
	Load(path);

}

Level::~Level()
{

	// Clean up world
	CleanUpWorld();

	// Clean up our entities and objects

}

void Level::CleanUpWorld()
{

	// Clean up our world
	delete m_BmpForegroundPtr;
	m_BmpForegroundPtr = nullptr;
	delete m_BmpBackgroundPtr;
	m_BmpBackgroundPtr = nullptr;
	delete m_ActWorldPtr;
	m_ActWorldPtr = nullptr;

}

void Level::Load(String path)
{

	// Check if the file extension is .txt
	String extension = path.SubStr(path.Rfind(String(".")));
	if (extension != String(".txt"))
	{
		std::cout << "LEVEL Invalid format" << std::endl;
		return;
	}

	// Find the file and parse it to generate our level
	std::ifstream fStream;
	fStream.open(path.C_str());

	if (fStream.fail())
	{
		// The path doesn't exist, report error and ignore
		std::cout << "LEVEL Error not found: " << path.C_str() << std::endl;
		return;
	}
	else
	{
		// Path exists, continue creating
		std::cout << "LEVEL Initializing: " << path.C_str() << std::endl;
		// Start parsing the level
		ParseLevel(path, fStream);
	}

}

void Level::ParseLevel(String path, std::ifstream& fStream)
{

	// Start a stringstream
	std::stringstream strStream;
	std::string line;

	while (!fStream.eof())
	{
		// Get a line in our file, pass it to the stringstream
		std::getline(fStream, line);
		strStream << line;

		if (line.find("/>") != std::string::npos)
		{
			// if there is a /> on this line, it's the end of an object so pass it on for parsing
			ParseObject(strStream.str());
			strStream.str("");
		}
	}

	// Close the filestream, it has no use for us anymore
	fStream.close();

}

void Level::ParseObject(const std::string& objectRef)
{

	// Check if this is the world, if so create it
	if (objectRef.find("world") != std::string::npos)
	{
		std::cout << "\nParsing world" << std::endl;
		CreateWorld(objectRef);
	}

}

std::string Level::FindParseValue(const std::string& valName, const std::string& objectRef)
{

	// Check if the name of the value is even here
	if (objectRef.find(valName) == std::string::npos) return std::string("");

	// Find the actual value
	std::string name = valName + std::string("=");
	std::size_t startIdx = objectRef.find(name) + name.length();
	std::size_t valIdx = objectRef.find("\"", startIdx) + 1;
	std::size_t valEnd = objectRef.find("\"", valIdx);
	std::string val = objectRef.substr(valIdx, valEnd - valIdx);

	return val;

}

DOUBLE2 Level::StringToDOUBLE2(const std::string& stringRef)
{

	// Fetch a DOUBLE2 from an std::string
	std::size_t firstIdx = stringRef.find(",");
	std::size_t secondIdx = stringRef.find(",", ++firstIdx);
	return DOUBLE2(std::stod(stringRef.substr(0, firstIdx)), std::stod(stringRef.substr(firstIdx, secondIdx - firstIdx)));

}

void Level::CreateWorld(const std::string& worldRef)
{

	// Make sure our world is empty before we start to avoid memory leaks when loading through levels
	CleanUpWorld();

	// Start setting our background, foreground and collision
	std::string fg = FindParseValue("foreground", worldRef);
	std::string bg = FindParseValue("background", worldRef);
	std::string col = FindParseValue("collision", worldRef);

	std::cout << "background: " << bg << std::endl;
	m_BmpBackgroundPtr = new Bitmap(String(bg.c_str()));

	std::cout << "foreground: " << fg << std::endl;
	m_BmpForegroundPtr = new Bitmap(String(fg.c_str()));
	m_BmpForegroundPtr->SetTransparencyColor(COLOR(128, 128, 128));

	std::cout << "collision: " << col << std::endl;
	m_ActWorldPtr = new PhysicsActor(DOUBLE2(0, 0), 0, BodyType::STATIC);
	m_ActWorldPtr->AddSVGFixture(String(col.c_str()), 0.0, 0.0);

	// Set the gravity (for possible space levels)
	GAME_ENGINE->SetGravity(DOUBLE2(0, std::stod(FindParseValue("gravity", worldRef))));

	// Get the world boundaries
	m_Width = m_BmpBackgroundPtr->GetWidth();
	m_Height = m_BmpBackgroundPtr->GetHeight();

	// Get the spawn position for our player
	std::string spawnPos = FindParseValue("spawnpos", worldRef);
	m_SpawnPosition = StringToDOUBLE2(spawnPos);

}

int Level::GetWidth()
{

	return m_Width;

}

int Level::GetHeight()
{

	return m_Height;

}

double Level::GetScale()
{

	return m_Scale;

}

DOUBLE2 Level::GetSpawnPosition()
{

	return m_SpawnPosition;

}

bool Level::IsOnGround(PhysicsActor* actOtherPtr)
{

	// Check if it collides with the world
	return m_ActWorldPtr->IsOverlapping(actOtherPtr);

}

void Level::Tick(double dTime)
{

}

void Level::PaintBackground()
{
	
	// Paint the background
	GAME_ENGINE->DrawBitmap(m_BmpBackgroundPtr);

}

void Level::PaintForeground()
{

	// Paint the foreground
	GAME_ENGINE->DrawBitmap(m_BmpForegroundPtr);

}