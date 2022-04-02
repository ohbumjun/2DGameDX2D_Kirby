#pragma once
#include "GameObject\GameObject.h"
#include "../Component/FightKirbyState.h"
#include "../Component/KirbyState.h"
#include "../Component/BeamKirbyState.h"
#include "../Component/FireKirbyState.h"
#include "../Component/BombKirbyState.h"
#include "../Component/SwordKirbyState.h"
#include "../Component/NormalKirbyState.h"

class CPlayerClone :
    public CGameObject
{
    friend class CPlayer2D;
public:
    CPlayerClone();
    virtual ~CPlayerClone();
private :
    CSharedPtr<CKirbyState> m_KirbyState;
    
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
};

