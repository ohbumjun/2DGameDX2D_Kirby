#pragma once
#include "IMGUIWindow.h"

class CLineEditWindow :
    public CIMGUIWindow
{
public:
	CLineEditWindow();
	virtual ~CLineEditWindow() override;
private: 
	class CIMGUIText* m_LineStartPosX;
	class CIMGUIText* m_LineStartPosY;
	class CIMGUIText* m_LineEndPosX;
	class CIMGUIText* m_LineEndPosY;
	class CIMGUIText* m_LineSlope;
	class CIMGUIListBox* m_CreatedLineListBox;
private:
	// Tile_EditMode m_EditMode; --> 무조건 여기서는 Type만 적용할 것이다.
	class CLineContainer* m_LineContainer;
	class CGameObject* m_SelectLine;
public:
	void SetLineContainer(class CGameObject* LineContainer);
	void SetLineInfo(class CLine* Line);
	void SetLineDescription(const Vector3& FinalLeftPos, const Vector3& FinalRightPos, float Slope);
public:
	class CGameObject* GetSelectLine() const
	{
		return m_SelectLine;
	}
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private:
	void SelectLineObject(int Index, const char* Name);
	void SetLineCreateMode();
	void SetLineEditMode();
	void CreateNewLine();
};

