#pragma once
#include "GameObject\GameObject.h"
class CPlayer2D :
    public CGameObject
{
	friend class CScene;

public :
	CPlayer2D();
	CPlayer2D(const CPlayer2D& Obj);
	virtual ~CPlayer2D() override;
private :
	CSharedPtr<class CSpriteComponent> m_Sprite;
public :
	virtual void Start();
	bool virtual Init() override;;
	void virtual Update(float DeltaTime) override;;
	void virtual PostUpdate(float DeltaTime) override;
	virtual CPlayer2D* Clone();
public :
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
public :
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void AddRotationZ(float DeltaTime);
	void AddRotationZInv(float DeltaTime);
};

