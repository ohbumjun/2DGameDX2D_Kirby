#pragma once

#include "UIWidget.h"

struct ButtonStateInfo {
	CSharedPtr<class CTexture> m_Texture;
	Vector4 m_Tint;
	std::vector<AnimationFrameData> m_vecAnimFrameData;
	ButtonStateInfo() :
		m_Texture(nullptr),
		m_Tint(Vector4::White)
	{
		m_vecAnimFrameData.reserve(5);
	}
};

class CUIButton : public CUIWidget {
public :
	CUIButton();
	virtual ~CUIButton() override;
public :
	Button_State m_State;
	ButtonStateInfo m_ButtonStateInfo[Button_State::Max];
	std::function<void()> m_ClickCallbackFunction;
public :
	Button_State GetButtonState() const
{
		return m_State;
}
public :
	void SetButtonState(Button_State State)
{
		m_State = State;
}
	virtual void Enable(bool Enable) override
{
		m_State = Enable ? Button_State::Normal : Button_State::Disable;
}
public :
	void AddFrameData(Button_State State, const Vector2& Start, const Vector2& Size);
	void SetTintColor(Button_State State, const Vector4& Color);
	void SetTintColor(Button_State State, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(Button_State State, const std::string& Name, const TCHAR* FullPath);
	void SetTexture(Button_State State, class CTexture* Texture);
public :
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render() override;
public :
	void CallClickCallbackFunction();
	template<typename T>
	void SetClickCallbackFunction(T* Obj, void(T::*Func)())
{
		m_ClickCallbackFunction = std::bind(Func, Obj);
}
};