#pragma once
#include "GameObject\GameObject.h"
#include "../Editor.h"

class CLine :
    public CGameObject
{
	friend class CScene;

protected:
	CLine();
	CLine(const CLine& Obj);
	virtual ~CLine() override;
public:
	CSharedPtr<class CStaticMeshComponent> m_MeshComponent;
	Vector2                                m_StartPos;
	Vector2                                m_EndPos;
	Line_DrawType m_DrawType;
public:
	Line_DrawType GetDrawType ()  const
	{
		return m_DrawType;
	}
	Vector3 GetStartPos() const
	{
		return Vector3(m_StartPos.x, m_StartPos.y, 1.f);
	}
	Vector3 GetEndPos() const
	{
		return Vector3(m_EndPos.x, m_EndPos.y, 1.f);
	}

public:
	void SetStartPos(const Vector2& Pos);
	void SetEndPos(const Vector2& Pos);
public:
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual CLine* Clone() override;
};

