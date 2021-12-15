#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
    public CIMGUIWindow
{
public:
	CSpriteWindow();
	virtual ~CSpriteWindow();

private:
	bool m_AnimListAdd;
	bool m_AnimFrameListAdd;
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_SpriteSampled;
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUITextInput* m_AnimInputName;
	CSharedPtr<class CSpriteEditObject> m_SpriteObject;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
public:
	void LoadTextureButton();
	void SpriteEditButton();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void AnimtionListClickUpdate();
	void AnimtionFrameListClickUpdate();
};

