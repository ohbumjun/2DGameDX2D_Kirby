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
private :
	Vector2                                m_DrawStartPos;
	Vector2                                m_DrawEndPos;
	Line_DrawType m_DrawType;
private :
	CSharedPtr<class CStaticMeshComponent> m_StartPosBox;
	CSharedPtr<class CStaticMeshComponent> m_EndPosBox;
public:
	Line_DrawType GetDrawType ()  const
	{
		return m_DrawType;
	}
	Vector3 GetStartPos() const
	{
		return Vector3(m_DrawStartPos.x, m_DrawStartPos.y, 1.f);
	}
	Vector3 GetEndPos() const
	{
		return Vector3(m_DrawEndPos.x, m_DrawEndPos.y, 1.f);
	}

public:
	void SetStartPos(const Vector2& Pos);
	void SetEndPos(const Vector2& Pos);
	void ResetDrawBoxPos();
public:
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual CLine* Clone() override;
};

