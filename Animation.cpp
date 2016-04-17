//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------
#include "stdafx.h"		
#include "Animation.h"
#include <fstream>

#define GAME_ENGINE (GameEngine::GetSingleton())

Animation::Animation(String name, String bitmap)
	: m_AnimationName(name)
{

	// Create the bitmap
	SetBitmap(bitmap);

}

Animation::Animation(String name, String bitmap, int startCol, int startRow, int rows, int cols, double FPS) 
	: m_AnimationName(name), m_StartCol(startCol), m_StartRow(startRow), m_Rows(rows), m_Cols(cols), m_FPS(FPS)
{
	
	// Create the bitmap after the settings are configured
	SetBitmap(bitmap);

}

void Animation::SetBitmap(String bitmap)
{

	// If bitmap already exists, stop
	if (m_BmpImagePtr != nullptr)
	{
		return;
	}

	// Bitmap doesn't exist yet, continue
	// Check if the path is valid
	std::ifstream fStream;
	fStream.open(bitmap.C_str());

	if (fStream.fail())
	{
		// The path doesn't exist, report error and ignore
		std::cout << "ANIMATION Error bitmap not found: " << bitmap.C_str() << std::endl;
		return;
	}
	else
	{
		// Close the stream, it has no use for us anymore
		fStream.close();

		// Set path
		m_AnimationPath = bitmap;

		// Path exists, continue creating
		std::cout << "ANIMATION Initializing: " << bitmap.C_str() << std::endl;
		m_BmpImagePtr = new Bitmap(bitmap);

		// Get some stuff ready for animating
		m_FrameW = m_BmpImagePtr->GetWidth() / m_Cols;
		m_FrameH = m_BmpImagePtr->GetHeight() / m_Rows;
		m_TotalFrames = m_Rows * m_Cols;
	}

}

Animation::~Animation()
{

	// Clear the bitmap if it exists
	if (m_BmpImagePtr != nullptr)
	{
		delete m_BmpImagePtr;
		m_BmpImagePtr = nullptr;
	}

}

String Animation::GetName()
{

	return m_AnimationName;

}

String Animation::GetPath()
{

	return m_AnimationPath;

}

int Animation::GetCols()
{

	return m_Cols;

}

int Animation::GetRows()
{

	return m_Rows;

}

double Animation::GetFPS()
{

	return m_FPS;

}

void Animation::Tick(double dTime)
{

	// Only care about frames if there is actually more than 1
	if (m_TotalFrames > 1)
	{
		m_ElapsedTime += dTime;

		if (m_ElapsedTime >= (1.0 / m_FPS))
		{
			m_CurrentFrame = (++m_CurrentFrame) % m_TotalFrames;
			m_ElapsedTime -= (1.0 / m_FPS);
		}
	}

}

void Animation::Paint(DOUBLE2 pos)
{


	if (m_BmpImagePtr == nullptr)
	{
		// Don't draw if the bitmap is a nullptr, safety precaution
		return;
	}

	// Determine the frame Rect
	RECT2 frameSrc;
	frameSrc.left = (m_CurrentFrame % m_Cols) * m_FrameW;
	frameSrc.right = frameSrc.left + m_FrameW;
	frameSrc.top = (m_CurrentFrame / m_Cols) * m_FrameH;
	frameSrc.bottom = frameSrc.top + m_FrameH;

	// Correct the position with an offset
	pos.x -= m_FrameW / 2;
	pos.y -= m_FrameH / 2;

	// Draw!
	GAME_ENGINE->DrawBitmap(m_BmpImagePtr, pos, frameSrc);

}