//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Character.h"
#include "Sprite.h"
#include "Effect.h"
#include "Vendor.h"
#include "Checkpoint.h"
#include "Level.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "Projectile.h"
#include "HUD.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())
#define ENT_MANAGER (EntityManager::GetInstance())

Character::Character(Level* levelPtr, HUD* HudPtr) : m_LevelPtr(levelPtr), m_HudPtr(HudPtr)
{

	// Console
	std::cout << "\nCreating player" << std::endl;

	// Create the Physicsactor
	m_ActPtr = new PhysicsActor(m_LevelPtr->GetSpawnPosition(), 0, BodyType::DYNAMIC);
	m_CollisionFixturePtr = m_ActPtr->AddBoxFixture(22, 34, 0.0, 0.0);
	m_ActPtr->SetFixedRotation(true);
	m_ActPtr->SetUserData(int(EntityType::PLAYER)); // Make sure everything else knows this is the player
	m_ActPtr->SetUserPointer(this);

	// Create the Sprite and add our animations
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/characters/harry/anim_stand.png"));
	m_SpritePtr->AddAnimation(String("IdleCrouch"), String("./resources/images/characters/harry/anim_crouch.png"));
	m_SpritePtr->AddAnimation(String("Fall"), String("./resources/images/characters/harry/anim_falling.png"));
	m_SpritePtr->AddAnimation(String("Fly"), String("./resources/images/characters/harry/anim_fly.png"));
	m_SpritePtr->AddAnimation(String("Edge"), String("./resources/images/characters/harry/anim_nearedge.png"), 0, 0, 1, 2, 3);
	m_SpritePtr->AddAnimation(String("Run"), String("./resources/images/characters/harry/anim_run.png"), 0, 0, 1, 6, 10);
	m_SpritePtr->AddAnimation(String("Buy"), String("./resources/images/characters/harry/anim_buy.png"), 0, 0, 1, 2, 4);
	m_SpritePtr->AddAnimation(String("Die"), String("./resources/images/characters/harry/anim_die.png"), 0, 0, 1, 3, 1);
	m_SpritePtr->AddAnimation(String("Dead"), String("./resources/images/characters/harry/anim_dead.png"));
	// Set the starting animation
	m_SpritePtr->SetAnimation(String("Fall"));

	// Create our edge detection physicsactors
	m_ActLeftSidePtr = new PhysicsActor(m_ActPtr->GetPosition(), 0, BodyType::KINEMATIC);
	m_ActLeftSidePtr->AddBoxFixture(2, 2);
	m_ActLeftSidePtr->SetSensor(true);
	m_ActRightSidePtr = new PhysicsActor(m_ActPtr->GetPosition(), 0, BodyType::KINEMATIC);
	m_ActRightSidePtr->AddBoxFixture(2, 2);
	m_ActRightSidePtr->SetSensor(true);

	// Create the flamethrower fire sprite
	m_SpriteFlamethrowerPtr = new Sprite();
	m_SpriteFlamethrowerPtr->AddAnimation(String("Idle"), String("./resources/images/effects/flamethrower_flame.png"), 0, 0, 1, 11, 37);
	// Set the starting animation
	m_SpriteFlamethrowerPtr->SetAnimation(String("Idle"));

	// Create the pilot flame sprite
	m_SpritePilotFlamePtr = new Sprite();
	m_SpritePilotFlamePtr->SetParent(m_SpritePtr);
	m_SpritePilotFlamePtr->AddAnimation(String("Idle"), String("./resources/images/effects/flamethrower_pilotflame.png"), 0, 0, 1, 3, 9);
	// Set the starting animation
	m_SpritePilotFlamePtr->SetAnimation(String("Idle"));

	// Create the pulse shot sprite
	m_SpritePulseShotPtr = new Sprite();
	m_SpritePulseShotPtr->AddAnimation(String("Idle"), String("./resources/images/effects/pulseshot.png"));
	m_SpritePulseShotPtr->SetAnimation(String("Idle"));

	// Create smoke sprite
	m_SpriteJetpackSmokePtr = new Sprite();
	m_SpriteJetpackSmokePtr->AddAnimation(String("Idle"), String("./resources/images/effects/jetpack_smoke.png"), 0, 0, 1, 5, 15);
	// Set the starting animation
	m_SpriteJetpackSmokePtr->SetAnimation(String("Idle"));

	// Sounds
	// Damage taken
	m_SndPlayerHit1Ptr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/player_hit.wav"));
	m_SndPlayerHit2Ptr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/player_hit02.wav"));
	// Shooting sounds
	m_SndFlamethrowerShootPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/boom.wav"));
	m_SndPulsegunShootPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/photon.wav"));

	// Welcome the player
	m_HudPtr->Message(Messages::SPAWN);

}

