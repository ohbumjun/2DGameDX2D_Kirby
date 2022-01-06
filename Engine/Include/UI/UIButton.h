#pragma once

#include "UIWidget.h"

struct ButtonStateInfo {
	CSharedPtr<class CTexture> m_Texture;
	Vector4 m_Tint;
	std::vector<AnimationFrameData> m_FrameData;
	ButtonStateInfo() :
	m_Texture(nullptr),
	m_Tint(Vector4::White)
	{
	}
};

class CUIButton : public CUIWidget {
	friend class CUIWindow;
protected :
	CUIButton();
	virtual ~CUIButton() override;
private :
	Button_State m_State;
	ButtonStateInfo m_ButtonStateInfo[(int)Button_State::Max];
	std::function<void()> m_ClickCallback;
public :
	Button_State GetButtonState() const
{
		return m_State;
}
	class CTexture* GetTexture(Button_State State)
{
		return m_ButtonStateInfo[(int)State].m_Texture;
}
public :
	virtual void Enable(bool enable) override;
	void SetTintColor(Button_State State, float r, float g, float b, float a);
	void AddFrameData(Button_State State, const Vector2& Start, const Vector2& Size);
	void SetTexture(Button_State State, class CTexture* Texture);
	void SetTexture(Button_State State, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureMultibyte(Button_State State, const std::string& TextureName, const char* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureMultibyteFullPath(Button_State State, const std::string& TextureName, const char* FullPath);
	void SetTextureFullPath(Button_State State, const std::string& TextureName, const TCHAR* FullPath);
public :
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
public :
	void CallClickCallback();
	template<typename T>
	void SetClickCallback(T* Obj, void(T::*Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}
};