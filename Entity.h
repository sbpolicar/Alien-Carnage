#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Sprite;

class Entity 
{
public:

	Entity();
	Entity(DOUBLE2 pos, DOUBLE2 actSize, BodyType actBodyType, Sprite* spritePtr);
	Entity(DOUBLE2 pos, DOUBLE2 actSize, BodyType actBodyType);
	virtual ~Entity( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Entity( const Entity& ) = delete;
	Entity& operator=( const Entity& ) = delete;

	virtual void FlagDestroy();
	
	virtual void Tick(double dTime);
	virtual void Paint();

protected:

	// Destroy the entity
	void DestroyEntity();
	bool m_ShouldDestroy = false;

	// Stuff every entity needs
	PhysicsActor* m_ActPtr = nullptr;
	Sprite* m_SpritePtr = nullptr;

};
