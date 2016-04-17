#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Animation 
{
public:

	Animation(String name, String bitmap); // For single frame
	Animation(String name, String bitmap, int startX, int startY, int rows, int cols, double FPS); // For multiple frames
	virtual ~Animation( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Animation( const Animation& ) = delete;
	Animation& operator=( const Animation& ) = delete;

	String GetName();
	String GetPath();
	int GetCols();
	int GetRows();
	double GetFPS();

	void Tick(double dTime);
	void Paint(DOUBLE2 pos);

private: 

	void SetBitmap(String bitmap);

	//  General info
	String m_AnimationName = String("Idle");
	String m_AnimationPath = String("");
	Bitmap* m_BmpImagePtr = nullptr;

	// Animation config
	int m_StartCol = 0;
	int m_StartRow = 0;
	int m_Rows = 1;
	int m_Cols = 1;
	// Frames
	double m_FPS = 1.0;
	double m_ElapsedTime = 0.0;
	int m_CurrentFrame = 0;
	int m_TotalFrames = 1;
	int m_FrameW = 0;
	int m_FrameH = 0;

};
