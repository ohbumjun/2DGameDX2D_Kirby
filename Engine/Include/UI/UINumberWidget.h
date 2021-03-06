#pragma once
#include "UIWidget.h"

class CUINumberWidget :
    public CUIWidget
{
    friend class CUIWindow;
protected:
    CUINumberWidget();
    CUINumberWidget(const CUINumberWidget& Widget);
    virtual ~CUINumberWidget() override;
private :
    WidgetImageInfo m_Info;
    int m_Number;
    std::vector<int> m_vecNumbers;
public :
    void SetNumber(int Number)
{
        m_Number = Number;
}
    
public :
    void SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTexture(const std::string& Name,
        const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    void AddAnimationFrameData(const Vector2& StartPos, const Vector2& Size);
    void AddAnimationFrameData(int Count);
    void SetTextureTint(const Vector4& Color);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
    virtual void Render() override;
    virtual CUINumberWidget* Clone() override;
};

