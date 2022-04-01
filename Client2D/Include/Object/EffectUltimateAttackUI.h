#pragma once
#include "GameObject\GameObject.h"

class CEffectUltimateAttackUI :
    public CGameObject
{
public:
    CEffectUltimateAttackUI();
    virtual ~CEffectUltimateAttackUI();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    float m_UIProceedTime;
    float m_UIProceedTimeMax;
    float m_UIProceedSpeed;
    Resolution m_RS;
    std::function<void()> m_SceneChangeCallback;
public:
    void SetUITexture(const TCHAR* FileName);
    void SetUIProceedTime(float Time)
    {
        m_UIProceedTimeMax = Time;
    }
private:
    void UpdateSceneChangeTime(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
public:
    template<typename T>
    void SetSceneChangeCallback(T* Obj, void(T::* Func)())
    {
        m_SceneChangeCallback = std::bind(Func, Obj);
    }
};

