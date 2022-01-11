#pragma once

#include "UIWidget.h"
#include "../Resource/Texture/Texture.h"

struct ButtonStateInfo {
	CSharedPtr<CTexture> m_Texture;
	Vector4 m_Tint;
	std::vector<AnimationFrameData> vecFrameData;
	ButtonStateInfo() :
		m_Texture(nullptr)
	{
		m_Tint = Vector4::White;
	}
};

class CUIButton : public CUIWidget {
	friend class CUIWindow;
protected :
	CUIButton();
	CUIButton(const CUIButton& Button);
	virtual ~CUIButton() override;
private :
	ButtonStateInfo m_Info[(int)Button_State::Max];
	std::function<void()> m_ClickCallback;
	Button_State m_State;
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
public :
	template<typename T>
	void SetClickCallback(T* Obj, void (T::*Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}
};


