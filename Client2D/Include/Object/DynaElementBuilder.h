#pragma once
#include "BossDynaBaby.h"
#include "DynaNest.h"
#include "Pool/SimplePoolAllocator.h"
#include "GameObject/GameObjectManager.h"

class CDynaElementBuilder {
	friend class CBossDyna;
	virtual ~CDynaElementBuilder(){}
};

class CDynaBabyBuilder : public CGameObjectManager
{
	friend class CBossDyna;
private :
	// BossDynaBaby ��ϵ��� List ���·� ��� �ְ� �Ѵ�.
	std::list <CSharedPtr<class CBossDynaBaby>> m_BabiesList;
	CBossDynaBaby* m_CurrentDynaBaby;
	CSimplePoolAllocator<CBossDynaBaby>* m_BossDynaBabyPool;
private :
	CDynaBabyBuilder();
	~CDynaBabyBuilder();
private :
	CBossDynaBaby* CreateBaby(const std::string& Name);
	CDynaBabyBuilder* SetWorldPos(float x, float y, float z);
	CDynaBabyBuilder* SetWorldScale(float x, float y, float z);
	CDynaBabyBuilder* SetObjectMoveDir(float Dir);
	CDynaBabyBuilder* SetJumpVelocity(float Velocity);
private:
	void DeleteDynaBaby(CBossDynaBaby* Baby);
	void DestroyAllBabies();
	void DeAllocate(CRef* m_CurrentDynaBaby);
private :
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render();
};

class CDynaNestBuilder : public CGameObjectManager {

	friend class CBossDyna;
	friend class CDynaBabyBuilder;
private :
	CDynaNest* m_Nest;
public :
	CDynaNest* GetNest() const
{
		return m_Nest;
}
private :
	CDynaNest* CreateNest();
	CDynaNest* SetWorldPos(float x, float y, float z);
};
