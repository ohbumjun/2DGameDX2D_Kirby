#include "Line.h"
#include "Component/StaticMeshComponent.h"
#include "Render/RenderManager.h"
#include "Resource/Material/Material.h"
#include "Resource/Mesh/Mesh.h"
#include "Resource/Mesh/StaticMesh.h"
#include "../Window/SpriteEditWindow.h"
#include "../EditorManager.h"
#include "SpriteEditObject.h"
#include "Device.h"

CLine::CLine()
{
	SetTypeID<CLine>();
}

CLine::CLine(const CLine& Obj) :
	CGameObject(Obj)
{
	m_MeshComponent = dynamic_cast<CStaticMeshComponent*>(FindComponent("Mesh"));
}

CLine::~CLine()
{
}

void CLine::SetFinalPosInfo(const Vector3& FinalLeftPos, const Vector3& FinalRightPos, float Slope)
{
	m_FinalLeftPos = FinalLeftPos;
	m_FinalRightPos = FinalRightPos;
	m_Slope = Slope;
}

void CLine::SetStartPos(const Vector2& Pos)
{
	m_DrawStartPos = Pos;
	m_MeshComponent->SetWorldPos(m_DrawStartPos.x, m_DrawStartPos.y, 1.f);
}

void CLine::SetEndPos(const Vector2& Pos)
{
	m_DrawEndPos = Pos;
	/*
	������ ���� ������� �Ϸ���, ��� ���� �ϳ��� World Scale�� ����
	������ �Ʒ����� �Ϸ���, WorldScale �Ѵ� ���� Ȥ�� �Ѵ� ���

	������ ���� �� ��������
	x,y �Ѵ� ���� ���� ��
	�׸��� ������ x,y �Ѵ� ū ��

	������ �Ʒ��� �� ��������
	X�� ���� ��, Y�� ū ��
	������ X�� ū ��, Y�� ���� ��
	*/
	float XScale = -1.f, YScale = -1.f;

	if (Pos.x >= m_DrawStartPos.x && Pos.y < m_DrawStartPos.y)
	{
		/*
			1) ������ �Ʒ��� �� �߱� ---
			�׷��� ������ �Ʒ��� ���� ���� ���� Y�� - �� �ȴ�.
			�׷��� ������ ���� �׷����� ���̴�.

			�׷��� �̶���
			StartPos�� ���,
			WorldScale�� Y�� ����� �ٲ���� �ϰ�

			StartPos �� Y�� EndPos.y�� ��������� �Ѵ�.

			
			XScale = Pos.x - m_DrawStartPos.x;
			YScale = m_DrawStartPos.y - Pos.y;
		 */

		m_DrawType = Line_DrawType::RightBottom;
	}
	else if (Pos.x >= m_DrawStartPos.x && Pos.y >= m_DrawStartPos.y)
	{
		/*
			2) ������ ���� �� �߱� --
			�̶��� Y�� + �ε�, Y�� -�� �ٲ���� �Ѵ�.
			�׸��� StartPos�� Y�� EndPos.y�� �ٽ� ��������� �Ѵ�.
			
			XScale = Pos.x - m_DrawStartPos.x;
			YScale = m_DrawStartPos.y - Pos.y;
		*/

		m_DrawType = Line_DrawType::RightUp;
	}
	else if (Pos.x < m_DrawStartPos.x && Pos.y < m_DrawStartPos.y)
	{
		/*
		3) ���� �Ʒ��� �� �߱� ---
		XScale �� YScale ��� ������ ���� ���̴�

		XScale�� ����� �ٲ�� �ϸ�
		StartPos.x �� EndPos.x �� �ٲ�� �Ѵ�.

		XScale = m_DrawStartPos.x - Pos.x;
		YScale = Pos.y - m_DrawStartPos.y;
		 */

		m_DrawType = Line_DrawType::LeftBottom;
	}
	else if (Pos.x < m_DrawStartPos.x && Pos.y < m_DrawStartPos.y)
	{
		/*
		3) ���� �Ʒ��� �� �߱� ---
		XScale �� YScale ��� ������ ���� ���̴�

		XScale�� ����� �ٲ�� �ϸ�
		StartPos.x �� EndPos.x �� �ٲ�� �Ѵ�.

		
		XScale = m_DrawStartPos.x - Pos.x;
		YScale = Pos.y - m_DrawStartPos.y;
		 */

		m_DrawType = Line_DrawType::LeftBottom;
	}
	else if (Pos.x < m_DrawStartPos.x && Pos.y > m_DrawStartPos.y)
	{
		/*
		4) ���� ���� �� �߱� ---
			XScale�� ������ ���� ���̴�.
		YScale�� ����� ���� ���̴�

		YScale�� ����� ������ �ٲ��ְ�
		x,y �Ѵ� EndPos��

		
		XScale = Pos.x - m_DrawStartPos.x;
		YScale = m_DrawStartPos.y - Pos.y;
		*/

		m_DrawType = Line_DrawType::LeftUp;
	}
	
	m_MeshComponent->SetWorldScale(0, 0, 1.f);

	// ���� ������ �簢�� �׸���
	m_StartPosBox->SetWorldPos(m_DrawStartPos.x, m_DrawStartPos.y, 1.f);
	m_StartPosBox->SetWorldScale(10.f, 10.f, 1.f);

	m_EndPosBox->SetWorldPos(m_DrawEndPos.x, m_DrawEndPos.y, 1.f);
	m_EndPosBox->SetWorldScale(10.f, 10.f, 1.f);
}

