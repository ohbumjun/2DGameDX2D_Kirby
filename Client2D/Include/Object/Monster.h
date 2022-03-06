#pragma once
#include "GameObject\LifeObject.h"

class CMonster :
    public CLifeObject
{
public :
    CMonster();
    CMonster(const CMonster& Monster);
    virtual ~CMonster();
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
    CSharedPtr<class CUIProgressBar> m_HpBar;
    CSharedPtr<class CPaperBurnComponent> m_PaperBurn;
    // CSharedPtr<class CColliderComponent> m_ColliderBody;
    float m_HPMax;
    float m_HP;
    float m_DeathAccTime;
    float m_DeathFinishTime;
    bool m_DeathStart;

    // Player 에게 끌어당겨지기
    bool m_IsBeingPulled;
    float m_BeginPulledAccel;
    float m_BeginPulledAccelSum;
    Vector3 m_PulledDestPos;
public :
    bool IsBeingPulled() const
{
        return m_IsBeingPulled;
}
public :
    void SetPulledDestPos(Vector3 Pos)
{
        m_PulledDestPos = Pos;
}
    void SetIsBeingPulled (bool Enable)
{
        m_IsBeingPulled = Enable;
}
public :
    void LoadAnimationInstance(const std::string& Name, const TCHAR* FileName, const std::string& PathName = ENGINE_ANIMATION_PATH);
    void SetCurrentAnimation(const std::string& Name);
public :
    void Damage(float Damage);
    void SetHPMax(float HPMax);
    void DeathStart();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
    virtual void PostUpdate(float DeltaTime)override;
    virtual CMonster* Clone() override;
public :
    void UpdateBeingPulled(float DeltaTime);
public :
    void OnMouseBegin(const CollisionResult& Result);
    void OnMouseEnd(const CollisionResult& Result);
    void CreateDamageFont(const CollisionResult& Result);
    void OnCollisionBegin(const CollisionResult& Result);
private :
    void PaperBurnEnd();
public :
    virtual void Save(FILE* pFile)override;
    virtual void Load(FILE* pFile)override;
};


