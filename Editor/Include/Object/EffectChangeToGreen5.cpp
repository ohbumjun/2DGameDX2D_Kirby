#include "EffectChangeToGreen5.h"
#include "Component/ColliderBox2D.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

CEffectChangeToGreen5::CEffectChangeToGreen5()
{
	SetTypeID<CEffectChangeToGreen5>();
}

CEffectChangeToGreen5::CEffectChangeToGreen5(const CEffectChangeToGreen5& Beatle) : CGameObject(Beatle)
{}

CEffectChangeToGreen5::~CEffectChangeToGreen5()
{}

void CEffectChangeToGreen5::Start()
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

			MonsterHUD->SetText(TEXT("Green5")); //
		}
	}
}

bool CEffectChangeToGreen5::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen5Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
	m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

	m_ColliderBody->AddChild(m_SimpleHUDWidget);

	CMonsterEditorHUD* HUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	HUD->SetText(TEXT("Green5"));


	return true;
}

void CEffectChangeToGreen5::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen5::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectChangeToGreen5* CEffectChangeToGreen5::Clone()
{
	return new CEffectChangeToGreen5(*this);
}

void CEffectChangeToGreen5::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	if (!FindComponentByType<CWidgetComponent>())
	{
		m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("MonsterHUD");
		m_SimpleHUDWidget->CreateUIWindow<CMonsterEditorHUD>("SimpleHUDWindow");

		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(m_SimpleHUDWidget->GetWidgetWindow());

		MonsterHUD->SetText(TEXT("Green5"));

		m_RootComponent->AddChild(m_SimpleHUDWidget);
	}
}