Character::~Character()
{

	// Actor
	delete m_ActPtr;
	m_ActPtr = nullptr;

	// Main sprite
	delete m_SpritePtr;
	m_SpritePtr = nullptr;

	// Pilot flame
	delete m_SpritePilotFlamePtr;
	m_SpritePilotFlamePtr = nullptr;

	// Flamethrower flame projectile sprite
	delete m_SpriteFlamethrowerPtr;
	m_SpriteFlamethrowerPtr = nullptr;

	// Smoke sprite
	delete m_SpriteJetpackSmokePtr;
	m_SpriteJetpackSmokePtr = nullptr;

	// Edge detection 'feet'
	delete m_ActLeftSidePtr;
	m_ActLeftSidePtr = nullptr;
	delete m_ActRightSidePtr;
	m_ActRightSidePtr = nullptr;

}

void Character::GiveCredits(int amount)
{

	m_Credits += amount;

	// Can't get less than 0
	if (m_Credits < 0)
	{
		m_Credits = 0;
	}

}

void Character::GiveFuel(double amount)
{

	m_Fuel += amount;

	// Clamp between 0 and 100
	if (m_Fuel > 100)
	{
		m_Fuel = 100;
	}
	else if (m_Fuel < 0)
	{
		m_Fuel = 0;
	}

}

void Character::GivePulseAmmo(double amount)
{

	m_PulseAmmo += amount;

	// Clamp between 0 and 77
	if (m_PulseAmmo > 77)
	{
		m_PulseAmmo = 77;
	}
	else if (m_PulseAmmo < 0)
	{
		m_PulseAmmo = 0;
	}

}

void Character::GiveMissiles(int amount)
{

	m_Missiles += amount;

	if (m_Missiles < 0)
	{
		m_Missiles = 0;
	}

}

void Character::GiveLives(int amount)
{

	// Lives are between 0 and 5
	m_Lives = m_Lives + amount;
	if (m_Lives > 5)
	{
		m_Lives = 5;
	}

}

void Character::SetCheckpoint(Checkpoint* checkpointPtr)
{

	if (checkpointPtr != nullptr)
	{
		if (m_LastCheckpoint != nullptr) { m_LastCheckpoint->Disactivate(); } // Disactivate last one
		m_LastCheckpoint = checkpointPtr;
	}

}

void Character::Win()
{

	m_HudPtr->Win();

}

void Character::TakeDamage(double amount)
{

	if (!m_Dead)
	{
		// Avoid taking damage very rapidly
		if (m_DamageTakenAccu >= m_DamageTakeDelay)
		{
			m_DamageTakenAccu = 0.0;

			m_Health -= amount;
			// Can't get less than 0 health
			if (m_Health <= 0)
			{
				m_Health = 0;
				Die();
			}

			// Play a damage sound
			if (rand() % 2 == 0)
			{
				m_SndPlayerHit1Ptr->Play();
			}
			else
			{
				m_SndPlayerHit2Ptr->Play();
			}
		}
	}

}

