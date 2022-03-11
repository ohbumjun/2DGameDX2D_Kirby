#pragma once
#include "IMGUIWindow.h"

class CLineEditWindow :
    public CIMGUIWindow
{
public:
	CLineEditWindow();
	virtual ~CLineEditWindow() override;

private: // todo :  BackGround Info
	class CIMGUIText* m_LineStartPosX;
	class CIMGUIText* m_LineStartPosY;
	class CIMGUIText* m_LineEndPosX;
	class CIMGUIText* m_LineEndPosY;
	class CIMGUIText* m_LineSlope;
	class CIMGUIListBox* m_CreatedLineListBox;
private:
	// Tile_EditMode m_EditMode; --> 무조건 여기서는 Type만 적용할 것이다.
	class CBackGroundComponent* m_BackGround;
public:
	void SetBackGround(class CBackGroundComponent* BackGround);
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private:
	void SetEditModeCallback(int Index, const char* Name);
	void SetLineCreateMode();
	void SetLineEditMode();
};

