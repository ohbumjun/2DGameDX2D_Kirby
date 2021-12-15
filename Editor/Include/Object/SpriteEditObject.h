#pragma once

#include "GameObject/GameObject.h"

class CSpriteEditObject :
    public CGameObject
{
    friend class CScene;

protected:
    CSpriteEditObject();
    CSpriteEditObject(const CSpriteEditObject& obj);
    virtual ~CSpriteEditObject();

private:
    CSharedPtr<class CSpriteComponent>    m_Sprite;
    float   m_Distance;
    bool   m_ObjMouseDown;
    Vector2 m_ObjImageStartPos;
    Vector2 m_ObjImageEndPos;
    class CIMGUIWindow* m_EditWindow;
public:
    class CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }
    void SetEditWindow(class CIMGUIWindow* Window)
    {
        m_EditWindow = Window;
    }
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CSpriteEditObject* Clone();
    void UpdateClickInfo();
    void SetEditWindowTexture();
};

