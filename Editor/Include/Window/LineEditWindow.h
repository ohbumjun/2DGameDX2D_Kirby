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
	// Tile_EditMode m_EditMode; --> ������ ���⼭�� Type�� ������ ���̴�.
	class CLineContainer* m_LineContainer;
public:
	void SetLineContainer(class CGameObject* LineContainer);
	void SetLineInfo(class CLine* Line);
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private:
	void SetEditModeCallback(int Index, const char* Name);
	void SetLineCreateMode();
	void SetLineEditMode();
	void CreateNewLine();
};

