#pragma once
#include "BossMonster.h"
#include "DynaElementBuilder.h"

class CBossDyna :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossDyna();
    virtual ~CBossDyna() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private :
    // BossDynaBaby 목록들을 List 형태로 들고 있게 한다.
    std::list <CSharedPtr<class CBossDynaBaby>> m_BabiesList;
    CDynaBabyBuilder* m_BabyBuilder;
    CDynaNestBuilder* m_NestBuilder;
    // CObjectManager<CBossDynaBaby>* m_BossDynaBabyManager;
public :
    std::list <CSharedPtr<class CBossDynaBaby>>* GetBossDynaBabyList()
{
        return &m_BabiesList;
}
public :
    void AddBossDynaBaby(class CBossDynaBaby* Baby);
    void DeleteDynaBaby(class CBossDynaBaby* Baby);
private:
    float m_SceneChangeLimitTime;
    CSharedPtr<class CSpriteComponent> m_DynaHead;
    CSharedPtr<class CSpriteComponent> m_DynaRightFoot;
    CSharedPtr<class CSpriteComponent> m_DynaLeftFoot;
private :
    float m_HeadToggleMaxTime;
    float m_HeadToggleTime;
    bool m_HeadToggleUp;
    float m_InitHeadYRelativePos;
private:
    // Movement
    bool m_MovementRight;
    bool m_MovementUp;
    float m_MovementTargetYPos;
    float m_MovementTargetXPos;
private :
    // Appear
    bool m_IsAppearing;
    float m_ScaleIncreasing;
    float m_AppearFlowTime;
private :
    // Hit
    float m_YPosBeforeHit;
protected :
    float m_MakeDynaNestTime;
    float m_MakeDynaNestFlowTime;
private:
    void FarAttack();
    void CloseAttack();
    void ChangeFarAttackAnimation();
    void ChangeCloseAttackAnimation();
    void UpdateSceneChangeLimitTime(float DeltaTime);
    void ChangeSceneToEndingScene();
    virtual void MakeBossAngry() override;
protected:
    void UpdateScale(float DeltaTime);
    void UpdateHeadToggle(float DeltaTime);
    void UpdateMovement(float DeltaTime);
    void UpdateAppearance(float DeltaTime);
    void UpdateMakeNestTime(float DeltaTime);
private :
    void CreateBossDynaBaby();
private :
    virtual void ChangeTraceAnimation() override;
    virtual void ChangeAttackAnimation() override;
    virtual void ChangeWalkAnimation() override;
    virtual void ChangeIdleAnimation() override;
private :
    virtual void SpecificActionAfterHitTimeEnd() override;
private:
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
    virtual void AIWalkSpecific(float DeltaTime) override;
    virtual void AIIdleSpecific(float DeltaTime) override;
    virtual void AIHitSpecific(float DeltaTime) override;
    virtual void AIDeathSpecific(float DeltaTime) override;
};

