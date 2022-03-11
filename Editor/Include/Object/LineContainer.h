#pragma once

#include "GameObject\GameObject.h"

class CLineContainer :
    public CGameObject
{
	friend class CScene;
protected:
	CLineContainer();
	CLineContainer(const CLineContainer& Obj);
	virtual ~CLineContainer() override;
};

