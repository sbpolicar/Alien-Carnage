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
	m_SpritePtr->AddAnimation(String("Edge"), String("./resources/images/characters/harry/anim_nearedge.png"), 0, 0, 1, 2, 5);
	m_SpritePtr->AddAnimation(String("Run"), String("./resources/images/characters/harry/anim_run.png"), 0, 0, 1, 6, 8);
	// Set the starting animation
	m_SpritePtr->SetAnimation(String("Fall"));

	// Create our edge detection physicsactors
	m_ActLeftSidePtr = new PhysicsActor(m_ActPtr->GetPosition(), 0, BodyType::KINEMATIC);
	m_ActLeftSidePtr->AddBoxFixture(2, 2);
	m_ActLeftSidePtr->SetSensor(true);
	m_ActRightSidePtr = new PhysicsActor(m_ActPtr->GetPosition(), 0, BodyType::KINEMATIC);
	m_ActRightSidePtr->AddBoxFixture(2, 2);
	m_ActRightSidePtr->SetSensor(true);

	// Create the flamethrower fire sprite, NEEDS WORK
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

	// Edge detection 'feet'
	delete m_ActLeftSidePtr;
	m_ActLeftSidePtr = nullptr;
	delete m_ActRightSidePtr;
	m_ActRightSidePtr = nullptr;

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

		if (state != CharacterState::FALLING && state != CharacterState::FLYING)
		{
			// If we're not falling or flying, we must be running
			state = CharacterState::RUNNING;
		}
	}
	else if (GAME_ENGINE->IsKeyboardKeyDown(VK_RIGHT))
	{
		m_SpritePtr->Flipped(false); // If we're going right, don't flip our sprites

		velocity.x += m_MoveSpeed; // Move to the right

		if (state != CharacterState::FALLING && state != CharacterState::FLYING)
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
	
	// Firing, HEAVY WIP

	// Make sure our state is in something that lets us attack
	if (!(m_State == CharacterState::IDLE || m_State == CharacterState::IDLECROUCH || m_State == CharacterState::RUNNING || m_State == CharacterState::FLYING))
	{
		// Can't attack in this state, stop here
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

	m_Firing = true;
	m_FlamethrowerAccuTime += dTime;

	// Delay ended, shoot another fire projectile
	if (m_FlamethrowerAccuTime >= m_FlamethrowerDelay)
	{
		// Calculate the shooting offset
		DOUBLE2 shootpos = m_ActPtr->GetPosition();

		if (m_State == CharacterState::IDLECROUCH)
		{
			shootpos += m_FlamethrowerOffsetCrouching;
		}
		else
		{
			shootpos += m_FlamethrowerOffsetStanding;
		}

		ENT_MANAGER->AddEntity(new Projectile(m_ActPtr->GetPosition() + m_FlamethrowerOffsetStanding, m_SpriteFlamethrowerPtr, m_FlamethrowerVelocity, m_FlamethrowerPhysicsSize, m_FlameLifetime));
		m_FlamethrowerAccuTime -= m_FlamethrowerDelay;
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

void Character::Tick(double dTime)
{

	DOUBLE2 moveVelocity = DOUBLE2(0, 0);

	// Listen for input
	ListenForMovement(moveVelocity);
	ListenForAttacking(dTime);

	// Apply our velocity to our Physicsactors
	m_ActPtr->SetLinearVelocity(moveVelocity);

	// Tick our Jetpack, to see if we need to make new smoke

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
		// No flame in this state!
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