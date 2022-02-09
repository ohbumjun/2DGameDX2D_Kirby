#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderCircle.h"
#include "MonsterAnimation.h"
#include "Engine.h"
#include "Component/PaperBurnComponent.h"
#include "../UI/SimpleHUD.h"

CMonster::CMonster() :
	m_HPMax(5.f),
	m_HP(5.f),
	m_DeathAccTime(0.f),
	m_DeathFinishTime(0.f),
	m_DeathStart(false)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Monster)
{
	m_Sprite = (CSpriteComponent*)const_cast<CMonster&>(Monster).FindComponent("MonsterSprite");
	m_ColliderBody = (CColliderCircle*)const_cast<CMonster&>(Monster).FindComponent("ColliderBody");
}

CMonster::~CMonster()
{}

void CMonster::SetAnimationInstance(const TCHAR* FileName)
{
	CAnimationSequence2DInstance* AnimationInstance = CResourceManager::GetInst()->LoadAnimationSequence2DInstance(FileName);
	m_Sprite->SetAnimationInstance(AnimationInstance);
}

void CMonster::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Sprite || !m_Sprite->GetAnimationInstance())
		return;
	m_Sprite->GetAnimationInstance()->ChangeAnimation(Name);
}

void CMonster::Damage(float Damage)
{
	// ���� ������
	m_HP -= Damage;
	if (m_HP < 0.f)
	{
		Destroy();
		return;
	}

	// Widget HP Bar �����ϱ�
	CSimpleHUD* HUD = dynamic_cast<CSimpleHUD*>(m_SimpleHUDWidget->GetWidgetWindow());
	HUD->GetProgressBar()->SetPercent(m_HP / m_HPMax);
}

void CMonster::SetHPMax(float HPMax)
{
	m_HPMax = HPMax;
	m_HP = HPMax;
}

void CMonster::DeathStart()
{
	m_DeathStart = true;
}

void CMonster::Start()
{
	CGameObject::Start();

	m_PaperBurn->SetFinishCallback(this, &CMonster::PaperBurnEnd);
}

bool CMonster::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("MonsterSprite");
	SetRootComponent(m_Sprite);

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("Monster");
	m_ColliderBody->AddCollisionMouseCallback(Collision_State::Begin, this,
		&CMonster::OnMouseBegin);
	m_ColliderBody->AddCollisionMouseCallback(Collision_State::End, this,
		&CMonster::OnMouseEnd);
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CMonster::OnCollisionBegin);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	m_SimpleHUDWidget->CreateUIWindow<CSimpleHUD>("SimpleHUDWindow");

	// m_HpBar = SimpleHUDWindow->CreateUIWidget<CUIProgressBar>("HPBar");
	// m_HpBar->SetPos(0.f, -50.f);

	m_PaperBurn = CreateComponent<CPaperBurnComponent>("PaperBurn");
	m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());

	m_Sprite->AddChild(m_ColliderBody);
	m_Sprite->SetTransparency(true);

	// Animation 
	CAnimationSequence2DInstance* AnimationInstance = CResourceManager::GetInst()->LoadAnimationSequence2DInstance(TEXT("Beam.anim"));
	m_Sprite->SetAnimationInstance(AnimationInstance);
	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	m_Sprite->AddChild(m_SimpleHUDWidget);
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// ������
	// 1) Animation �ٲ��ְ�
	// 2) �ش� Animation�� Time �����ϰ�
	// 3) Animation Time�� ���ҽ��Ѽ� 0�� �Ǹ� �׶� ���� 
	// 2) Animation ���� ������ ��ٸ� ���� PaperBurn


	if (m_DeathStart)
	{
		if (m_DeathFinishTime > 0.f)
		{
			m_DeathFinishTime -= DeltaTime;
			if (m_DeathFinishTime < 0.f)
			{
				m_PaperBurn->StartPaperBurn();
				m_DeathStart = false;
			}
		}
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::OnMouseBegin(const CollisionResult& Result)
{
	CEngine::GetInst()->SetMouseState(Mouse_State::State1);
}

void CMonster::OnMouseEnd(const CollisionResult& Result)
{
	CEngine::GetInst()->SetMouseState(Mouse_State::Normal);
}

void CMonster::CreateDamageFont(const CollisionResult& Result)
{}

void CMonster::OnCollisionBegin(const CollisionResult& Result)
{
  	--m_HP;
	if (m_HP <= 0)
	{
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightDeath");
		m_DeathFinishTime = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetPlayTime();
		m_ColliderBody->Enable(false);
		DeathStart();
	}
}

void CMonster::PaperBurnEnd()
{
	Destroy();
}