void Character::Respawn()
{

	// Reset death check
	m_Dead = false;
	// Fix damping
	m_ActPtr->GetBody()->SetLinearDamping(0);

	// Return us to our spawn position or last checkpoint
	if (m_LastCheckpoint != nullptr)
	{
		m_ActPtr->SetPosition(m_LastCheckpoint->GetPosition());
	}
	else
	{
		m_ActPtr->SetPosition(m_LevelPtr->GetSpawnPosition());
	}

	// Reset health and ammo
	// Credits, score and special weapon ammo are kept as they were
	m_Health = 100;
	m_Fuel = 100;

	// Reset state
	m_State = CharacterState::IDLE;
	// Just for fancyness, make us face right again
	m_SpritePtr->Flipped(false);

}

void Character::Die()
{

	m_State = CharacterState::DEAD;
	m_ActPtr->SetLinearVelocity(DOUBLE2(0, 0)); // Prevent sliding
	m_ActPtr->GetBody()->SetLinearDamping(50); // Slow our descent to fix the instant fall (because using SetVelocity)

}

void Character::TickDeath(double dTime)
{

	// Death by timer, also known as 'you ran out of time'
	if (m_HudPtr->GetTimeLeft() <= 0)
	{
		// No time left, player loses and will die
		m_Lives = 1;
		Die();
	}

	if (m_State == CharacterState::DEAD)
	{
		if (m_DeathAccuTime == 0)
		{
			// Start death animation
			Act(String("Die"));
		}

		m_DeathAccuTime += dTime;

		if (m_DeathAccuTime >= m_DeathAnimationTime && m_DeathAccuTime <= (m_DeathAnimationTime + m_DeathWaitTime))
		{
			// Waiting before respawn, dramatic effect
			Act(String("Dead"));
		}
		else if (m_DeathAccuTime > (m_DeathAnimationTime + m_DeathWaitTime))
		{
			GiveLives(-1);
			if (m_Lives > 0)
			{
				// Dramatic effect over, respawn
				m_DeathAccuTime = 0.0;
				Respawn();
			}
		}
	}

}

void Character::GiveScore(int amount)
{

	// No negative scores
	if (amount > 0)
	{
		m_Score += amount;
	}

}

int Character::GetCredits()
{

	return m_Credits;

}

int Character::GetScore()
{

	return m_Score;

}

double Character::GetHealth()
{

	return m_Health;

}

double Character::GetFuel()
{

	return m_Fuel;

}

double Character::GetPulseAmmo()
{

	return m_PulseAmmo;

}

int Character::GetMissiles()
{

	return m_Missiles;

}

int Character::GetLives()
{

	return m_Lives;

}

DOUBLE2 Character::GetPosition()
{

	return m_ActPtr->GetPosition();

}

Character::WeaponType Character::GetCurrentWeapon()
{

	return m_CurrentWeapon;

}

void Character::Act(String anim)
{

	m_SpritePtr->SetAnimation(anim);

}

