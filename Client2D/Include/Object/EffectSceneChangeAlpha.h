#pragma once
#include "Component/SpriteComponent.h"
#include "GameObject\GameObject.h"
#include "../Client.h"

class CEffectSceneChangeAlpha :
    public CGameObject
{
public:
    CEffectSceneChangeAlpha();
    virtual ~CEffectSceneChangeAlpha();
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    bool m_SceneStart;
    float m_SceneChangeTime;
    float m_SceneChangeTimeMax;
    std::function<void()> m_SceneChangeCallback;
    bool m_MaintainOpacity;
private :
    bool m_ApplyDecreaseDestroy;
    bool m_StartOpacityDecrease;
    float m_StartDestroyTime;
    float m_StartDestroyCurTime;
public :
    void SetMaintainOpacity(bool Enable)
{
        m_MaintainOpacity = Enable;
}
    void SetSceneStart(bool Enable)
{
        m_SceneStart = Enable;

    if (Enable)
    {
        m_Sprite->SetOpacity(1.f);
    }
    else
    {
        m_Sprite->SetOpacity(0.f);
    }
}
    void SetOpacity(float Opacity)
    {
        m_Sprite->SetOpacity(Opacity);
    }
    // 해당 Time 이후, Opacity가 점점 감소하다가 마지막에 사라지기 시작한다.
    void SetStartDestroyTime(float Time)
{
        m_StartDestroyTime = Time;
}
    void SetApplyDecreaseDestroy(bool Enable)
{
        m_ApplyDecreaseDestroy = Enable;
}
    void SetBlackTexture();
    void SetUltimateAttackTexture(Ability_State State);
private :
    void UpdateSceneChangeTime(float DeltaTime);
    void UpdateOpacityDestroy(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
public :
    template<typename T>
    void SetSceneChangeCallback(T* Obj, void(T::*Func)())
{
        m_SceneChangeCallback = std::bind(Func, Obj);
}
};

