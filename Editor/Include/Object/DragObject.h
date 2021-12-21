#pragma once

#include "GameObject/GameObject.h"

class CDragObject :
	public CGameObject
{
	friend class CScene;

protected:
	CDragObject();
	CDragObject(const CDragObject& Obj);
	virtual ~CDragObject() override;
public :
	CSharedPtr<class CStaticMeshComponent> m_MeshComponent;
	Vector2                                m_StartPos;
	Vector2                                m_EndPos;
public :
	Vector2 GetStartPos() const
	{
		return m_StartPos;
	}

	Vector2 GetEndPos() const
	{
		return m_EndPos;
	}

public :
	void SetStartPos(const Vector2& Pos);
	void SetEndPos(const Vector2& Pos);
public:
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual CDragObject* Clone() override;
};