void Character::ListenForMovement(DOUBLE2& velocity)
{

	// Start by assuming we do nothing
	CharacterState state = CharacterState::IDLE;

	// Check if we're in front of a vendor, to buy
	if (GAME_ENGINE->IsKeyboardKeyPressed('B'))
	{
		Vendor* touchingVendor = ENT_MANAGER->TouchingVendor(m_ActPtr);
		if (touchingVendor != nullptr)
		{
			if (!touchingVendor->Buy(this))
			{
				m_HudPtr->Message(BUYFAIL);
			}
			else
			{
				m_State = CharacterState::BUYING;
			}
		}
	}

	// Animation for buying only takes a little bit
	if (m_State == CharacterState::BUYING)
	{
		// Can't do anything while buying, it leaves us vulnerable
		AnimateState();
		return;
	}

	// Check if we're moving to the left, by sliding or other, and flip us (unless flying / falling)
	if (m_ActPtr->GetLinearVelocity().x < 0)
	{
		m_SpritePtr->Flipped(true);
	}

	// Edge checking if on ground and not falling / flying
	if (m_LevelPtr->IsOnGround(m_ActPtr) && m_State != CharacterState::FLYING && m_State != CharacterState::FALLING)
	{
		// Flip if needed to make us face the edge
		if (m_ActPtr->GetLinearVelocity().y <= 0.1)
		{
			if (!m_LevelPtr->IsOnGround(m_ActLeftSidePtr))
			{
				m_SpritePtr->Flipped(true);
				state = CharacterState::NEAREDGE;
			}
			else if (!m_LevelPtr->IsOnGround(m_ActRightSidePtr))
			{
				m_SpritePtr->Flipped(false);
				state = CharacterState::NEAREDGE;
			}
		}
	}

	// Crouching
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_DOWN))
	{
		state = CharacterState::IDLECROUCH;
	}
	// Flying
	else if (GAME_ENGINE->IsKeyboardKeyDown(VK_UP) && m_Fuel > 0)
	{
		velocity.y -= m_JetpackSpeed;
		state = CharacterState::FLYING;
		m_Fuel -= m_JetpackFuelUsage;
	}

	// Falling
	if (state != CharacterState::FLYING && (!m_LevelPtr->IsOnGround(m_ActPtr) || !m_LevelPtr->IsOnGround(m_ActLeftSidePtr) || !m_LevelPtr->IsOnGround(m_ActRightSidePtr)))
	{
		if (m_ActPtr->GetLinearVelocity().y > 0)
		{
			// Falling
			state = CharacterState::FALLING;
		}
	}

	// Left and right
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_LEFT))
	{
		m_SpritePtr->Flipped(true);	// If we're going left, flip our sprites

		velocity.x -= m_MoveSpeed; // Move to the left

		if (state != CharacterState::FALLING && state != CharacterState::FLYING && m_ActPtr->GetLinearVelocity().y <= 0)
		{
			// If we're not falling or flying, we must be running
			state = CharacterState::RUNNING;
		}
	}
	else if (GAME_ENGINE->IsKeyboardKeyDown(VK_RIGHT))
	{
		m_SpritePtr->Flipped(false); // If we're going right, don't flip our sprites

		velocity.x += m_MoveSpeed; // Move to the right

		if (state != CharacterState::FALLING && state != CharacterState::FLYING && m_ActPtr->GetLinearVelocity().y <= 0)
		{
			// If we're not falling or flying, we must be running
			state = CharacterState::RUNNING;
		}
	}

	// Fix hitboxes if we crouch or stop crouching
	if (state == CharacterState::IDLECROUCH && m_State != CharacterState::IDLECROUCH)
	{
		// We just started crouching, smaller hitbox
		m_ActPtr->DestroyFixture(m_CollisionFixturePtr);
		m_CollisionFixturePtr = m_ActPtr->AddBoxFixture(22, 26, 0.0, 0.0);
		m_ActPtr->SetPosition(m_ActPtr->GetPosition().x, m_ActPtr->GetPosition().y + 4);
		m_EdgeDetectorsOffset = DOUBLE2(11.0, 13.0);

	}
	if (state != CharacterState::IDLECROUCH && m_State == CharacterState::IDLECROUCH)
	{
		// We stopped crouching, return hitbox to normal size
		m_ActPtr->DestroyFixture(m_CollisionFixturePtr);
		m_CollisionFixturePtr = m_ActPtr->AddBoxFixture(22, 34, 0.0, 0.0);
		m_ActPtr->SetPosition(m_ActPtr->GetPosition().x, m_ActPtr->GetPosition().y - 4);
		m_EdgeDetectorsOffset = DOUBLE2(11.0, 17.0);
	}
	
	// Play the fiting animation for the state
	m_State = state;
	AnimateState();

}

