#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
    public CIMGUIWindow
{
public:
	CSpriteWindow();
	virtual ~CSpriteWindow();

private:
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_SpriteSampled;
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimInputName;
	class CIMGUITextInput* m_StartFramePosXInput;
	class CIMGUITextInput* m_StartFramePosYInput;
	class CIMGUITextInput* m_EndFramePosXInput;
	class CIMGUITextInput* m_EndFramePosYInput;
	CSharedPtr<class CSpriteEditObject> m_SpriteObject;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
public:
	void LoadTextureButton();
	void SpriteEditButton();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void DeleteFrameButton();
	void ClearFrameButton();
	void EditFrameButton();
public :
	void SelectAnimationSequence(int , const char*);
	void SelectAnimationFrame(int, const char*);
};

