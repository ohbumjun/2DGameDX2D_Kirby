#pragma once
#include "UIWidget.h"

class CUIDamageFont :
    public CUIWidget
{
    friend class CUIWindow;
protected:
    CUIDamageFont();
    CUIDamageFont(const CUIDamageFont& Font);
    virtual ~CUIDamageFont() override;
private :
    WidgetImageInfo m_Info;
    int m_DamageNumber;
    float m_LifeTime;
    std::vector<int> m_vecNumber;
public :
    void SetLifeTime(float LifeTime)
{
        m_LifeTime = LifeTime;
}
    void SetDamage(int Damage)
{
        m_DamageNumber = Damage;
}
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureTint(const Vector4& Color);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& StartPos, const Vector2& Size);
    void AddFrameData(int Count);
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Render() override;
};

