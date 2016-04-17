#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Van Handenhove Brent
// Group: 1DAE16
//-----------------------------------------------------------------

class Animation;

class Sprite 
{
public:

	Sprite();
	virtual ~Sprite( );

	// Make objects non-copyable
	// https://msdn.microsoft.com/en-us/library/dn457344.aspx 
	Sprite( const Sprite& ) = delete;
	Sprite& operator=( const Sprite& ) = delete;

	void Tick(double dTime);
	void Paint(DOUBLE2 position);
	void AddAnimation(String name, String bitmap);
	void AddAnimation(String name, String bitmap, int startCol, int startRow, int rows, int cols, double FPS);
	void SetAnimation(String name);
	void SetParent(Sprite* spriteOtherPtr);
	bool Flipped();
	void Flipped(bool flip);
	Sprite* Copy();

private:

	// Parenting to other sprites
	DOUBLE2 m_Position = DOUBLE2(0, 0);
	Sprite* m_SpriteParentPtr = nullptr;

	// Animations
	std::vector<Animation*> m_Animations;
	Animation* m_CurrentAnim = nullptr;
	bool m_Flipped = false;

};
