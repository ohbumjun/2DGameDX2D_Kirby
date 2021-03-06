#pragma once
#include "UIWidget.h"
class CUIImage :
    public CUIWidget
{
    friend class CUIWindow;
protected :
    CUIImage();
    CUIImage(const CUIImage& Widget);
    virtual ~CUIImage() override;
protected :
    WidgetImageInfo m_Info;
    std::function<void()> m_ClickCallback;

public :
    WidgetImageInfo GetWidgetImageInfo() const
{
        return m_Info;
}
    CTexture* GetTexture() const
{
        return m_Info.m_Texture;
}
public :
    void SetPlayTime(float PlayTime)
{
        m_Info.m_PlayTime = PlayTime;
}
    void SetPlayScale(float Scale)
{
        m_Info.m_PlayScale = Scale;
}

public :
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetAnimationFrameData(const char* AnimName, const TCHAR* AnimFileName, const std::string& PathName = ENGINE_ANIMATION_PATH);
    void AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size);
    void AddAnimationFrameData(int Count);
    void SetTextureTint(const Vector4& Color);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
public :
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CUIImage* Clone() override;
public :
    template<typename T>
    void SetClickCallback(T* Obj, void(T::*Func)())
{
        m_ClickCallback = std::bind(Func, Obj);
}

};

