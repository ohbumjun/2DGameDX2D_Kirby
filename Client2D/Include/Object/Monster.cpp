#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/PaperBurnComponent.h"
#include "Component/ColliderCircle.h"
#include "MonsterAnimation.h"
#include "Engine.h"
#include "Player2D.h"
#include "../UI/SimpleHUD.h"

CMonster::CMonster() :
	m_HPMax(5.f),
	m_HP(5.f),
	m_DeathAccTime(0.f),
	m_DeathFinishTime(0.f),
	m_DeathStart(false),
	m_IsBeingPulled(false),
	m_BeginPulledAccel(2.f),
	m_BeginPulledAccelSum(0.f),
	m_AttackDistance(150.f),
	m_DashDistance(500.f),
	m_MonsterMoveVelocity(100.f),
	m_RandomMoveTime(5.f),
	m_RandomMoveTimeMax(5.f),
	m_IsBeingSpitOut(false),
	m_SpitOutDistanceMax(500.f),
	m_IsAbilityMonster(false)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& Monster) : CLifeObject(Monster)
{
	m_Sprite = (CSpriteComponent*)const_cast<CMonster&>(Monster).FindComponent("MonsterSprite");
	m_ColliderBody = (CColliderCircle*)const_cast<CMonster&>(Monster).FindComponent("ColliderBody");
	m_SimpleHUDWidget = (CWidgetComponent*)const_cast<CMonster&>(Monster).FindComponent("SimpleHUD");
	m_PaperBurn = (CPaperBurnComponent*)const_cast<CMonster&>(Monster).FindComponent("PaperBurn");

	// todo :  CSharedPtr<class CUIProgressBar> m_HpBar; ����

	m_HPMax = Monster.m_HPMax;
	m_HP = m_HPMax;
	m_DeathAccTime = 0.f;
	m_DeathFinishTime = Monster.m_DeathFinishTime;
	m_DeathStart = false;

	// Player ���� ����������
	m_IsBeingPulled = false;
	m_BeginPulledAccel = Monster.m_BeginPulledAccel;
	m_BeginPulledAccelSum = 0.f;

	// AI
	m_AttackDistance = Monster.m_AttackDistance;
	m_DashDistance = Monster.m_DashDistance;
	m_IsBeingHit = Monster.m_IsBeingHit;

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

	 // Clone �ؼ� ��������� �Ѵ�.
	m_Sprite->SetAnimationInstance(AnimationInstance);
}

void CMonster::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Sprite || !m_Sprite->GetAnimationInstance())
		return;
	m_Sprite->GetAnimationInstance()->ChangeAnimation(Name);
}

