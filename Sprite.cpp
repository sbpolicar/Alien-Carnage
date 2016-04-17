//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Sprite.h"
#include <fstream>
#include "Animation.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Sprite::Sprite()
{
}

Sprite::~Sprite()
{

	// Clean up the animations
	for (Animation* anim : m_Animations)
	{
		delete anim;
	}
	m_Animations.clear();

}

Sprite* Sprite::Copy()
{

	Sprite* copy = new Sprite();

	// Copy the animations
	for (size_t i = 0; i < m_Animations.size(); ++i)
	{
		Animation* anim = m_Animations[i];
		copy->AddAnimation(anim->GetName(), anim->GetPath(), 0, 0, anim->GetRows(), anim->GetCols(), anim->GetFPS());
	}
	// Set the current animation
	copy->SetAnimation(m_CurrentAnim->GetName());

	return copy;

}

void Sprite::AddAnimation(String name, String bitmap)
{

	// Add the animation at the end of our animations vector
	m_Animations.push_back(new Animation(name, bitmap));

}

void Sprite::AddAnimation(String name, String bitmap, int startCol, int startRow, int rows, int cols, double FPS)
{

	// Add the animation at the end of our animations vector
	m_Animations.push_back(new Animation(name, bitmap, startCol, startRow, rows, cols, FPS));

}

void Sprite::SetAnimation(String name)
{

	// Find the animation that has this name in our list of animations
	for (Animation* anim : m_Animations)
	{
		if (anim->GetName() == name)
		{
			// Found it, set it as our current animation
			m_CurrentAnim = anim;
			return;
		}
	}

	// We're still in the code, so report that the animation doesn't exist
	std::cout << "SPRITE Could not set animation: " << name.C_str() << std::endl;

}

void Sprite::Flipped(bool flip)
{
	
	m_Flipped = flip;

}

void Sprite::SetParent(Sprite* spriteOtherPtr)
{

	m_SpriteParentPtr = spriteOtherPtr;

}

void Sprite::Tick(double dTime)
{

	// Pass on the tick to our animation
	m_CurrentAnim->Tick(dTime);

}

void Sprite::Paint(DOUBLE2 position)
{
	
	// Safety check to see if there is a current animation
	if (m_CurrentAnim == nullptr)
	{
		std::cout << "SPRITE Tried to play animation but none were set" << std::endl;
		return;
	}

	// Update position, just in case another sprite uses this one as a parent
	m_Position = position;

	// If we have a parent, draw with the parent's position as origin and flip according to parent
	if (m_SpriteParentPtr != nullptr)
	{
		m_Flipped = m_SpriteParentPtr->m_Flipped;
		m_Position = m_SpriteParentPtr->m_Position;
	}

	MATRIX3X2 matTransform = MATRIX3X2::CreateScalingMatrix(1, 1);

	// Generate a matrix to flip our animations, if already flipped -> flip them back to normal
	if (m_Flipped)
	{
		MATRIX3X2 matScale = MATRIX3X2::CreateScalingMatrix(-1, 1);
		MATRIX3X2 matPivot = MATRIX3X2::CreateTranslationMatrix(-m_Position);
		MATRIX3X2 matTranslate = MATRIX3X2::CreateTranslationMatrix(m_Position);

		// Set this new matrix as our sprite's matrix
		matTransform = matPivot * matScale * matTranslate;
	}

	// Again, if we have a parent, re-add our offset
	if (m_SpriteParentPtr != nullptr)
	{
		m_Position += position;
	}

	// Temp store the old matrix so we don't mess with other systems
	MATRIX3X2 matOld = GAME_ENGINE->GetWorldMatrix();

	// Set the world matrix for this sprite
	GAME_ENGINE->SetWorldMatrix(matTransform * matOld);

	// Pass on the paint to our animation
	m_CurrentAnim->Paint(m_Position);

	// Reset to old matrix
	GAME_ENGINE->SetWorldMatrix(matOld);

}
