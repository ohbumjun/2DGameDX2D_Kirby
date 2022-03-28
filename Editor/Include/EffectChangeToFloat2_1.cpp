#include "EffectChangeToFloat2_1.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

CEffectChangeToFloat2_1::CEffectChangeToFloat2_1()
{
	SetTypeID<CEffectChangeToFloat2_1>();
}

CEffectChangeToFloat2_1::~CEffectChangeToFloat2_1()
{}

void CEffectChangeToFloat2_1::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat2_1::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat2_1Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
	m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

	m_ColliderBody->AddChild(m_SimpleHUDWidget);

	CMonsterEditorHUD* HUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	HUD->SetText(TEXT("Float2_1"));

	return true;
}

void CEffectChangeToFloat2_1::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectChangeToFloat2_1::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
