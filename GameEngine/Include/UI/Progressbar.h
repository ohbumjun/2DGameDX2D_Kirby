#pragma once
#include "UIWidget.h"
#include "../Resource/Shader/ProgressbarConstantBuffer.h"

class CProgressbar :
    public CUIWidget
{
    friend class CUIWindow;
protected: 
    CProgressbar();
    CProgressbar(const CProgressbar& Bar);
    virtual ~CProgressbar() override;
protected :
    CProgressbarConstantBuffer* m_ProgressBarCBuffer;
    WidgetImageInfo m_Info;
    std::function<void()> m_MouseOnCallback;
    float m_Percent;
    bool m_StartMouseOn;
    ProgressBar_Dir m_Dir;
public :
    float GetPercent() const
{
        return m_Percent;
}
public :
    void SetPercent(float Percent)
{
        m_Percent = Percent;

        if (m_Percent > 1.f)
            m_Percent = 1.f;
        else if (m_Percent < 0.f)
            m_Percent = 0.f;

        m_ProgressBarCBuffer->SetPercent(m_Percent);
}
    void SetDir(ProgressBar_Dir Dir)
{
        m_Dir = Dir;
        m_ProgressBarCBuffer->SetDir(Dir);
}
    void SetTexture(CTexture* Texture)
{
        m_Info.m_Texture = Texture;
        if (m_Info.m_Texture)
            SetUseTexture(true);
}
public :
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    void AddAnimationFrameData(const Vector2& Start, const Vector2& Size);
    void SetTextureTint(const Vector4& Tint);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CProgressbar* Clone();
public :
    template<typename T>
    void SetMouseOnCallback(T* Obj, void(T::*Func)())
{
        m_MouseOnCallback = std::bind(Func, Obj);
}
};

