//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "ResourceManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

ResourceManager* ResourceManager::m_ResourceManagerPtr = nullptr;

ResourceManager::ResourceManager()
{
	
}

ResourceManager::~ResourceManager()
{

	std::map<std::string, Bitmap*>::iterator ib;
	std::map<std::string, FmodSound*>::iterator is;

	std::cout << m_BitmapMap.size();

	// Delete the bitmaps
	for (ib = m_BitmapMap.begin(); ib != m_BitmapMap.end(); ++ib)
	{
		delete ib->second;
	}
	m_BitmapMap.clear();

	// Delete the sounds
	for (is = m_SoundMap.begin(); is != m_SoundMap.end(); ++is)
	{
		delete is->second;
	}
	m_SoundMap.clear();

}

ResourceManager* ResourceManager::GetInstance()
{

	if (m_ResourceManagerPtr != nullptr)
	{
		return m_ResourceManagerPtr;
	}
	else
	{
		m_ResourceManagerPtr = new ResourceManager;
		return m_ResourceManagerPtr;
	}

}

void ResourceManager::Destroy()
{

	delete m_ResourceManagerPtr;
	m_ResourceManagerPtr = nullptr;

}

Bitmap* ResourceManager::GetBitmap(String path)
{

	std::string pathstr = path.C_str();

	// Check if the bitmap is already loaded
	if (m_BitmapMap.find(pathstr) != m_BitmapMap.end())
	{
		return m_BitmapMap[pathstr];
	}
	else
	{
		return CreateBitmap(path);
	}

}

FmodSound* ResourceManager::GetSound(String path)
{

	std::string pathstr = path.C_str();

	// Check if the bitmap is already loaded
	if (m_SoundMap.find(pathstr) != m_SoundMap.end())
	{
		return m_SoundMap[pathstr];
	}
	else
	{
		return CreateSound(path);
	}

}

Bitmap* ResourceManager::CreateBitmap(String path)
{

	std::cout << "Creating bitmap for the first time (" << path.C_str() << ")" << std::endl;

	std::string pathstr = path.C_str();
	Bitmap* newBitmapPtr = new Bitmap(path);

	m_BitmapMap.insert({ pathstr, newBitmapPtr });
	return newBitmapPtr;

}

FmodSound* ResourceManager::CreateSound(String path)
{

	std::cout << "Creating sound for the first time (" << path.C_str() << ")" << std::endl;

	std::string pathstr = path.C_str();

	FmodSound* sndPtr = new FmodSound();
	sndPtr->CreateSound(path);

	m_SoundMap.insert({pathstr, sndPtr});
	return sndPtr;

}