#pragma once
#include "Component/ColliderPixel.h"
#include "GameObject\GameObject.h"

class CPixelTest :
    public CGameObject
{
    friend class CScene;
protected :
    CPixelTest();
    CPixelTest(const CPixelTest& Pixel);
    virtual ~CPixelTest() override;
private :
    CSharedPtr<class CColliderPixel> m_Body;
public :
    virtual bool Init() override;
    virtual CPixelTest* Clone() override;

};

