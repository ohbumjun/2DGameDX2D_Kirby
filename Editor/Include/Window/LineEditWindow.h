#pragma once
#include "IMGUIWindow.h"

class CLineEditWindow :
    public CIMGUIWindow
{
public:
	CLineEditWindow();
	virtual ~CLineEditWindow() override;

private: // todo :  BackGround Info
	class CIMGUIButton* m_BackGroundImageLoadButton;
	class CIMGUIImage* m_BackGroundImageSprite;
	class CIMGUIText* m_BackImgScaleX;
	class CIMGUIText* m_BackImgScaleY;
	class CIMGUIText* m_BaseImgScrollRatio;
	class CIMGUITextInput* m_BaseImgScrollRatioInput;
private:
	// Tile_EditMode m_EditMode; --> 무조건 여기서는 Type만 적용할 것이다.
	class CBackGroundComponent* m_BackGround;
public:
	/*
	void SetTileEditMode (Tile_EditMode EditMode)
	{
		m_EditMode = EditMode;
	}
	*/
	void SetBackGround(class CBackGroundComponent* BackGround);
public:
	/*
	Tile_EditMode GetTileEditMode () const
{
		return m_EditMode;
}
*/
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private:
	void SetEditModeCallback(int Index, const char* Name);
private:
	void BackGroundImageLoadButton();
	void SetBackGroundScrollRatio();

};

