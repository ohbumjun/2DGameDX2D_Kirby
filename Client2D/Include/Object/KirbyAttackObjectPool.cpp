#include "KirbyAttackObjectPool.h"
#include "Scene/Scene.h"
#include "../ClientManager.h"

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

// Size : ���Ӱ� ������ Size ũ��
void CKirbyAttackObjectPool::ExtendPool(int NewSize)
{
	// ���� ������ ũ�� 
	size_t  CurrentSize = m_vecAttackEffects.size();

	// ��밡���� Object ũ�� Ȯ��
	m_vecAttackEffects.resize(NewSize);

	for (size_t i = CurrentSize; i < NewSize; ++i)
	{
		// ���Ӱ� Object ����
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->Enable(false);
		AttackEffect->m_BelongToObjectPool = true;
		m_vecAttackEffects[i] = AttackEffect;

		// ��� ������ Index ��ϵ��� �־��ش�.
		m_vecReadyIndex.push(i);
	}

}

void CKirbyAttackObjectPool::ReFillObjectPool()
{
	// ��밡���� Object ũ�� Ȯ��
	size_t  CurrentSize = m_vecAttackEffects.size();

	for (size_t i = 0; i < CurrentSize; ++i)
	{
		// ��� ������ Index ��ϵ��� �־��ش�.
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

		m_vecAttackEffects[AliveIndex]->SetAttackObjectMaxLimit(1000.f);

		const float NumFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

		m_vecAttackEffects[AliveIndex]->SetWorldPos(
			m_Scene->GetPlayerObject()->GetWorldPos().x - 300.f + (600.f * NumFrom0To1),
			m_Scene->GetPlayerObject()->GetWorldPos().y + 300.f + (500 * NumFrom0To1),
			m_Scene->GetPlayerObject()->GetWorldPos().z);

		if (m_vecAttackEffects[AliveIndex]->GetWorldPos().x <= 0.f)
			m_vecAttackEffects[AliveIndex]->SetWorldPos(600.f * NumFrom0To1, m_vecAttackEffects[AliveIndex]->GetWorldPos().y, 0.f);

		m_vecAttackEffects[AliveIndex]->SetLeftAttackDir(-1.f);
		m_vecAttackEffects[AliveIndex]->SetAttackDirX(0.f);
		m_vecAttackEffects[AliveIndex]->AddRelativeRotationZ(90.f);

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

	// Attack Effect Ư�� �����ϱ�
	m_vecAttackEffects[AliveIndex]->Enable(true);

	const float NumFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

	m_vecAttackEffects[AliveIndex]->SetAttackObjectMaxLimit(1000.f);
	// m_vecAttackEffects[AliveIndex]->SetWorldPos(m_Scene->GetPlayerObject()->GetWorldPos());
	m_vecAttackEffects[AliveIndex]->SetWorldPos(
		m_Scene->GetPlayerObject()->GetWorldPos().x - 300.f + (600.f * NumFrom0To1),
		m_Scene->GetPlayerObject()->GetWorldPos().y + 300.f + (500 * NumFrom0To1),
		m_Scene->GetPlayerObject()->GetWorldPos().z);

	if (m_vecAttackEffects[AliveIndex]->GetWorldPos().x <= 0.f)
		m_vecAttackEffects[AliveIndex]->SetWorldPos(600.f * NumFrom0To1, m_vecAttackEffects[AliveIndex]->GetWorldPos().y, 0.f);

	m_vecAttackEffects[AliveIndex]->SetLeftAttackDir(-1.f);
	m_vecAttackEffects[AliveIndex]->SetAttackDirX(0.f);
	m_vecAttackEffects[AliveIndex]->AddRelativeRotationZ(90.f);

	// ��� ���� ����
	m_UsedObjectsNum += 1;
}

bool CKirbyAttackObjectPool::Init()
{
	if (!CGameObject::Init())
		return false;

	// ó������ 200���� Kirby Attack Object ���� ������.
	ExtendPool(200);
		
	return true;
}

void CKirbyAttackObjectPool::Update(float DeltaTime)
{
	// if (!m_ExecuteObjectPool)
	//	return;

	CGameObject::Update(DeltaTime);
}
