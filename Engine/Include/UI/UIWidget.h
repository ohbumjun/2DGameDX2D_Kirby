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
    class CScene* m_Scene;
    Vector2 m_Size;
    Vector2 m_Pos;
public :
    void SetSize(const Vector2& Size)
{
        m_Size = Size;
}
    void SetPos(const Vector2& Pos)
    {
        m_Pos = Pos;
    }
    void SetOwner(class CUIWindow* Owner)
{
        m_Owner = Owner;
}
    void SetScene(class CScene* Scene)
{
        m_Scene = Scene;
}
public :
    virtual void Start();
	virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
};

