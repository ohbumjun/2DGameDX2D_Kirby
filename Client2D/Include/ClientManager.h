#pragma once

#include "GameInfo.h"
#include <random>

class CClientManager
{
private:
	std::mt19937 m_RandomEngine;
private :
	CClientManager();
	~CClientManager();
private :
	static CClientManager* m_Inst;
public :
	static CClientManager* GetInst()
{
		if (!m_Inst)
			m_Inst = new CClientManager;
		return m_Inst;
}
	static void DestroyInst()
{
		SAFE_DELETE(m_Inst);
}
	// DECLARE_SINGLE(CClientManager)
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int  Run();
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimationInstance(class CSpriteComponent* Component, const size_t AnimationTypeID);
public:
	float GenerateRandomNumberFrom0To1();
};
