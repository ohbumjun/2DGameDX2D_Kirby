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
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimInputName;
private:
	class CIMGUITextInput* m_StartFramePosXInput;
	class CIMGUITextInput* m_StartFramePosYInput;
	class CIMGUITextInput* m_EndFramePosXInput;
	class CIMGUITextInput* m_EndFramePosYInput;
private:
	class CIMGUIComboBox* m_AnimationLoop;
	class CIMGUIComboBox* m_AnimationReverse;
private:
	CSharedPtr<class CSpriteEditObject> m_SpriteObject;
	class CAnimationSequence2DInstance* m_Animation;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
public:
	void LoadTextureButton();
	void SpriteEditButton();
	void DeleteSequence();
	void ClearSequence();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void DeleteFrameButton();
	void ClearFrameButton();
	void EditFrameButton();
	void PlayAnimationButton();
	void StopAnimationButton();
public :
	void SetCurrentAnimationLoop(int, const char*);
	void SetCurrentAnimationReverse(int, const char*);
public:
	void SaveSequence();
	void LoadSequence();
	void SaveAnimation();
	void LoadAnimation();
public:
	void SelectAnimationSequence(int, const char*);
	void SelectAnimationFrame(int, const char*);
};
