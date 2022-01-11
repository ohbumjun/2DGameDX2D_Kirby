#pragma once

#include "../Ref.h"

class CUIWidget :
    public CRef
{
    friend class CUIWindow;
protected :
    CUIWidget();
    virtual ~CUIWidget();
protected:
    class CUIWindow* m_Owner;
    Vector2 m_Size;
    Vector2 m_Pos;
    bool m_Start;
    int m_ZOrder;
    float m_Angle;
    Vector4 m_Tint;
    class CWidgetConstantBuffer* m_CBuffer;
    CSharedPtr<class CShader> m_Shader;
    CSharedPtr<class CMesh> m_Mesh;
    bool m_MoueHovered;
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
    int GetZOrder() const
{
        return m_ZOrder;
}
    float GetAngle() const
{
        return m_Angle;
}

public :
    void SetSize(const Vector2& Size)
{
        m_Size = Size;
}
    void SetPos(const Vector2& Pos)
    {
        m_Pos = Pos;
    }
    void SetSize(float x, float y)
    {
        m_Size = Vector2(x, y);
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
public :
    virtual bool CollisionMouse(const Vector2& MousePos);
public :
    virtual void Start();
	virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
};