void CLine::ResetDrawBoxPos()
{
	m_StartPosBox->SetWorldScale(0.f, 0.f, 0.f);
	m_EndPosBox->SetWorldScale(0.f, 0.f, 0.f);
}

bool CLine::Init()
{
	if (!CGameObject::Init())
		return false;

	m_MeshComponent = CreateComponent<CStaticMeshComponent>("Mesh");
	SetRootComponent(m_MeshComponent);

	// Drag Layer�� �����ϱ� 
	m_MeshComponent->SetLayerName("DragLayer");
	CRenderManager::GetInst()->AddRenderList(m_MeshComponent);

	m_MeshComponent->SetMesh("Line");
	// m_MeshComponent->SetMesh("FrameRect");
	m_MeshComponent->GetMaterial()->SetShader("PosMeshShader");
	// m_MeshComponent->SetPivot(0.001f, 0.001f, 0.f);
	m_MeshComponent->SetBaseColor(0.f, 1.f, 0.f, 1.f);

	m_MeshComponent->SetWorldPos(200.f, 200.f, 0.f);
	// m_MeshComponent->SetWorldScale(-200.f, -200.f, 0.f); // ������ �Ʒ� 0.200 --> 200.0
	// m_MeshComponent->SetWorldScale(-200.f, 200.f, 0.f); // ������ �� 0.200 -> 200.400
	m_MeshComponent->SetWorldScale(200.f, 200.f, 0.f); // ������ �Ʒ� --> 200.400 ���� 400.200 ���� 
	// m_MeshComponent->SetWorldScale(200.f, -200.f, 0.f); // ������ �� --> 200.0 ���� 400.200 ����

	/*
	������ ���� ������� �Ϸ���, ��� ���� �ϳ��� World Scale�� ����
	������ �Ʒ����� �Ϸ���, WorldScale �Ѵ� ���� Ȥ�� �Ѵ� ���

	������ ���� �� ��������
	x,y �Ѵ� ���� ���� ��
	�׸��� ������ x,y �Ѵ� ū ��

	������ �Ʒ��� �� ��������
	X�� ���� ��, Y�� ū ��
	������ X�� ū ��, Y�� ���� �� 

	1) ������ �Ʒ��� �� �߱� ---
	�׷��� ������ �Ʒ��� ���� ���� ���� Y�� - �� �ȴ�.
	�׷��� ������ ���� �׷����� ���̴�.

	�׷��� �̶���
	StartPos�� ���,
	WorldScale�� Y�� ����� �ٲ���� �ϰ�

	StartPos �� Y�� EndPos.y�� ��������� �Ѵ�.

	2) ������ ���� �� �߱� --
	�̶��� Y�� + �ε�, Y�� -�� �ٲ���� �Ѵ�.
	�׸��� StartPos�� Y�� EndPos.y�� �ٽ� ��������� �Ѵ�.

	2) ������ ���� �� �߱� ---

	3) ���� �Ʒ��� �� �߱� ---
	// m_MeshComponent->SetWorldScale(200.f, -200.f, 0.f); // ������ �� --> 200.0 ���� 400.200 ����

	XScale �� YSCale ��� ������ ���� ���̴�

	XScale�� ����� �ٲ�� �ϸ�
	StartPos.x �� EndPox.s �� �ٲ�� �Ѵ�.

	4) ���� ���� �� �߱� ---
	// m_MeshComponent->SetWorldScale(-200.f, -200.f, 0.f); // ������ �Ʒ� 0.200 --> 200.0

	XScale�� ������ ���� ���̴�.
	YScale�� ����� ���� ���̴�

	YScale�� ����� ������ �ٲ��ְ�
	x,y �Ѵ� EndPos�� 
	
	 */


	m_StartPosBox = CreateComponent<CStaticMeshComponent>("DrawStartPosMesh");
	m_StartPosBox->SetLayerName("DragLayer");
	CRenderManager::GetInst()->AddRenderList(m_StartPosBox);
	m_StartPosBox->SetMesh("FrameRect");
	m_StartPosBox->GetMaterial()->SetShader("PosMeshShader");
	m_StartPosBox->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	m_MeshComponent->AddChild(m_StartPosBox);

	m_EndPosBox = CreateComponent<CStaticMeshComponent>("DrawEndPosMesh");
	m_EndPosBox->SetLayerName("DragLayer");
	CRenderManager::GetInst()->AddRenderList(m_EndPosBox);
	m_EndPosBox->SetMesh("FrameRect");
	m_EndPosBox->GetMaterial()->SetShader("PosMeshShader");
	m_EndPosBox->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	m_MeshComponent->AddChild(m_EndPosBox);

	return true;
}

void CLine::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLine::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLine* CLine::Clone()
{
	return new CLine(*this);
}
