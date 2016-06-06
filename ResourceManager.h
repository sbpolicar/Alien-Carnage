#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include <map>

class ResourceManager 
{
public:

	static ResourceManager* GetInstance();
	static void Destroy();
	virtual ~ResourceManager( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	ResourceManager( const ResourceManager& ) = delete;
	ResourceManager& operator=( const ResourceManager& ) = delete;

	Bitmap* GetBitmap(String path);
	FmodSound* GetSound(String path);

private: 

	ResourceManager();
	static ResourceManager* m_ResourceManagerPtr;

	Bitmap* CreateBitmap(String path);
	FmodSound* CreateSound(String path);

	std::map<std::string, Bitmap*> m_BitmapMap;
	std::map<std::string , FmodSound*> m_SoundMap;

};
