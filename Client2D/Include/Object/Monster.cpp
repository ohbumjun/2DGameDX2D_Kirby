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
	m_DeathStart(false),
	m_IsBeingPulled(false),
	m_BeginPulledAccel(1.f),
	m_BeginPulledAccelSum(0.f)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Monster) : CLifeObject(Monster)
{
	m_Sprite = (CSpriteComponent*)const_cast<CMonster&>(Monster).FindComponent("MonsterSprite");
	m_ColliderBody = (CColliderCircle*)const_cast<CMonster&>(Monster).FindComponent("ColliderBody");
}

CMonster::~CMonster()
{}

void CMonster::LoadAnimationInstance(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(Name, FileName, PathName);

	if (!AnimationInstance)
	{
		assert(false);
		return;
	}

	 // Clone 해서 세팅해줘야 한다.
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
	// 실제 데미지
	m_HP -= Damage;
	if (m_HP < 0.f)
	{
		Destroy();
		return;
	}

	// Widget HP Bar 조정하기
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
	CLifeObject::Start();

	m_Sprite = (CSpriteComponent*)FindComponent("MonsterSprite");

	// Collider 
	m_ColliderBody = (CColliderCircle*)FindComponent("ColliderBody");
	m_ColliderBody->AddCollisionMouseCallback(Collision_State::Begin, this,
		&CMonster::OnMouseBegin);
	m_ColliderBody->AddCollisionMouseCallback(Collision_State::End, this,
		&CMonster::OnMouseEnd);
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CMonster::OnCollisionBegin);

	// PaperBurn
	m_PaperBurn = (CPaperBurnComponent*)FindComponent("PaperBurn");
	m_PaperBurn->SetFinishCallback(this, &CMonster::PaperBurnEnd);
	m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());

	// UIWindow
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");
	if (!m_SimpleHUDWidget)
	{
		m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	}
	m_SimpleHUDWidget->CreateUIWindow<CSimpleHUD>("SimpleHUDWindow");
	m_Sprite->AddChild(m_SimpleHUDWidget);
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);


}

bool CMonster::Init()
{
	if (!CLifeObject::Init())
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
	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Beam", TEXT("Beam.anim"));
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
	CLifeObject::Update(DeltaTime);

	// 죽으면
	// 1) Animation 바꿔주고
	// 2) 해당 Animation의 Time 저장하고
	// 3) Animation Time을 감소시켜서 0이 되면 그때 가서 
	// 2) Animation 끝날 때까지 기다린 다음 PaperBurn


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

	UpdateBeingPulled(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::UpdateBeingPulled(float DeltaTime)
{
	if (!m_IsBeingPulled)
		return;

	Vector3 MonsterPos = GetWorldPos();
	Vector3 PulledDir = m_PulledDestPos - MonsterPos;
	PulledDir.Normalize();

	m_BeginPulledAccelSum += m_BeginPulledAccel;

	AddWorldPos(Vector3(PulledDir) * DeltaTime * m_BeginPulledAccelSum);
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

void CMonster::Save(FILE* pFile)
{
	CLifeObject::Save(pFile);

	fwrite(&m_HPMax, sizeof(float), 1, pFile);
	fwrite(&m_HP, sizeof(float), 1, pFile);
	fwrite(&m_DeathAccTime, sizeof(float), 1, pFile);
	fwrite(&m_DeathFinishTime, sizeof(float), 1, pFile);
	fwrite(&m_DeathStart, sizeof(bool), 1, pFile);
}

void CMonster::Load(FILE* pFile)
{
	CLifeObject::Load(pFile);

	fread(&m_HPMax, sizeof(float), 1, pFile);
	fread(&m_HP, sizeof(float), 1, pFile);
	fread(&m_DeathAccTime, sizeof(float), 1, pFile);
	fread(&m_DeathFinishTime, sizeof(float), 1, pFile);
	fread(&m_DeathStart, sizeof(bool), 1, pFile);
}