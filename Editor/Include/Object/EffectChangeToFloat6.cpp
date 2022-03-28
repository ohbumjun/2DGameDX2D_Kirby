#include "EffectChangeToFloat6.h"
#include "Component/ColliderBox2D.h"
#include "Component/SpriteComponent.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

CEffectChangeToFloat6::CEffectChangeToFloat6()
{
	SetTypeID<CEffectChangeToFloat6>();
}

CEffectChangeToFloat6::~CEffectChangeToFloat6()
{}

void CEffectChangeToFloat6::Start()
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

			MonsterHUD->SetText(TEXT("Float6")); //
		}
	}
}

bool CEffectChangeToFloat6::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat6Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
	m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

	m_ColliderBody->AddChild(m_SimpleHUDWidget);

	CMonsterEditorHUD* HUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	HUD->SetText(TEXT("Float6"));

	return true;
}

void CEffectChangeToFloat6::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat6::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
