//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Camera.h"
#include "Character.h"
#include "Level.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Camera::Camera(int w, int h) : m_Width(w), m_Height(h)
{
}

Camera::~Camera()
{
}

void Camera::TrackPlayer(DOUBLE2& posRef, Character* charPtr)
{

	// Get the player's position and set our position accordingly
	posRef = charPtr->GetPosition();

}

void Camera::ClampToLevel(DOUBLE2& posRef, Level* levelPtr)
{

	// Check x position
	if (posRef.x < m_Width / 2)
	{
		posRef.x = m_Width / 2;
	}
	else if (posRef.x > (levelPtr->GetWidth() * levelPtr->GetScale()) - m_Width / 2)
	{
		posRef.x = levelPtr->GetWidth() - m_Width / 2;
	}

	// Check y position
	if (posRef.y < m_Height / 2)
	{
		posRef.y = m_Height / 2;
	}
	else if (posRef.y > (levelPtr->GetHeight() * levelPtr->GetScale()) - m_Height / 2)
	{
		posRef.y = levelPtr->GetHeight() - m_Height / 2;
	}

}

MATRIX3X2 Camera::GetViewMatrix(Level* lvlPtr, Character* charPtr)
{

	// Get the position of the camera
	DOUBLE2 position;
	TrackPlayer(position, charPtr);

	// Clamp to the level bounds
	ClampToLevel(position, lvlPtr);

	// Apply an offset to center the camera
	position -= DOUBLE2(m_Width/2, m_Height/2);

	// Calculate the camera matrix and invert it for the view matrix, then return this
	MATRIX3X2 matTranslate = MATRIX3X2::CreateTranslationMatrix(position * lvlPtr->GetScale());

	return matTranslate.Inverse();

}