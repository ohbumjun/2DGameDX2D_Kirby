#pragma once
#include "BossDynaBaby.h"
#include "DynaNest.h"
#include "Pool/SimplePoolAllocator.h"
#include "GameObject/GameObjectManager.h"


class CDynaBabyBuilder : public CGameObjectManager
{
	friend class CBossDyna;
	friend class CScene;
private :
	// BossDynaBaby 목록들을 List 형태로 들고 있게 한다.
	// std::list <CSharedPtr<class CBossDynaBaby>> m_BabiesList;
	CBossDynaBaby* m_CurrentDynaBaby;
	CSimplePoolAllocator<CBossDynaBaby>* m_BossDynaBabyPool;
private :
	CDynaBabyBuilder();
	virtual ~CDynaBabyBuilder() override;
private :
	CBossDynaBaby* CreateBaby(const std::string& Name);
	CDynaBabyBuilder* SetWorldPos(float x, float y, float z);
	CDynaBabyBuilder* SetWorldScale(float x, float y, float z);
	CDynaBabyBuilder* SetObjectMoveDir(float Dir);
	CDynaBabyBuilder* SetJumpVelocity(float Velocity);
private:
	void DeleteDynaBaby(CBossDynaBaby* Baby);
	void DestroyAllBabies();
	virtual void DeAllocate(CGameObject* m_CurrentDynaBaby) ;
private :
	void Render();
};

class CDynaNestBuilder {

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
