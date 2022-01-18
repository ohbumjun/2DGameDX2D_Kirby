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
    float m_PlayTime;
    float m_AnimTime;
    int m_FrameIndex;
public :
    WidgetImageInfo GetWidgetImageInfo() const
{
        return m_Info;
}
public :
    void SetPlayTime(float PlayTime)
{
        m_PlayTime = PlayTime;
}
public :
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size);
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

