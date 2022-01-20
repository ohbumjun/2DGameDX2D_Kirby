#pragma once
#include "UI\UIWindow.h"
#include "UI/UIImage.h"

class CMouseAttack :
    public CUIWindow
{
    friend class CViewPort;
    friend class CEngine;

protected :
    CMouseAttack();
    CMouseAttack(const  CMouseAttack& Attack);
    virtual ~CMouseAttack() override;
private :
    CSharedPtr<CUIImage> m_Image;
public :
    virtual bool Init() override;

};

