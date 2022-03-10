#pragma once
#include "GameObject\GameObject.h"

class CLineObject :
    public CGameObject
{
	friend class CScene;

protected:
	CLineObject();
	CLineObject(const CLineObject& Obj);
	virtual ~CLineObject() override;
public:
	CSharedPtr<class CStaticMeshComponent> m_MeshComponent;
	Vector2                                m_StartPos;
	Vector2                                m_EndPos;
public:
	Vector2 GetStartPos() const
	{
		return m_StartPos;
	}

	Vector2 GetEndPos() const
	{
		return m_EndPos;
	}

public:
	void SetStartPos(const Vector2& Pos);
	void SetEndPos(const Vector2& Pos);
public:
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual CLineObject* Clone() override;
};

