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

};

class CDynaNestBuilder {

	friend class CBossDyna;
private :
	CDynaNest* m_Nest;

	CDynaNest* CreateNest();
	CDynaNest* SetWorldPos(float x, float y, float z);

};
