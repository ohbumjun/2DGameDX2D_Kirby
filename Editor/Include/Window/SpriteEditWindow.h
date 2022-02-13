#pragma once

#include "IMGUIWindow.h"

class CSpriteEditWindow :
	public CIMGUIWindow
{
public:
	CSpriteEditWindow();
	virtual ~CSpriteEditWindow() override;
private :
	bool m_Reverse;
	bool m_CandyCrushVersion;
private :
	class CIMGUIText* m_IsReverseText;
	class CIMGUIText* m_IsCandyVersionText;
	class CIMGUIText* m_CameraPosX;
	class CIMGUIText* m_CameraPosY;
private :
	class CIMGUIText* m_FrameStartX;
	class CIMGUIText* m_FrameStartY;
	class CIMGUIText* m_FrameEndX;
	class CIMGUIText* m_FrameEndY;
private:
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_SpriteSampled;
	class CIMGUIImage* m_SpriteCurrentFrame;
private :
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimInputName;
	class CIMGUITextInput* m_RevCopyTargetInputName;
	class CIMGUITextInput* m_NewSequenceName;
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
	bool IsReverseMode() const
{
		return m_Reverse;
}
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
public :
	void SetCameraPosText(float PosX, float PosY);
	void SetFrameStartText(float PosX, float PosY);
	void SetFrameEndText(float PosX, float PosY);
public :
	void SetReverseMode();
	void SetNormalMode();
public :
	void SetCandyCrushMode();
	void UndoCandyCrushMode();
public:
	void EditSequenceName();
	void LoadTextureButton();
	void SpriteEditButton();
	void RevCopySequenceButton();
public:
	void DeleteAnimationSequence();
	void ClearAnimationSequence();
public :
	void AddAnimationButton();
	void AddAnimationFrameButton();
public :
	void DeleteFrameButton();
	void ClearFrameButton();
	void EditFrameButton();
public :
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
private:
	void EnlargeOnePixelRight();
	void ReduceOnePixelLeft();
	void ReduceOnePixelUp();
	void EnlargeOnePixelDown();
public :
	void SetSpriteCurrentFrameImageStart(const float x, const float y);
	void SetSpriteCurrentFrameImageEnd(const float x, const float y);
private :
	std::pair<Vector2, Vector2> GetFinalFrameStartEndPos(const Vector2& FrameStart, const Vector2& FrameEnd);
};
