//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Coin.h"
#include "Sprite.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "Character.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define RESOURCE_MANAGER (ResourceManager::GetInstance())
#define ENT_MANAGER (EntityManager::GetInstance())

Coin::Coin(DOUBLE2 pos) : Entity()
{

	// Create the sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/coin.png"), 0, 0, 1, 6, 6);
	// Play idle
	m_SpritePtr->SetAnimation(String("Idle"));

	// Create the physicsactor
	m_ActPtr = new PhysicsActor(pos, 0, BodyType::DYNAMIC);
	m_ActPtr->AddBoxFixture(13, 20, 0.0, 1.0);
	m_ActPtr->SetGravityScale(0.05);
	m_ActPtr->AddContactListener(this);
	m_ActPtr->SetFixedRotation(true);

	m_ActPtr->SetUserData(EntityType::PICKUP);

	// Sounds
	m_SndSpawnPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/coindrop.wav"));
	m_SndPickupPtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/coin_pickup.wav"));
	m_SndBouncePtr = RESOURCE_MANAGER->GetSound(String("./resources/audio/sfx/coin_bounce.wav"));

	m_SndSpawnPtr->Play();

}

Coin::~Coin()
{

}

void Coin::Tick(double dTime)
{

	// Sprite tick
	m_SpritePtr->Tick(dTime);

	// Fake bouncing sound effect (bouncing is in the animation)
	m_BounceAccuTime += dTime;
	if (m_BounceAccuTime >= m_BounceDelay)
	{
		m_BounceAccuTime -= m_BounceDelay;
		// Dont play the sound if the player isnt nearby
		DOUBLE2 charcPos = ENT_MANAGER->GetPlayer()->GetPosition();
		DOUBLE2 myPos = m_ActPtr->GetPosition();
		if (sqrt(pow(charcPos.y - myPos.y, 2) + pow(charcPos.x - myPos.x, 2)) <= 180)
		{
			m_SndBouncePtr->Play();
		}
	}

	// Flag destroy part
	if (m_ShouldDestroy)
	{
		ENT_MANAGER->RemoveEntity(this);
	}

}

void Coin::PreSolve(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr, bool& enableContactRef)
{

	// Allow the player to move through us
	if (actOtherPtr->GetUserData() == int(EntityType::WORLD))
	{
		enableContactRef = true;
	}
	else
	{
		enableContactRef = false;
	}

}

void Coin::BeginContact(PhysicsActor* actThisPtr, PhysicsActor* actOtherPtr)
{

	if (actOtherPtr->GetUserData() == int(EntityType::PLAYER))
	{
		ENT_MANAGER->GetPlayer()->GiveCredits(1);
		m_SndPickupPtr->Play();
		FlagDestroy();
	}

}