void CMonster::ResetPulledInfo()
{
	m_BeginPulledAccelSum = 0.f;
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

void CMonster::AIStateUpdate(float DeltaTime)
{
	// ���� �������� �ִ� ���¶�� ���� X --> UpdateBeingPulled �Լ��� ����
	if (m_IsBeingPulled)
		return;
	if (m_IsBeingSpitOut)
		return;

	// Hit Ȥ�� Death�� �������ְ� Return
	// Bullet �� ��쿡��, �ű� ������ ���� �˰����� ���� �ʰ�,
	// m_HP�� 0���� ������ְ� --> �߰����� ������ ���⼭ �����ϵ��� �Ѵ�.
	if (m_HP <= 0)
	{
		m_AI = Monster_AI::Death;
		return;
	}
	else if (m_IsBeingHit)
	{
		m_AI = Monster_AI::Hit;
		return;
	}

	// Player ������ �����´�
	CPlayer2D* Player2D = (CPlayer2D*)m_Scene->GetPlayerObject();

	// ���� Collider ������ ���ϴ��� Ȯ��
	if (Player2D)
	{
		// ���� ������ ���Ѵٸ�
		float DistToPlayer = GetWorldPos().Distance(Player2D->GetWorldPos());

		if (DistToPlayer <= m_DashDistance)
		{
			if (DistToPlayer <= m_AttackDistance)
			{
				// Attack AI ����
				m_AI = Monster_AI::Attack;
				return;
			}
			else
			{
				// Trace AT ����
				m_AI = Monster_AI::Trace;
				return;
			}
		}
		// ���� ������ ������ �ʴ´ٸ� --> Walk Ȥ�� AI
		else
		{
			if (m_ObjectMoveDir.Length() == 0.f)
			{
				m_AI = Monster_AI::Idle;
			}
			else
			{
				m_AI = Monster_AI::Walk;
			}
		}
	}
	else
	{
		// todo : ���⿡ �ɸ��� �ȵȴ� ! --> ���ʿ� Scene ���� Player �� ���� ���� �����Ƿ�
		// ������, Scene Change �߰����� ���� ���� ���� ������ ?
	}

	// ������ �ʴ´ٸ�, Idle, Walk ���� ����
}

void CMonster::AIActionUpdate(float DeltaTime)
{
	CPlayer2D* Player2D = (CPlayer2D*)m_Scene->GetPlayerObject();

	if (!Player2D)
		return;

	switch(m_AI)
	{
	case Monster_AI::Idle :
		{
			AIIdle(DeltaTime);
		}
		break;
	case Monster_AI::Walk:
	{
		AIWalk(DeltaTime);
	}
	break;
	case Monster_AI::Trace:
	{
		AITrace(DeltaTime, Player2D->GetWorldPos());
	}
	break;
	case Monster_AI::Attack:
	{
		AIAttack(DeltaTime, Player2D->GetWorldPos());
	}
	break;
	case Monster_AI::Hit:
	{
		AIHit(DeltaTime);
	}
	break;
	case Monster_AI::Death:
	{
		AIDeath(DeltaTime);
	}
	break;
	}
}

void CMonster::AIIdle(float DeltaTime)
{
	ChangeIdleAnimation();
}
void CMonster::AIWalk(float DeltaTime)
{
	ChangeWalkAnimation();
}

void CMonster::AITrace(float DeltaTime, Vector3 PlayerPos)
{
	// todo : Attack Distance �ȿ� ������, �̵��� ���ƾ� �ϳ� ?

	Vector3 MonsterPos = GetWorldPos();

	Vector3 TraceDir = PlayerPos - MonsterPos;

	TraceDir.Normalize();

	// ���� Monster�� ��� --> Trace �߿��� �¿�θ� ������ �� �ֵ��� �����Ѵ�.
	if (m_IsGroundObject)
	{
		TraceDir.y = 0.f;
	}

	AddWorldPos(Vector3(TraceDir) * DeltaTime * m_MonsterMoveVelocity);

	if (TraceDir.x < 0.f)
	{
		m_ObjectMoveDir.x = -1.f;
	}
	else if (TraceDir.x > 0.f)
	{
		m_ObjectMoveDir.x = 1.f;
	}

	ChangeTraceAnimation();


	/*
	if (MonsterPos.Distance(m_PulledDestPos) <= 1.f)
	{
		m_IsBeingPulled = false;
		SetEnable(false);
	}
	*/
}

void CMonster::AIAttack(float DeltaTime, Vector3 PlayerPos)
{
	ChangeAttackAnimation();
}

void CMonster::AIDeath(float DeltaTime)
{
	ChangeDeathAnimation();
}

void CMonster::AIHit(float DeltaTime)
{
	ChangeHitAnimation();
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

	SetRandomTargetDir();

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

	UpdateBeingPulled(DeltaTime);

	UpdateBeingOutOfPlayer(DeltaTime);

	AIStateUpdate(DeltaTime);

	AIActionUpdate(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);

	UpdateMonsterMove(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::UpdateBeingPulled(float DeltaTime)
{
	if (!m_IsBeingPulled)
		return;

	if (m_IsBeingSpitOut)
		return;

	Vector3 MonsterPos = GetWorldPos();

	CPlayer2D* Player2D = (CPlayer2D*)m_Scene->GetPlayerObject();

	if (!Player2D)
		return;

	m_PulledDestPos = Player2D->GetWorldPos();

	Vector3 PulledDir = m_PulledDestPos - MonsterPos;
	PulledDir.Normalize();

	m_BeginPulledAccelSum += m_BeginPulledAccel;

	AddWorldPos(Vector3(PulledDir) * DeltaTime * m_BeginPulledAccelSum);

	if (MonsterPos.Distance(m_PulledDestPos) <= 10.f)
	{
		m_IsBeingPulled = false;

		Enable(false);

		// Destroy();

		Player2D->SetIsEatingMonster(true);
		Player2D->SetEatenMonster(this);
	}
}

void CMonster::UpdateBeingOutOfPlayer(float DeltaTime)
{
	if (!m_IsBeingSpitOut)
		return;

	if (m_SpitOutDistance < m_SpitOutDistanceMax)
	{
		m_SpitOutDistance += DeltaTime * 500.f;

		m_IsBeingSpitOut = true;

		AddWorldPos(m_ObjectMoveDir * DeltaTime * 500.f);

		if (m_SpitOutDistance >= m_SpitOutDistanceMax)
		{
			m_IsBeingSpitOut = false;
		}
	}
}

void CMonster::UpdateMonsterMove(float DeltaTime)
{
	// ���� �������̰ų�, Trace ���¶�� ���� Dir�� �״�� �����Ѵ�.
	if (m_AI == Monster_AI::Attack || m_AI == Monster_AI::Trace)
		return;

	if (m_IsBeingSpitOut || m_IsBeingPulled)
		return;

	AddWorldPos(m_ObjectMoveDir * DeltaTime * m_MonsterMoveVelocity);

	m_RandomMoveTime -= DeltaTime;

	if (m_RandomMoveTime <= 0.f)
	{
		m_RandomMoveTime = m_RandomMoveTimeMax;
		SetRandomTargetDir();
	}
}

void CMonster::SetRandomTargetDir()
{
	// ���� �������̰ų�, Trace ���¶�� ���� Dir�� �״�� �����Ѵ�.
	if (m_AI == Monster_AI::Attack || m_AI == Monster_AI::Trace)
		return;

	/*
	Vector2 WorldResolution = m_Scene->GetWorldResolution();

	float TargetX = (float)(rand() % (int)WorldResolution.x);
	float TargetY = GetWorldPos().y;
	Vector3 WorldPos = GetWorldPos();

	float Angle = GetWorldPos().Angle(Vector3(TargetX, TargetY, 1.f));
	
	m_ObjectMoveDir.x = cosf(DegreeToRadian(Angle));
	m_ObjectMoveDir.y = sinf(DegreeToRadian(Angle));
	*/

	int RandomDir = (int)(rand() % 2);
	m_ObjectMoveDir.x = RandomDir == 0 ? -1.f : 1.f;

	m_ObjectMoveDir.Normalize();

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

void CMonster::ChangeIdleAnimation()
{
	std::string CurAnimName = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (m_ObjectMoveDir.x < 0)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftIdle");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightIdle");
}

void CMonster::ChangeWalkAnimation()
{
	std::string CurAnimName = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (m_ObjectMoveDir.x < 0)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftWalk");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightWalk");
}

void CMonster::ChangeHitAnimation()
{
	std::string CurAnimName = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (m_ObjectMoveDir.x < 0)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftHit");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightHit");
}

void CMonster::ChangeTraceAnimation() 
{
	std::string CurAnimName = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (m_ObjectMoveDir.x < 0)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftRun");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightRun");
}

void CMonster::ChangeDeathAnimation() 
{
	std::string CurAnimName = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (m_ObjectMoveDir.x < 0)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftDeath");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightDeath");
}

void CMonster::ChangeAttackAnimation() 
{
	std::string CurAnimName = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (m_ObjectMoveDir.x < 0)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttack");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttack");
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