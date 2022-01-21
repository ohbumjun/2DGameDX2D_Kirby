#pragma once

#include "../Ref.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"

struct WidgetImageInfo {
    CSharedPtr<CTexture> m_Texture;
    Vector4 m_Tint;
    std::vector<AnimationFrameData> m_vecFrameData;
    float m_PlayTime;
    float m_AnimTime;
    int m_FrameIndex;
    float m_PlayScale;

    WidgetImageInfo() :
        m_FrameIndex(0),
	    m_AnimTime(0.f),
	    m_PlayTime(1.f),
        m_PlayScale(1.f),
		m_Tint(Vector4::White)
    {
    }
};

class CUIWidget :
    public CRef
{
    friend class CUIWindow;
protected :
    CUIWidget();
    CUIWidget(const CUIWidget& Widget);
    virtual ~CUIWidget();
protected:
    class CUIWindow* m_Owner;
    Vector2 m_Size;
    Vector2 m_Pos;
    Vector2 m_RenderPos;
    bool m_Start;
    int m_ZOrder;
    float m_Angle;
    Vector4 m_Tint;
    float m_Opacity;
    class CWidgetConstantBuffer* m_CBuffer;
    CSharedPtr<class CShader> m_Shader;
    CSharedPtr<class CMesh> m_Mesh;
    bool m_MouseHovered;
    bool m_CollisionMouseEnable;
public :
    virtual void Enable(bool bEnable)
{
        CRef::Enable(bEnable);
}
    CUIWindow* GetOwner() const
{
        return m_Owner;
}
    Vector2 GetWindowPos() const
{
        return m_Pos;
}
    Vector2 GetWindowSize() const
    {
        return m_Size;
    }
    void SetCollisionMouseEnable(bool Enable)
    {
        m_CollisionMouseEnable = Enable;
    }
    int GetZOrder() const
{
        return m_ZOrder;
}
    float GetAngle() const
{
        return m_Angle;
}

public :
    void SetOpacity(float Opacity)
{
        m_Opacity = Opacity;
}
    virtual void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
    virtual void SetSize(float x, float y)
    {
        m_Size = Vector2(x, y);
    }
    void SetPos(const Vector2& Pos)
    {
        m_Pos = Pos;
    }
    void SetPos(float x, float y)
    {
        m_Pos = Vector2(x, y);
    }
    void SetOwner(class CUIWindow* Owner)
{
        m_Owner = Owner;
}
    void SetZOrder(int ZOrder)
{
        m_ZOrder = ZOrder;
}
    void SetAngle(float Angle)
{
        m_Angle = Angle;
}
    void SetTint(const Vector4& Tint)
{
        m_Tint = Tint;
}
    void SetShader(const std::string& Name);
    void SetUseTexture(bool Use);
    void SetUseAnimation(bool Use);
public :
    virtual bool CollisionMouse(const Vector2& MousePos);
public :
    virtual void Start();
	virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUIWidget* Clone();
};

