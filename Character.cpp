//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Character.h"
#include "Sprite.h"
#include "Level.h"
#include "EntityManager.h"
#include "Projectile.h" // TEMP TEMP TEMP

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Character::Character(Level* levelRef)
{

	// Console
	std::cout << "\nCreating player" << std::endl;

	// Save the level pointer for future stuff
	m_LevelPtr = levelRef;

	// Create the Physicsactor
	m_ActPtr = new PhysicsActor(m_LevelPtr->GetSpawnPosition(), 0, BodyType::DYNAMIC);
	m_ActPtr->AddBoxFixture(22, 34, 0.0, 0.0);
	m_ActPtr->SetFixedRotation(true);

	// Create the Sprite and add our animations
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/characters/harry/anim_stand.png"));
	m_SpritePtr->AddAnimation(String("IdleCrouch"), String("./resources/images/characters/harry/anim_crouch.png"));
	m_SpritePtr->AddAnimation(String("Fall"), String("./resources/images/characters/harry/anim_falling.png"));
	m_SpritePtr->AddAnimation(String("Fly"), String("./resources/images/characters/harry/anim_fly.png"));
	m_SpritePtr->AddAnimation(String("Edge"), String("./resources/images/characters/harry/anim_nearedge.png"), 0, 0, 1, 2, 3);
	m_SpritePtr->AddAnimation(String("Run"), String("./resources/images/characters/harry/anim_run.png"), 0, 0, 1, 6, 10);
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
	m_SpriteFlamethrowerPtr->AddAnimation(String("Idle"), String("./resources/images/effects/flamethrower_flame.png"), 0, 0, 1, 8, 30);
	// Set the starting animation
	m_SpriteFlamethrowerPtr->SetAnimation(String("Idle"));

	// Create the pilot flame sprite
	m_SpritePilotFlamePtr = new Sprite();
	m_SpritePilotFlamePtr->SetParent(m_SpritePtr);
	m_SpritePilotFlamePtr->AddAnimation(String("Idle"), String("./resources/images/effects/flamethrower_pilotflame.png"), 0, 0, 1, 3, 9);
	// Set the starting animation
	m_SpritePilotFlamePtr->SetAnimation(String("Idle"));

	// Create smoke sprite
	m_SpriteJetpackSmokePtr = new Sprite();
	m_SpriteJetpackSmokePtr->AddAnimation(String("Idle"), String("./resources/images/effects/jetpack_smoke.png"), 0, 0, 1, 5, 15);
	// set the starting animation
	m_SpriteJetpackSmokePtr->SetAnimation(String("Idle"));

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

void Character::GiveLives(int amount)
{

	// Lives are between 0 and 5
	m_Lives = (m_Lives + amount) % 5;

	// Can't go below 1 for now (0 = death)
	if (m_Lives < 1)
	{
		m_Lives = 1;
	}

}

void Character::TakeDamage(double amount)
{

	m_Health -= amount;
	
	// Can't get less than 0 health
	if (m_Health < 0)
	{
		m_Health = 0;
	}

}

int Character::GetCredits()
{

	return m_Credits;

}

int Character::GetCaptives()
{

	return m_Captives;

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

int Character::GetLives()
{

	return m_Lives;

}

DOUBLE2 Character::GetPosition()
{

	return m_ActPtr->GetPosition();

}

void Character::Act(String anim)
{

	m_SpritePtr->SetAnimation(anim);

}

void Character::ListenForMovement(DOUBLE2& velocity)
{

	// Start by assuming we do nothing
	CharacterState state = CharacterState::IDLE;

	// Check if we're moving to the left, by sliding or other, and flip us (unless flying / falling)
	if (m_ActPtr->GetLinearVelocity().x < 0)
	{
		m_SpritePtr->Flipped(true);
	}

	// Edge checking if on ground and not falling / flying
	if (m_LevelPtr->IsOnGround(m_ActPtr) && m_State != CharacterState::FLYING && m_State != CharacterState::FALLING)
	{
		// Flip if needed to make us face the edge
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

	// Crouching
	if (GAME_ENGINE->IsKeyboardKeyDown(VK_DOWN))
	{
		state = CharacterState::IDLECROUCH;
	}
	// Flying
	else if (GAME_ENGINE->IsKeyboardKeyDown(VK_UP))
	{
		velocity.y -= m_JetpackSpeed;
		state = CharacterState::FLYING;
	}

	// Falling
	if (state != CharacterState::FLYING && !m_LevelPtr->IsOnGround(m_ActPtr))
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
		FireFlamethrower(dTime);
	}
	else if (GAME_ENGINE->IsKeyboardKeyReleased(VK_CONTROL))
	{
		// Stop firing
		m_Firing = false;
		m_FlamethrowerAccuTime = m_FlamethrowerDelay;
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
	m_FlamethrowerAccuTime += dTime;

	// Delay ended, shoot another fire projectile
	if (m_FlamethrowerAccuTime >= m_FlamethrowerDelay)
	{
		// Calculate the shooting offset
		DOUBLE2 shootpos = m_ActPtr->GetPosition();
		DOUBLE2 vel = m_FlamethrowerVelocity;
		DOUBLE2 offset;

		// Compare to state, set appropriate offset
		if (m_State == CharacterState::IDLECROUCH)
		{
			offset = m_FlamethrowerOffsetCrouching;
		}
		else
		{
			offset = m_FlamethrowerOffsetStanding;
		}
		// Check if flipped, if so reverse our x offset and velocity
		if (m_SpritePtr->Flipped())
		{
			offset.x *= -1;
			vel.x *= -1;
		}
		// Apply the offset
		shootpos += offset;

		ENT_MANAGER->AddEntity(new Projectile(shootpos, m_SpriteFlamethrowerPtr, vel, m_FlamethrowerPhysicsSize, m_FlameLifetime));
		m_FlamethrowerAccuTime -= m_FlamethrowerDelay;

		// Take fuel
		GiveFuel(-m_FlamethrowerFuelUsage);
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
	}

}

void Character::TickJetpackSmoke(double dTime)
{

	// Only work when in a state that involves moving
	if (m_State != CharacterState::FALLING && m_State != CharacterState::FLYING && m_State != CharacterState::RUNNING)
	{
		// not moving, don't spawn smoke
		return;
	}

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
		ENT_MANAGER->AddEntity(new Projectile(spawnPos, m_SpriteJetpackSmokePtr, DOUBLE2(0, 0), m_FlamethrowerPhysicsSize, m_JetpackSmokeLifetime));
		// Reset the accumulated time since last spawn
		m_JetpackSmokeAccuTime -= m_JetpackSmokeDelay;
	}

}

void Character::Tick(double dTime)
{

	// Starting velocity before we calculate movement
	DOUBLE2 moveVelocity = DOUBLE2(0, 0);

	// Listen for input
	ListenForMovement(moveVelocity);
	ListenForAttacking(dTime);

	// Apply our velocity to our Physicsactors
	m_ActPtr->SetLinearVelocity(moveVelocity);

	// Tick our Jetpack, to see if we need to make new smoke
	TickJetpackSmoke(dTime);

	// Tick for our pilot flame
	m_SpritePilotFlamePtr->Tick(dTime);

	// And lastly, tick for our sprite
	m_SpritePtr->Tick(dTime);

}

void Character::PaintPilotFlame()
{

	// Check if we're currently firing our flamethrower, if we are we have to hide the pilot flame
	if (m_Firing)
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

	// Reposition our edge detectors
	// If we do this in tick, they visually 'lag'
	m_ActLeftSidePtr->SetPosition(m_ActPtr->GetPosition() + DOUBLE2(-m_EdgeDetectorsOffset.x, m_EdgeDetectorsOffset.y));
	m_ActRightSidePtr->SetPosition(m_ActPtr->GetPosition() + m_EdgeDetectorsOffset);

	// Paint our sprite at our Physicsactor's location
	m_SpritePtr->Paint(m_ActPtr->GetPosition());

	// Paint the pilot flame
	PaintPilotFlame();
	
}
