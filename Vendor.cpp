//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Vendor.h"
#include "Character.h"
#include "Sprite.h"
#include "EntityManager.h"

#define GAME_ENGINE (GameEngine::GetSingleton())
#define ENT_MANAGER (EntityManager::GetInstance())

Vendor::Vendor(DOUBLE2 pos, int type) : Entity(pos, DOUBLE2(26, 39), BodyType::STATIC)
{

	// The sprite
	m_SpritePtr = new Sprite();
	m_SpritePtr->AddAnimation(String("Idle"), String("./resources/images/entities/vendor/vendor") + String(type) + String(".png"));
	m_SpritePtr->SetAnimation(String("Idle"));

	// Make the physicsactor a sensor
	m_ActPtr->SetSensor(true);

	// Type
	switch (type)
	{
	case 1:
		m_Type = VendorType::FUEL;
		break;
	case 2:
		m_Type = VendorType::PULSEAMMO;
		break;
	case 3:
		m_Type = VendorType::GRENADES;
		break;
	case 4:
		m_Type = VendorType::MISSILES;
		break;
	};

	ENT_MANAGER->AddVendor(this);

}

Vendor::~Vendor()
{


}

bool Vendor::IsOverlapping(PhysicsActor * actOtherPtr)
{

	return m_ActPtr->IsOverlapping(actOtherPtr);

}

bool Vendor::Buy(Character* characterPtr)
{

	// Check if we have enough money
	if (characterPtr->GetCredits() < m_Cost)
	{
		// Nope, end here
		return false;
	}

	switch (m_Type)
	{
	case Vendor::FUEL:
		characterPtr->GiveFuel(100);
		break;
	case Vendor::PULSEAMMO:
		characterPtr->GivePulseAmmo(25);
		break;
	case Vendor::MISSILES:
		characterPtr->GiveMissiles(100);
		break;
	case Vendor::GRENADES:
		break;
	default:
		// It didn't buy because of a wrong type
		// Just abort here and pretend it worked, but don't take the player's credits
		return true;
		break;
	}

	characterPtr->GiveCredits(-m_Cost);

	return true;
	
}