void Character::ListenForAttacking(double dTime)
{
	
	// Make sure our state is in something that lets us attack
	if (!(m_State == CharacterState::IDLE || m_State == CharacterState::IDLECROUCH || m_State == CharacterState::RUNNING || m_State == CharacterState::FLYING))
	{
		// Can't attack in this state, stop here
		m_Firing = false;
		return;
	}

	// Check if player is holding down the attack button
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_CONTROL))
	{
		// Fire
		switch (m_CurrentWeapon)
		{
		case Character::FLAMETHROWER:
			FireFlamethrower(dTime);
			break;
		case Character::PULSEGUN:
			FirePulsegun(dTime);
			break;
		case Character::GRENADES:
			FireGrenade();
			break;
		case Character::MISSILES:
			FireMissile();
			break;
		default:
			FireFlamethrower(dTime);
			break;
		}
		return;
	}
	else if (GAME_ENGINE->IsKeyboardKeyReleased(VK_CONTROL))
	{
		// Stop firing
		m_Firing = false;
	}
	
}

void Character::ListenForWeaponSwap()
{

	// 1, 2, 3, 4 for weapon swapping
	// 0x31 to 0x39 is 1 to 9 (non numeric keypad)
	if (GAME_ENGINE->IsKeyboardKeyPressed(0x31))
	{
		EquipWeapon(FLAMETHROWER);
	}
	else if (GAME_ENGINE->IsKeyboardKeyPressed(0x32))
	{
		EquipWeapon(PULSEGUN);
	}
	else if (GAME_ENGINE->IsKeyboardKeyPressed(0x33))
	{
		EquipWeapon(GRENADES);
	}
	else if (GAME_ENGINE->IsKeyboardKeyPressed(0x34))
	{
		EquipWeapon(MISSILES);
	}

	// Space for weapon cycling
	if (GAME_ENGINE->IsKeyboardKeyPressed(VK_SPACE))
	{

		switch (m_CurrentWeapon)
		{
		case Character::FLAMETHROWER:
			if (m_PulseAmmo > 0)
			{
				EquipWeapon(PULSEGUN);
			}
			break;
		case Character::PULSEGUN:
			if (m_Missiles > 0)
			{
				EquipWeapon(MISSILES);
			}
			else
			{
				EquipWeapon(FLAMETHROWER);
			}
			break;
		case Character::GRENADES:
			if (m_Grenades > 0)
			{
				EquipWeapon(MISSILES);
			}
		case Character::MISSILES:
			EquipWeapon(FLAMETHROWER);
			break;
		default:
			EquipWeapon(FLAMETHROWER);
			break;
		}

	}

}

void Character::FireFlamethrower(double dTime)
{

	// No fuel = don't fire
	if (m_Fuel <= 0)
	{
		return;
	}

	m_Firing = true;
	m_ShootAccuTime += dTime;

	// Delay ended, shoot another fire projectile
	if (m_ShootAccuTime >= m_FlamethrowerDelay)
	{
		// Calculate the shooting offset
		DOUBLE2 shootpos = m_ActPtr->GetPosition();
		DOUBLE2 vel = m_FlamethrowerVelocity;
		DOUBLE2 offset;

		// Compare to state, set appropriate offset
		if (m_State == CharacterState::IDLECROUCH)
		{
			offset = m_ShootOffsetCrouching;
		}
		else
		{
			offset = m_ShootOffsetStanding;
		}
		// Check if flipped, if so reverse our x offset and velocity
		if (m_SpritePtr->Flipped())
		{
			offset.x *= -1;
			vel.x *= -1;
		}
		// Apply the offset
		shootpos += offset;

		// Velocity also increases or decreases along with character velocity (to prevent the character catching up to projectiles and whatnot)
		// vel += DOUBLE2(m_ActPtr->GetLinearVelocity().x, 0);
		// Removed (for now?), weird bug gives insane velocity bonus when tapping movement keys (Possible box2D thing)

		m_SndFlamethrowerShootPtr->Play();
		new Projectile(shootpos, m_SpriteFlamethrowerPtr, vel, m_FlamethrowerPhysicsSize, m_FlameLifetime, m_FlamethrowerDamage);
		m_ShootAccuTime -= m_FlamethrowerDelay;

		// Take fuel
		GiveFuel(-m_FlamethrowerFuelUsage);
	}

}

