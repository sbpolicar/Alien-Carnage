//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Camera.h"
#include "Character.h"
#include "Level.h"
#include "HUD.h"

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
	posRef += charPtr->GetPosition();

}

void Camera::ClampToLevel(DOUBLE2& posRef, Level* levelPtr)
{

	// The actual visible area is smaller because our game is not filling the window
	// So we need to do a slight adjustment to check our 'visible' edge, not the actual, hidden edge of the view
	
	// Check x position
	if (posRef.x + m_Offset.x < m_Width / 2)
	{
		posRef.x = m_Width / 2 - m_Offset.x;
	}
	else if (posRef.x + m_Width / 2 - m_Offset.x > levelPtr->GetWidth())
	{
		posRef.x = levelPtr->GetWidth() - m_Width / 2 + m_Offset.x;
	}

	// Check y position
	if (posRef.y < m_Height / 2)
	{
		posRef.y = m_Height / 2;
	}
	else if (posRef.y - m_Offset.y > levelPtr->GetHeight() - m_Height / 2) // Only need offset here because the HUD is at the bottom
	{
		posRef.y = levelPtr->GetHeight() - m_Height / 2 + m_Offset.y;
	}

}

MATRIX3X2 Camera::GetViewMatrix(Level* lvlPtr, Character* charPtr)
{

	// Get the position of the camera
	// Use an offset because our actual game is smaller than the window size
	TrackPlayer(m_TargetPosition, charPtr);

	// Clamp to the level bounds
	ClampToLevel(m_TargetPosition, lvlPtr);
	
	// Calculate the camera matrix and invert it for the view matrix
	MATRIX3X2 matTranslate = MATRIX3X2::CreateTranslationMatrix(m_CurrentPosition * lvlPtr->GetScale());

	return (matTranslate).Inverse();

}

void Camera::Tick(Character* charcPtr, double dTime)
{

	// Set the target
	m_TargetPosition = charcPtr->GetPosition() - DOUBLE2(m_Width / 2, m_Height / 2) + DOUBLE2(0.0, m_Offset.y / 2);;
	m_StartPosition = m_CurrentPosition;
	
	// "Freedom box", where the character can move around without the camera moving, check
	if ((abs(m_CurrentPosition.x - m_TargetPosition.x) >= m_FreedomBoxSize.x / 2)
		|| abs(m_CurrentPosition.y - m_TargetPosition.y) >= m_FreedomBoxSize.y / 2)
	{
		// Interpolate
		if (m_CurrentPosition == m_TargetPosition)
		{
			// Target has been reached
			m_CurrentPosition = m_TargetPosition;
			m_StartPosition = m_TargetPosition;
		}
		else
		{
			// Calculate the distance we have to travel to the target per deltatime, keeping speed as a factor
			// Then apply it to our position to get slightly closer to the target
			DOUBLE2 stepSize = m_TargetPosition - m_StartPosition;
			stepSize = stepSize * dTime * m_InterpolateSpeed;
			m_CurrentPosition += stepSize;
		}
	}

}