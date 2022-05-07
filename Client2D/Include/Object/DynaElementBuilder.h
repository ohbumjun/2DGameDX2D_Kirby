#pragma once
#include "BossDynaBaby.h"
#include "DynaNest.h"

class CDynaElementBuilder {
	friend class CBossDyna;
	virtual ~CDynaElementBuilder(){}
};

class CDynaBabyBuilder
{
	friend class CBossDyna;
private :
	// BossDynaBaby 목록들을 List 형태로 들고 있게 한다.
	std::list <CSharedPtr<class CBossDynaBaby>> m_BabiesList;
	CBossDynaBaby* m_CurrentDynaBaby;
private :
	CBossDynaBaby* CreateBaby(const std::string& Name);
	CBossDynaBaby* SetWorldPos(float x, float y, float z);
	CBossDynaBaby* SetWorldScale(float x, float y, float z);
	CBossDynaBaby* SetObjectMoveDir(float Dir);
	CBossDynaBaby* SetJumpVelocity(float Velocity);


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
