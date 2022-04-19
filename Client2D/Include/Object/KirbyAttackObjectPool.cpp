#include "KirbyAttackObjectPool.h"
#include "Scene/Scene.h"
#include "Player2D.h"
#include "../ClientManager.h"
#include "Engine.h"

CKirbyAttackObjectPool::CKirbyAttackObjectPool()  :
	m_ExecuteObjectPool(false),
	m_UsedObjectsNum(0)
{}

CKirbyAttackObjectPool::~CKirbyAttackObjectPool()
{
}

CKirbyAttackEffect* CKirbyAttackObjectPool::GetPoolObject()
{
	if (m_vecReadyIndex.empty())
		ExtendPool((int)m_vecAttackEffects.size() * 2);

	int NextIndex = m_vecReadyIndex.front();

	m_vecReadyIndex.pop();

	CKirbyAttackEffect* AttackEffect = m_vecAttackEffects[NextIndex];

	AttackEffect->Enable(true);

	return m_vecAttackEffects[NextIndex];
}

// Size : 새롭게 세팅할 Size 크기
void CKirbyAttackObjectPool::ExtendPool(int NewSize)
{
	// 현재 기존의 크기 
	size_t  CurrentSize = m_vecAttackEffects.size();

	if (CurrentSize >= NewSize)
		return;

	// 사용가능한 Object 크기 확장
	m_vecAttackEffects.resize(NewSize);

	for (size_t i = CurrentSize; i < NewSize; ++i)
	{
		// 새롭게 Object 생성
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->Enable(false);
		AttackEffect->m_BelongToObjectPool = true;
		m_vecAttackEffects[i] = AttackEffect;

		// 사용 가능한 Index 목록들을 넣어준다.
		m_vecReadyIndex.push(i);
	}

}

void CKirbyAttackObjectPool::ReFillObjectPool()
{
	// 사용가능한 Object 크기 확장
	size_t  CurrentSize = m_vecAttackEffects.size();

	for (size_t i = 0; i < CurrentSize; ++i)
	{
		// 사용 가능한 Index 목록들을 넣어준다.
		m_vecReadyIndex.push(i);
	}
}

void CKirbyAttackObjectPool::SetAttackType(KirbyAttackEffect_Type Type, int EnableSize)
{
	if (EnableSize >= m_vecAttackEffects.size())
		ExtendPool(EnableSize);
		;
	for (int i = 0; i < EnableSize; ++i)
	{
		m_vecAttackEffects[i]->SetAttackType(Type);
	}
}

void CKirbyAttackObjectPool::SetAttackType(KirbyAttackEffect_Type Type)
{
	size_t VecSize = m_vecAttackEffects.size();

	for (int i = 0; i < VecSize; ++i)
	{
		m_vecAttackEffects[i]->SetAttackType(Type);
	}
}

void CKirbyAttackObjectPool::SetInitObjectAlive(int ObjectSize)
{
	if (ObjectSize >= (int)m_vecAttackEffects.size())
		ObjectSize = (int)m_vecAttackEffects.size();

	if (m_vecReadyIndex.empty())
	{
		ReFillObjectPool();

		if (m_FuncInitializePool)
			m_FuncInitializePool();
	}

	m_UsedObjectsNum = ObjectSize;

	for (int i = 0; i < ObjectSize; ++i)
	{
		int AliveIndex = m_vecReadyIndex.front();

		m_vecReadyIndex.pop();

		m_vecAttackEffects[AliveIndex]->Enable(true);

		SetObjectTrait(m_vecAttackEffects[AliveIndex]->GetAttackType(), AliveIndex);
	}
}

void CKirbyAttackObjectPool::AddAliveObject()
{
	if (m_vecReadyIndex.empty())
	{
		m_UsedObjectsNum = 0;

		m_ExecuteObjectPool = false;

		return;
	}

	int AliveIndex = m_vecReadyIndex.front();

	m_vecReadyIndex.pop();

	// Attack Effect 특성 세팅하기
	m_vecAttackEffects[AliveIndex]->Enable(true);

	SetObjectTrait(m_vecAttackEffects[AliveIndex]->GetAttackType(), AliveIndex);

	// 사용 개수 증가
	m_UsedObjectsNum += 1;
}

void CKirbyAttackObjectPool::SetObjectTrait(KirbyAttackEffect_Type Type, int AliveIndex)
{
	// Beam Kirby
	if (Type == KirbyAttackEffect_Type::BeamSpecial)
	{
		m_vecAttackEffects[AliveIndex]->SetAttackObjectMaxLimit(1000.f);

		const float NumFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

		m_vecAttackEffects[AliveIndex]->SetWorldPos(
			m_Scene->GetPlayerObject()->GetWorldPos().x - 400.f + (800.f * NumFrom0To1),
			m_Scene->GetPlayerObject()->GetWorldPos().y - 200.f + (900.f * NumFrom0To1),
			m_Scene->GetPlayerObject()->GetWorldPos().z);

		if (m_vecAttackEffects[AliveIndex]->GetWorldPos().x <= 0.f)
			m_vecAttackEffects[AliveIndex]->SetWorldPos(600.f * NumFrom0To1, m_vecAttackEffects[AliveIndex]->GetWorldPos().y, 0.f);

		m_vecAttackEffects[AliveIndex]->SetLeftAttackDir(-1.f);
		m_vecAttackEffects[AliveIndex]->SetAttackDirX(0.f);

		if (m_PlayerOwner->GetObjectMoveDir().x < 0.f)
			m_vecAttackEffects[AliveIndex]->AddRelativeRotationZ(90.f);
		else
			m_vecAttackEffects[AliveIndex]->AddRelativeRotationZ(-90.f);

	}
	// Fight Kirby
	else
	{
		const float NumFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

		m_vecAttackEffects[AliveIndex]->SetWorldPos(m_Scene->GetPlayerObject()->GetWorldPos().x - 600.f + (NumFrom0To1 * 1200.f),
			m_Scene->GetPlayerObject()->GetWorldPos().y - 300.f + (1000.f * NumFrom0To1),
			0.f);

		m_vecAttackEffects[AliveIndex]->SetAttackObjectSpeed(1800.f);

		m_vecAttackEffects[AliveIndex]->SetAttackObjectMaxLimit(1000.f);

		m_vecAttackEffects[AliveIndex]->SetBottomCollisionEnable(false);

		m_vecAttackEffects[AliveIndex]->SetSideCollisionEnable(false);

		// 왼쪽을 보고 있었다면
		if (m_PlayerOwner->GetObjectMoveDir().x < 0.f)
			m_vecAttackEffects[AliveIndex]->SetLeftAttackDir(-1.f);
		// 오른쪽으로 보고 있었다면
		else
			m_vecAttackEffects[AliveIndex]->SetRightAttackDir(-1.f);

		m_vecAttackEffects[AliveIndex]->SetAttackDirX(0.f);
	}
}

bool CKirbyAttackObjectPool::Init()
{
	if (!CGameObject::Init())
		return false;

	// 처음에는 200개의 Kirby Attack Object 들을 만들어낸다.
	ExtendPool(100);
		
	return true;
}

void CKirbyAttackObjectPool::Update(float DeltaTime)
{
	// if (!m_ExecuteObjectPool)
	//	return;

	CGameObject::Update(DeltaTime);
}