void Character::FirePulsegun(double dTime)
{

	// No ammo = don't fire
	if (m_PulseAmmo <= 0)
	{
		return;
	}

	m_Firing = true;
	m_ShootAccuTime += dTime;

	// Delay ended, shoot another fire projectile
	if (m_ShootAccuTime >= m_PulseShotDelay)
	{
		// Calculate the shooting offset
		DOUBLE2 shootpos = m_ActPtr->GetPosition();
		DOUBLE2 vel = m_PulseShotVelocity;
		DOUBLE2 offset;

		// Compare to state, set appropriate offset
		if (m_State == CharacterState::IDLECROUCH)
		{
			offset = m_ShootOffsetCrouching;
		}
		else
		{
			offset = m_ShootOffsetStanding;
		}
		// Check if flipped, if so reverse our x offset and velocity
		if (m_SpritePtr->Flipped())
		{
			offset.x *= -1;
			vel.x *= -1;
		}
		// Apply the offset
		shootpos += offset;

		m_SndPulsegunShootPtr->Play();
		new Projectile(shootpos, m_SpritePulseShotPtr, vel, m_PulseShotPhysicsSize, m_PulseShotLifetime, m_PulseShotDamage);
		m_ShootAccuTime -= m_PulseShotDelay;

		// Take pulse ammo
		GivePulseAmmo(-m_PulseShotAmmoUsage);

		// Check if we still have ammo, if not equip the flamethrower
		if (m_PulseAmmo <= 0)
		{
			EquipWeapon(FLAMETHROWER);
		}

	}

}

void Character::FireGrenade()
{

}

void Character::FireMissile()
{


}

void Character::EquipWeapon(WeaponType weapon)
{
	
	switch (weapon)
	{
	case Character::FLAMETHROWER:
		m_CurrentWeapon = FLAMETHROWER;
		m_ShootAccuTime = m_FlamethrowerDelay;
		break;
	case Character::PULSEGUN:
		m_CurrentWeapon = PULSEGUN;
		m_ShootAccuTime = m_PulseShotDelay;
		break;
	case Character::GRENADES:
		m_CurrentWeapon = GRENADES;
		break;
	case Character::MISSILES:
		m_CurrentWeapon = MISSILES;
		break;
	default:
		m_CurrentWeapon = FLAMETHROWER;
		break;
	}

}

void Character::AnimateState()
{

	// Here we check what state we are in and set the corresponding animations
	switch (m_State)
	{
	case CharacterState::IDLE:
		Act(String("Idle"));
		break;
	case CharacterState::IDLECROUCH:
		Act(String("IdleCrouch"));
		break;
	case CharacterState::RUNNING:
		Act(String("Run"));
		break;
	case CharacterState::FALLING:
		Act(String("Fall"));
		break;
	case CharacterState::FLYING:
		Act(String("Fly"));
		break;
	case CharacterState::NEAREDGE:
		Act(String("Edge"));
		break;
	case CharacterState::BUYING:
		Act(String("Buy"));
		break;
	}

}

