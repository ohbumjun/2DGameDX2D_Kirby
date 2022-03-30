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

	// Monster 에서의 Start 와 마찬가지로, WidgetComponent 의 World Pos를
	// RootComponent 의 상대적인 위치에 세팅하기 위한 코드이다
	if (m_RootComponent)
	{
		m_RootComponent->SetRelativePos(GetWorldPos());
	}

	m_SimpleHUDWidget = FindComponentByType<CWidgetComponent>();

	if (m_SimpleHUDWidget)
	{
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

void CEffectChangeToFloat7::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	if (!FindComponentByType<CWidgetComponent>())
	{
		m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
		m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(m_SimpleHUDWidget->GetWidgetWindow());

		MonsterHUD->SetText(TEXT("Float7"));

		m_RootComponent->AddChild(m_SimpleHUDWidget);
	}
}
