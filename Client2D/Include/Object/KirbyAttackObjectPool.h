#pragma once
#include "GameInfo.h"
#include "KirbyAttackEffect.h"
#include <queue>

class CKirbyAttackObjectPool : public CGameObject
{
	friend class CKirbyState;
	friend class CBeamKirbyState;
	friend class CFightKirbyState;
	friend class CPlayer2D;
	friend class CScene;
	friend class CKirbyAttackEffect;
private:
	CKirbyAttackObjectPool();
	~CKirbyAttackObjectPool();
private :
	std::vector<CSharedPtr<CKirbyAttackEffect>> m_vecAttackEffects;
	std::queue<int> m_vecReadyIndex;
	bool m_ExecuteObjectPool;
	int   m_UsedObjectsNum;
	std::function<void()> m_FuncInitializePool;
	class CPlayer2D* m_PlayerOwner;
public :
	const std::vector<CSharedPtr<CKirbyAttackEffect>>& GetVecKirbyAttackEffects () const
{
		return m_vecAttackEffects;
}
public :
	void SetObjectsPoolEnable(bool Enable)
{
		m_ExecuteObjectPool = Enable;
}
private :
	CKirbyAttackEffect* GetPoolObject();
	void                ExtendPool(int NewSize);
	void				  ReFillObjectPool();
	void                SetAttackType(KirbyAttackEffect_Type Type, int EnableSize);
	void                SetAttackType(KirbyAttackEffect_Type Type);
	void				  SetInitObjectAlive(int ObjectSize);
	void				  AddAliveObject();
	void				  SetObjectTrait(KirbyAttackEffect_Type Type, int AliveIndex);
public :
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
public :
	template<typename T>
	void SetPoolInitializeFunction(T* Obj, void(T::*Func)())
{
		m_FuncInitializePool = std::bind(Func, Obj);
}
};

