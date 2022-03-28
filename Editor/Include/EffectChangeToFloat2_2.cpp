#include "EffectChangeToFloat2_2.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

CEffectChangeToFloat2_2::CEffectChangeToFloat2_2()
{
	SetTypeID<CEffectChangeToFloat2_2>();
}

CEffectChangeToFloat2_2::~CEffectChangeToFloat2_2()
{}

void CEffectChangeToFloat2_2::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat2_2::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat2_2Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
	m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

	m_ColliderBody->AddChild(m_SimpleHUDWidget);

	CMonsterEditorHUD* HUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	HUD->SetText(TEXT("Float2_2"));

	return true;
}

void CEffectChangeToFloat2_2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat2_2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
