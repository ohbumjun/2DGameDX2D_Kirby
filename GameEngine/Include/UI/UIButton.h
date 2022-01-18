#pragma once

#include "UIWidget.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Sound/Sound.h"


class CUIButton : public CUIWidget {
	friend class CUIWindow;
protected :
	CUIButton();
	CUIButton(const CUIButton& Button);
	virtual ~CUIButton() override;
private :
	WidgetImageInfo m_Info[(int)Button_State::Max];
	std::function<void()> m_ClickCallback;
	Button_State m_State;
	CSharedPtr<CSound> m_ButtonSounds[Button_SoundState::BtnMax];
	bool m_HoverSound;
	bool m_ClickSound;
public :
	void SetPlayTime(Button_State State, float PlayTime)
	{
		m_Info[(int)State].m_PlayTime = PlayTime;
	}
	void SetPlayScale(Button_State State, float Scale)
	{
		m_Info[(int)State].m_PlayScale = Scale;
	}
	void SetMouseHoverSound(CSound* Sound);
	void SetMouseClickSound(CSound* Sound);
	void SetMouseSound(Button_SoundState State, const std::string& GroupName, const std::string& SoundName,
		bool Loop, const TCHAR* FileName, const std::string& PathName = SOUND_PATH);
	void SetMouseSound(Button_SoundState State, const std::string& SoundName);
	void SetMouseSound(Button_SoundState State,CSound* Sound);
public :
	void SetTextureTint(Button_State State, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool SetTexture(Button_State State, CTexture* Texture);
	bool SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(Button_State State, const std::string& Name, const TCHAR* FullPath);
	void SetButtonEnable(bool Enable);
	void AddFrameData(Button_State State, const Vector2& StartPos, const Vector2& Size);
public :
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render() override;
	virtual CUIButton* Clone() override;
public :
	template<typename T>
	void SetClickCallback(T* Obj, void (T::*Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}
};


