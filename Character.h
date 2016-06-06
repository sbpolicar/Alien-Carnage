#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Sprite;
class Level;
class HUD;
class Checkpoint;

class Character 
{
public:

	Character(Level* levelPtr, HUD* HudPtr);
	virtual ~Character( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Character( const Character& ) = delete;
	Character& operator=( const Character& ) = delete;

	// For weapons, will be used in the HUD to display the correct weapon and ammo
	enum WeaponType
	{
		FLAMETHROWER, PULSEGUN, GRENADES, MISSILES
	};

	enum Messages
	{
		NONE, BUYFAIL, SPAWN, VICTORY
	};

	DOUBLE2 GetPosition();
	void Act(String name);

	// Gameplay things
	void TakeDamage(double amount);
	void GiveCredits(int amount);
	void GiveScore(int amount);
	void GiveFuel(double amount);
	void GivePulseAmmo(double amount);
	void GiveMissiles(int amount);
	void GiveLives(int amount);
	void SetCheckpoint(Checkpoint* checkpointPtr);
	void Win();

	// Gameplay things for HUD
	double GetHealth();
	double GetFuel();
	double GetPulseAmmo();
	int GetMissiles();
	int GetCredits();
	int GetScore();
	int GetLives();
	Character::WeaponType GetCurrentWeapon();

	void Tick(double dTime);
	void Paint();

private: 
	
	// States enum
	enum CharacterState
	{
		IDLE, IDLECROUCH, RUNNING, FLYING, FALLING, NEAREDGE, BUYING, DEAD
	};
	
	// Gameplay stuff
	double m_Health = 100;
	double m_Fuel = 100;
	double m_PulseAmmo = 10;
	int m_Grenades = 0;
	int m_Missiles = 0;
	int m_Credits = 0;
	int m_Score = 0;
	int m_Lives = 6;

	// Checkpoints
	Checkpoint* m_LastCheckpoint = nullptr;

	// Functions and methods
	void SetState(CharacterState state);
	void ListenForMovement(DOUBLE2& velocity);
	void ListenForAttacking(double dTime);
	void ListenForWeaponSwap();
	void FireFlamethrower(double dTime);
	void FirePulsegun(double dTime);
	void FireGrenade();
	void FireMissile();
	void EquipWeapon(WeaponType weapon);
	void AnimateState();
	void Respawn();
	void Die();
	void TickDeath(double dTime);

	// Effects
	void TickJetpackSmoke(double dTime);
	void PaintPilotFlame();
	
	// Config
	CharacterState m_State = IDLE; 
	const double m_MoveSpeed = 140.0;
	const double m_JetpackSpeed = 240.0;

	// Physicsactor and sprite
	PhysicsActor* m_ActPtr = nullptr;
	b2Fixture* m_CollisionFixturePtr; // Used to change our hitbox when we crouch / stand up
	Sprite* m_SpritePtr = nullptr;
	Level* m_LevelPtr = nullptr;
	HUD* m_HudPtr = nullptr;

	// Edge detection feet (SVG line workaround)
	PhysicsActor* m_ActLeftSidePtr = nullptr;
	PhysicsActor* m_ActRightSidePtr = nullptr;
	DOUBLE2 m_EdgeDetectorsOffset = DOUBLE2(11.0, 17.0);

	// Taking damage has a delay so you can't run in and out of enemies to die really fast (can only take damage every x seconds)
	double m_DamageTakenAccu = 0.0;
	const double m_DamageTakeDelay = 0.5;

	// Buying animation freezes us for a bit, leaving the player vulnerable to attacks
	double m_BuyAccuTime = 0.0;
	const double m_BuyingTime = 0.5;

	// Keep track of wether or not the player has saved all the captives
	bool m_AllCaptivesSaved = false;

	// Damage sounds, no need for a vector or anything for just 2 sounds
	FmodSound* m_SndPlayerHit1Ptr = nullptr;
	FmodSound* m_SndPlayerHit2Ptr = nullptr;

	// Dying has a delay where we see our character slowly die before we respawn
	// Game over has this too, except we don't respawn but display the game over screen instead
	const double m_DeathAnimationTime = 3.0;
	const double m_DeathWaitTime = 2.0;
	double m_DeathAccuTime = 0.0;
	bool m_Dead = false;

	//
	// WEAPONS
	//
	
	WeaponType m_CurrentWeapon = FLAMETHROWER;
	bool m_Firing = false;
	const DOUBLE2 m_ShootOffsetStanding = DOUBLE2(15.0, -4.0);
	const DOUBLE2 m_ShootOffsetCrouching = DOUBLE2(15.0, 0.0);
	double m_ShootAccuTime = m_FlamethrowerDelay;

	// Primary weapon (Flamethrower)
	FmodSound* m_SndFlamethrowerShootPtr = nullptr;
	Sprite* m_SpriteFlamethrowerPtr = nullptr;
	const DOUBLE2 m_FlamethrowerPhysicsSize = DOUBLE2(18, 14);
	const DOUBLE2 m_FlamethrowerVelocity = DOUBLE2(300, 0);
	const double m_FlamethrowerDamage = 5;
	const double m_FlameLifetime = 0.3;
	const double m_FlamethrowerDelay = 0.05;
	const double m_FlamethrowerFuelUsage = 0.05;
	// Flamethrower pilot flame
	Sprite* m_SpritePilotFlamePtr = nullptr;
	DOUBLE2 m_PilotFlameOffsetStanding = DOUBLE2(17.0, -6.0);
	DOUBLE2 m_PilotFlameOffsetCrouching = DOUBLE2(17.0, -1.0);

	// Secondary weapon (pulsegun)
	FmodSound* m_SndPulsegunShootPtr = nullptr;
	Sprite* m_SpritePulseShotPtr = nullptr;
	const DOUBLE2 m_PulseShotPhysicsSize = DOUBLE2(18, 14);
	const DOUBLE2 m_PulseShotVelocity = DOUBLE2(400, 0);
	const double m_PulseShotDamage = 17;
	const double m_PulseShotLifetime = 1.0;
	const double m_PulseShotDelay = 0.18;
	const double m_PulseShotAmmoUsage = 1.0;

	// Third weapon (grenades)
	// Fourth weapon (missiles)

	// Smoke effect (treated as projectiles for now)
	Sprite* m_SpriteJetpackSmokePtr = nullptr;
	const DOUBLE2 m_JetpackSmokeOffset = DOUBLE2(-8.0, 1.0);
	const double m_JetpackSmokeLifetime = 0.33;
	const double m_JetpackSmokeDelay = 0.11;
	const double m_JetpackFuelUsage = 0.01;
	double m_JetpackSmokeAccuTime = m_JetpackSmokeDelay;

};
