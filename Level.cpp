//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Level.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "Trigger.h"
#include "Captive.h"
#include "Vendor.h"
#include "Checkpoint.h"
#include "SpikeTrap.h"
#include "Finish.h"
#include "Effect.h"
#include "Character.h"
#include <fstream>

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

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
	delete m_ActWaterPtr;
	m_ActWaterPtr = nullptr;
	delete m_SpriteWaterEffectPtr;
	m_SpriteWaterEffectPtr = nullptr;
	delete m_SndMusicPtr;
	m_SndMusicPtr = nullptr;

}

void Level::Load(String path)
{

	// Check if there's a . that isn't the first one
	if (path.Rfind(String(".")) == path.Find(String(".")))
	{
		std::cout << "LEVEL Missing extension, adding it to path" << std::endl;
		path += String(".txt");
	}
	else
	{
		// Check if the file extension is .txt
		String extension = path.SubStr(path.Rfind(String(".")));
		if (extension != String(".txt"))
		{
			std::cout << "LEVEL Invalid format" << std::endl;
			return;
		}
	}

	// Find the file and parse it to generate our level
	std::ifstream fStream;
	fStream.open(path.C_str());

	if (fStream.fail())
	{
		// The path doesn't exist, report error and ignore
		std::cout << "LEVEL Path not found: " << path.C_str() << std::endl;
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
		std::cout << std::endl;
	}
	else if (objectRef.find("trigger") != std::string::npos)
	{
		std::cout << "Creating trigger" << std::endl;
		CreateTrigger(objectRef);
	}
	else if (objectRef.find("captive") != std::string::npos)
	{
		std::cout << "Creating captive" << std::endl;
		CreateCaptive(objectRef);
	}
	else if (objectRef.find("enemy") != std::string::npos)
	{
		std::cout << "Creating enemy" << std::endl;
		CreateEnemy(objectRef);
	}
	else if (objectRef.find("vendor") != std::string::npos)
	{
		std::cout << "Creating vendor" << std::endl;
		CreateVendor(objectRef);
	}
	else if (objectRef.find("checkpoint") != std::string::npos)
	{
		std::cout << "Creating checkpoint" << std::endl;
		CreateCheckpoint(objectRef);
	}
	else if (objectRef.find("spiketrap") != std::string::npos)
	{
		std::cout << "Creating spiketrap" << std::endl;
		CreateSpiketrap(objectRef);
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

	// Make sure our world is empty before we start to avoid memory leaks when loading through levels (if added)
	CleanUpWorld();

	// Start setting our background, foreground and collision
	std::string fg = FindParseValue("foreground", worldRef);
	std::string bg = FindParseValue("background", worldRef);
	std::string col = FindParseValue("collision", worldRef);
	std::string water = FindParseValue("water", worldRef);
	std::string music = FindParseValue("music", worldRef);
	std::string finish = FindParseValue("finish", worldRef);

	if (bg.c_str() != "")
	{
		std::cout << "background: " << bg << std::endl;
		m_BmpBackgroundPtr = new Bitmap(String(bg.c_str()));
	}
	else
	{
		std::cout << "LEVEL MISSING BACKGROUND" << std::endl;
	}

	if (fg.c_str() != "")
	{
		std::cout << "foreground: " << fg << std::endl;
		m_BmpForegroundPtr = new Bitmap(String(fg.c_str()));
		m_BmpForegroundPtr->SetTransparencyColor(COLOR(128, 128, 128));
	}
	else
	{
		std::cout << "LEVEL MISSING FOREGROUND" << std::endl;
	}

	if (col.c_str() != "")
	{
		std::cout << "collision: " << col << std::endl;
		m_ActWorldPtr = new PhysicsActor(DOUBLE2(0, 0), 0, BodyType::STATIC);
		m_ActWorldPtr->AddSVGFixture(String(col.c_str()), 0.0, 0.0);
	}
	else
	{
		std::cout << "LEVEL MISSING COLLISION" << std::endl;
	}

	if (water.c_str() != "")
	{
		std::cout << "water: " << water << std::endl;
		m_ActWaterPtr = new PhysicsActor(DOUBLE2(0, 0), 0, BodyType::STATIC);
		m_ActWaterPtr->AddSVGFixture(String(water.c_str()), 0.0, 0.0);
		m_ActWaterPtr->SetSensor(true);
		m_ActWaterPtr->AddContactListener(this);

		// Water effect
		m_SpriteWaterEffectPtr = new Sprite();
		m_SpriteWaterEffectPtr->AddAnimation(String("Splash"), String("./resources/images/effects/water_splash.png"), 0, 0, 1, 9, 23);
		m_SpriteWaterEffectPtr->SetAnimation(String("Splash"));
	}
	
	if (music.c_str() != "")
	{
		std::cout << "music: " << music << std::endl;
		m_SndMusicPtr = new FmodSound();
		m_SndMusicPtr->CreateSound(String(music.c_str()));
		m_SndMusicPtr->SetLoopCount(-1);
		m_SndMusicPtr->Play();
	}

	// Create the finish
	if (finish.c_str() != "")
	{
		std::cout << "finish: " << finish << std::endl;
		new Finish(StringToDOUBLE2(FindParseValue("finish", worldRef)));
	}

	// Set the gravity (for possible space levels)
	GAME_ENGINE->SetGravity(DOUBLE2(0, std::stod(FindParseValue("gravity", worldRef))));

	// Get the world boundaries
	m_Width = m_BmpBackgroundPtr->GetWidth();
	m_Height = m_BmpBackgroundPtr->GetHeight();

	// Get the spawn position for our player
	std::string spawnPos = FindParseValue("spawnpos", worldRef);
	m_SpawnPosition = StringToDOUBLE2(spawnPos);

}

void Level::CreateTrigger(const std::string& entRef)
{
		
	// Get the position
	std::string pos = FindParseValue("pos", entRef);
	// Get the doorpos
	std::string doorPos = FindParseValue("doorpos", entRef);

	// Create the trigger
	new Trigger(StringToDOUBLE2(pos), StringToDOUBLE2(doorPos));

}

void Level::CreateCaptive(const std::string& entRef)
{

	// Position
	std::string pos = FindParseValue("pos", entRef);

	// Create the captive
	new Captive(StringToDOUBLE2(pos));
	
}

void Level::CreateEnemy(const std::string& entRef)
{

	// Enemy type
	std::string enemyType = FindParseValue("type", entRef);
	// Position
	DOUBLE2 pos = StringToDOUBLE2(FindParseValue("pos", entRef));

	// Enemies are a bit different from other entities, as they respawn every few seconds
	// Let the entity manager handle this one
	ENT_MANAGER->AddEnemy(enemyType, pos);

}

void Level::CreateVendor(const std::string& entRef)
{

	// Position
	std::string pos = FindParseValue("pos", entRef);
	// Type
	std::string type = FindParseValue("type", entRef);

	// Create the vendor
	new Vendor(StringToDOUBLE2(pos), std::stoi(type));

}

void Level::CreateCheckpoint(const std::string & entRef)
{

	// Position
	std::string pos = FindParseValue("pos", entRef);

	// Create the checkpoint
	new Checkpoint(StringToDOUBLE2(pos));

}

void Level::CreateSpiketrap(const std::string & entRef)
{

	// Position
	std::string pos = FindParseValue("pos", entRef);

	// Create the spiketrap
	new SpikeTrap(StringToDOUBLE2(pos));

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

void Level::SetScale(double scale)
{

	m_Scale = scale;

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

void Level::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		// Offset based on player size and stuff
		new Effect(actOtherPtr->GetPosition() + DOUBLE2(0, 20), m_SpriteWaterEffectPtr, m_WaterEffectLifetime);
	}

}

void Level::Tick(double dTime)
{

	// Mute music
	if (GAME_ENGINE->IsKeyboardKeyPressed('M'))
	{
		m_MusicMuted = !m_MusicMuted;
		if (m_MusicMuted) { 
			m_SndMusicPtr->SetVolume(0); 
		}
		else {
			m_SndMusicPtr->SetVolume(1); 
		}
		
	}

}

void Level::PaintBackground()
{
	
	// Paint the background
	// Moves around a bit depending on player movement for added effect
	DOUBLE2 charcPos = ENT_MANAGER->GetPlayer()->GetPosition();
	charcPos.x *= -1; // Invert x

	// Not too harsh, just a smooth, subtle movement
	charcPos.x = charcPos.x / 20;
	charcPos.y = charcPos.y / 8;

	// Draw
	GAME_ENGINE->DrawBitmap(m_BmpBackgroundPtr, charcPos);

}

void Level::PaintForeground()
{

	// Paint the foreground
	GAME_ENGINE->DrawBitmap(m_BmpForegroundPtr);

}