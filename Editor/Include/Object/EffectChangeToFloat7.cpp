#include "EffectChangeToFloat7.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

CEffectChangeToFloat7::CEffectChangeToFloat7()
{
	SetTypeID<CEffectChangeToFloat7>();
}

CEffectChangeToFloat7::~CEffectChangeToFloat7()
{}

void CEffectChangeToFloat7::Start()
{
	CGameObject::Start();


	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		m_SimpleHUDWidget = Component;

		if (!m_SimpleHUDWidget->GetWidgetWindow())
		{
			m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

			CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(m_SimpleHUDWidget->GetWidgetWindow());

			MonsterHUD->SetText(TEXT("Float7")); //
		}
	}
}

bool CEffectChangeToFloat7::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat7Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
	m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

	m_ColliderBody->AddChild(m_SimpleHUDWidget);

	CMonsterEditorHUD* HUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	HUD->SetText(TEXT("Float7"));

	return true;
}

void CEffectChangeToFloat7::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat7::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