void Character::TickJetpackSmoke(double dTime)
{

	// Only work when flying
	if (m_State == CharacterState::FLYING)
	{
		m_JetpackSmokeAccuTime += dTime;

		// New smoke?
		if (m_JetpackSmokeAccuTime >= m_JetpackSmokeDelay)
		{
			// Calculate the spawning position
			DOUBLE2 spawnPos = m_ActPtr->GetPosition();
			DOUBLE2 offset = m_JetpackSmokeOffset;
			if (m_SpritePtr->Flipped())
			{
				offset.x *= -1;
			}
			spawnPos += offset;

			// Spawn the smoke
			new Effect(spawnPos, m_SpriteJetpackSmokePtr, m_JetpackSmokeLifetime);
			// Reset the accumulated time since last spawn
			m_JetpackSmokeAccuTime -= m_JetpackSmokeDelay;
		}
	}

}

void Character::Tick(double dTime)
{

	// Tick for dying checks
	TickDeath(dTime);

	// Tick for our sprite
	m_SpritePtr->Tick(dTime);

	// Stop here, we died
	if (m_State == CharacterState::DEAD)
	{
		return;
	}

	// Update time since last damage
	m_DamageTakenAccu += dTime;

	// Starting velocity before we calculate movement
	DOUBLE2 moveVelocity = DOUBLE2(0, 0);

	// Listen for input
	ListenForMovement(moveVelocity);
	ListenForAttacking(dTime);
	ListenForWeaponSwap();

	// Apply our velocity to our Physicsactors
	m_ActPtr->SetLinearVelocity(moveVelocity);

	// Tick our Jetpack, to see if we need to make new smoke
	TickJetpackSmoke(dTime);

	// Tick for our pilot flame
	m_SpritePilotFlamePtr->Tick(dTime);

	// Tick to notify player that the finish has unlocked
	if (!m_AllCaptivesSaved && ENT_MANAGER->GetCaptiveCount() <= 0)
	{
		// Doesn't actually win, but we need to let the player know that he saved all the captives through a transmission
		m_HudPtr->Message(Messages::VICTORY);
		m_AllCaptivesSaved = true;
	}

	// Tick for buying animation
	if (m_State == CharacterState::BUYING)
	{
		m_BuyAccuTime += dTime;
		if (m_BuyAccuTime >= m_BuyingTime)
		{
			m_BuyAccuTime = 0.0;
			m_State = CharacterState::IDLE;
		}
	}

}

void Character::PaintPilotFlame()
{

	// Check if we have the flamethrower equipped
	if (m_CurrentWeapon != FLAMETHROWER)
	{
		return;
	}

	// Check if we're currently firing our flamethrower, if we are we have to hide the pilot flame
	// Also hide it if we have no fuel
	if (m_Firing || m_Fuel <= 0)
	{
		return;
	}

	// Check if we are in a state to draw the flame in
	if (!(m_State == CharacterState::IDLE || m_State == CharacterState::IDLECROUCH || m_State == CharacterState::RUNNING || m_State == CharacterState::FLYING))
	{
		// No flame in these states, stop here
		return;
	}

	DOUBLE2 drawPosition;

	// If we crouch, use the crouch offset, if standing use the standing offset for our flame
	if (m_State == CharacterState::IDLECROUCH)
	{
		drawPosition = m_PilotFlameOffsetCrouching;
	}
	else
	{
		drawPosition = m_PilotFlameOffsetStanding;
	}

	// Paint our pilot flame
	m_SpritePilotFlamePtr->Paint(drawPosition);

}

void Character::Paint()
{

	// Paint our sprite at our Physicsactor's location
	m_SpritePtr->Paint(m_ActPtr->GetPosition());

	if (m_State == CharacterState::DEAD)
	{
		return; // We're dead, don't paint anything but our character
	}

	// Reposition our edge detectors
	// If we do this in tick, they visually 'lag' in debug mode
	m_ActLeftSidePtr->SetPosition(m_ActPtr->GetPosition() + DOUBLE2(-m_EdgeDetectorsOffset.x, m_EdgeDetectorsOffset.y));
	m_ActRightSidePtr->SetPosition(m_ActPtr->GetPosition() + m_EdgeDetectorsOffset);
	
	// Paint the pilot flame
	PaintPilotFlame();
	
}
