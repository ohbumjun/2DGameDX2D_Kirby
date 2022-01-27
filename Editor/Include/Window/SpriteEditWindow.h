#pragma once

#include "IMGUIWindow.h"

class CSpriteEditWindow :
	public CIMGUIWindow
{
public:
	CSpriteEditWindow();
	virtual ~CSpriteEditWindow() override;

private:
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_SpriteSampled;
	class CIMGUIImage* m_SpriteCurrentFrame;
private :
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimInputName;
private:
	class CIMGUITextInput* m_StartFramePosXInput;
	class CIMGUITextInput* m_StartFramePosYInput;
	class CIMGUITextInput* m_EndFramePosXInput;
	class CIMGUITextInput* m_EndFramePosYInput;
private:
	class CIMGUIComboBox* m_NewSeqAnimationLoop;
	class CIMGUIComboBox* m_NewSeqAnimationReverse;
	class CIMGUIComboBox* m_CurSeqAnimationLoop;
	class CIMGUIComboBox* m_CurSeqAnimationReverse;
private:
	class CIMGUITextInput* m_AddDivideNumberInput;
	class CIMGUITextInput* m_DivMultiNumberInput;
private:
	CSharedPtr<class CSpriteEditObject> m_SpriteObject;
	class CAnimationSequence2DInstance* m_Animation;
public :
	class CSpriteEditObject* GetSpriteEditObject() const
{
		return m_SpriteObject;
}
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
public:
	void LoadTextureButton();
	void SpriteEditButton();
	void DeleteAnimationSequence();
	void ClearAnimationSequence();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void DeleteFrameButton();
	void ClearFrameButton();
	void EditFrameButton();
	void PlayAnimationButton();
	void StopAnimationButton();
public :
	void SetNewAnimSequenceLoop(int, const char*);
	void SetNewAnimSequenceReverse(int, const char*);
	void SetCurAnimSequenceLoop(int, const char*);
	void SetCurAnimSequenceReverse(int, const char*);
public:
	void SaveSequence();
	void LoadSequence();
	void SaveAnimation();
	void LoadAnimation();
public:
	void SelectAnimationSequence(int, const char*);
	void SelectAnimationFrame(int, const char*);
public :
	void SetDragObjectToRightEnd();
	void SetDragObjectToLeftEnd();
	void SetDragObjectToTop();
	void SetDragObjectToBottom();
	void SetDragObjectTexture();
private :
	void DivideFrameWidthAndAdd();
	void DivideFrameHeightAndAdd();
private:
	void DivideFrameWidth();
	void DivideFrameHeight();
	void MultiplyFrameWidth();
	void MultiplyFrameHeight();
private :
	void MoveOneBlockRight();
	void MoveOneBlockLeft();
	void MoveOneBlockUp();
	void MoveOneBlockDown();
private:
	void MoveOnePixelRight();
	void MoveOnePixelLeft();
	void MoveOnePixelUp();
	void MoveOnePixelDown();
public :
	void SetSpriteCurrentFrameImageStart(const float x, const float y);
	void SetSpriteCurrentFrameImageEnd(const float x, const float y);
private :
	std::pair<Vector2, Vector2> GetFinalFrameStartEndPos(const Vector2& FrameStart, const Vector2& FrameEnd);
};
