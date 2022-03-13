#pragma once
#include "GameObject.h"

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
private:
	Vector2                                m_DrawStartPos;
	Vector2                                m_DrawEndPos;
	Line_DrawType m_DrawType;
private:
	Vector3 m_FinalWorldLeftPos;
	Vector3 m_FinalWorldRightPos;
	float m_Slope;
private:
	CSharedPtr<class CStaticMeshComponent> m_StartPosBox;
	CSharedPtr<class CStaticMeshComponent> m_EndPosBox;
public:
	Line_DrawType GetDrawType()  const
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
	float GetSlope () const
	{
		return m_Slope;
	}
	Vector3 GetFinalWorldLeftPos() const
	{
		return m_FinalWorldLeftPos;
	}
	Vector3 GetFinalWorldRightPos() const
	{
		return m_FinalWorldRightPos;
	}
public:
	void SetFinalPosInfo(const Vector3& FinalLeftPos, const Vector3& FinalRightPos, float Slope);
	void SetStartPos(const Vector2& Pos);
	void SetEndPos(const Vector2& Pos);
	void ResetDrawBoxPos();
	void CalculateFinalPosInfo();
public:
	virtual void SetWorldPos(const Vector3& Pos);
	virtual void SetWorldPos(float x, float y, float z);
public:
	virtual auto   Init() -> bool override;
	virtual void   Start() override;
	virtual void   Update(float DeltaTime) override;
	virtual void   PostUpdate(float DeltaTime) override;
	virtual CLine* Clone() override;
public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;
};



