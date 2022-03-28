#include "MushRoom.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CMushRoom::CMushRoom()
{
	SetTypeID<CMushRoom>();
}

CMushRoom::CMushRoom(const CMushRoom& Beatle) : CNormalMonster(Beatle)
{}

CMushRoom::~CMushRoom()
{}

void CMushRoom::Start()
{
	CNormalMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;
}

bool CMushRoom::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_MushRoom", TEXT("Normal_MushRoom.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("MushRoom"));
	}


	return true;
}

void CMushRoom::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CMushRoom::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CMushRoom* CMushRoom::Clone()
{
	return new CMushRoom(*this);
}
