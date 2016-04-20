#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Sprite;
class Level;

class Character 
{
public:

	Character(Level* levelRef);
	virtual ~Character( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Character( const Character& ) = delete;
	Character& operator=( const Character& ) = delete;

	DOUBLE2 GetPosition();
	void Act(String name);

	// Gameplay things
	void TakeDamage(double amount);
	void GiveCredits(int amount);
	void GiveFuel(double amount);
	void GiveLives(int amount);

	// Gameplay things for HUD
	double GetHealth();
	double GetFuel();
	int GetCredits();
	int GetCaptives();
	int GetScore();
	int GetLives();

	void Tick(double dTime);
	void Paint();

private: 
	
	// States enum
	enum CharacterState
	{
		IDLE, IDLECROUCH, RUNNING, FLYING, FALLING, NEAREDGE, DEAD
	};
	
	// Gameplay stuff
	double m_Health = 100;
	double m_Fuel = 100;
	int m_Captives = 0;
	int m_Credits = 0;
	int m_Score = 0;
	int m_Lives = 2;

	// Functions and methods
	void SetState(CharacterState state);
	void ListenForMovement(DOUBLE2& velocity);
	void ListenForAttacking(double dTime);
	void FireFlamethrower(double dTime);
	void AnimateState();

	// Effects
	void TickJetpackSmoke(double dTime);
	void PaintPilotFlame();
	
	// Config
	CharacterState m_State = IDLE; 
	const double m_MoveSpeed = 140.0;
	const double m_JetpackSpeed = 240.0;

	// Physicsactor and sprite
	PhysicsActor* m_ActPtr = nullptr;
	Sprite* m_SpritePtr = nullptr;
	Level* m_LevelPtr = nullptr;

	// Edge detection feet (SVG line workaround)
	PhysicsActor* m_ActLeftSidePtr = nullptr;
	PhysicsActor* m_ActRightSidePtr = nullptr;
	DOUBLE2 m_EdgeDetectorsOffset = DOUBLE2(11.0, 17.0);

	// Flamethrower pilot flame
	Sprite* m_SpritePilotFlamePtr = nullptr;
	DOUBLE2 m_PilotFlameOffsetStanding = DOUBLE2(17.0, -6.0);
	DOUBLE2 m_PilotFlameOffsetCrouching = DOUBLE2(17.0, -1.0);

	// Primary attack (Flamethrower)
	bool m_Firing = false;
	Sprite* m_SpriteFlamethrowerPtr = nullptr;
	const DOUBLE2 m_FlamethrowerPhysicsSize = DOUBLE2(18, 14);
	const DOUBLE2 m_FlamethrowerVelocity = DOUBLE2(300, 0);
	const DOUBLE2 m_FlamethrowerOffsetStanding = DOUBLE2(15.0, -4.0);
	const DOUBLE2 m_FlamethrowerOffsetCrouching = DOUBLE2(15.0, 0.0);
	const double m_FlameLifetime = 0.25;
	const double m_FlamethrowerDelay = 0.03;
	const double m_FlamethrowerFuelUsage = 0.05;
	double m_FlamethrowerAccuTime = m_FlamethrowerDelay;

	// Smoke effect (treated as projectiles for now)
	Sprite* m_SpriteJetpackSmokePtr = nullptr;
	const DOUBLE2 m_JetpackSmokeOffset = DOUBLE2(-8.0, 1.0);
	const double m_JetpackSmokeLifetime = 0.33;
	const double m_JetpackSmokeDelay = 0.11;
	double m_JetpackSmokeAccuTime = m_JetpackSmokeDelay;

};
