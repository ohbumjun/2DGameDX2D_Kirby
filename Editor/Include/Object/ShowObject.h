#pragma once

#include "GameObject\GameObject.h"

class CShowObject :
    public CGameObject
{
    friend class CScene;
    friend class CEditorManager;
protected:
    CShowObject();
    CShowObject(const CShowObject& obj);
    virtual ~CShowObject() override;
private :
    CSharedPtr<class CStaticMeshComponent> m_MeshComponent;
    Vector2 m_StartPos;
    Vector2 m_EndPos;
public :
    void SetStartPos(const Vector2& Pos);
    void SetEndPos(const Vector2& Pos);
public :
    virtual bool Init() override;
    virtual CShowObject* Clone() override;

};

