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

void CLine::SetStartPos(const Vector2& Pos)
{
	m_StartPos = Pos;
	m_MeshComponent->SetWorldPos(m_StartPos.x, m_StartPos.y, 1.f);
}

void CLine::SetEndPos(const Vector2& Pos)
{
	m_EndPos = Pos;
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
	*/
	float XScale = -1.f, YScale = -1.f;

	if (Pos.x >= m_StartPos.x && Pos.y < m_StartPos.y)
	{
		XScale = Pos.x - m_StartPos.x;
		YScale = m_StartPos.y - Pos.y;

		m_DrawType = Line_DrawType::RightBottom;
		// m_StartPos.y = Pos.y;
	}
	else if (Pos.x >= m_StartPos.x && Pos.y >= m_StartPos.y)
	{
		/*
			2) ������ ���� �� �߱� --
			�̶��� Y�� + �ε�, Y�� -�� �ٲ���� �Ѵ�.
			�׸��� StartPos�� Y�� EndPos.y�� �ٽ� ��������� �Ѵ�.
		*/
		XScale = Pos.x - m_StartPos.x;
		YScale = m_StartPos.y - Pos.y;

		m_DrawType = Line_DrawType::RightUp;
	}
	else if (Pos.x < m_StartPos.x && Pos.y < m_StartPos.y)
	{
		/*
		3) ���� �Ʒ��� �� �߱� ---
		XScale �� YScale ��� ������ ���� ���̴�

		XScale�� ����� �ٲ�� �ϸ�
		StartPos.x �� EndPos.x �� �ٲ�� �Ѵ�.
		 */
		XScale = m_StartPos.x - Pos.x;
		YScale = Pos.y - m_StartPos.y;

		m_DrawType = Line_DrawType::LeftBottom;
	}
	else if (Pos.x < m_StartPos.x && Pos.y < m_StartPos.y)
	{
		/*
		3) ���� �Ʒ��� �� �߱� ---
		XScale �� YScale ��� ������ ���� ���̴�

		XScale�� ����� �ٲ�� �ϸ�
		StartPos.x �� EndPos.x �� �ٲ�� �Ѵ�.
		 */
		XScale = m_StartPos.x - Pos.x;
		YScale = Pos.y - m_StartPos.y;

		m_DrawType = Line_DrawType::LeftBottom;
	}
	else if (Pos.x < m_StartPos.x && Pos.y > m_StartPos.y)
	{
		/*
		4) ���� ���� �� �߱� ---
			XScale�� ������ ���� ���̴�.
		YScale�� ����� ���� ���̴�

		YScale�� ����� ������ �ٲ��ְ�
		x,y �Ѵ� EndPos��
		*/
		XScale = Pos.x - m_StartPos.x;
		YScale = m_StartPos.y - Pos.y;

		m_DrawType = Line_DrawType::LeftUp;
	}

	m_MeshComponent->SetWorldScale(XScale, YScale, 1.f);
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

	return true;
}

void CLine::Update(float DeltaTime)
{
	/*
	Vector3 StartPos = GetWorldPos();
	Vector3 WorldScale = GetWorldScale();
	Vector3 EndPos = Vector3(StartPos.x + WorldScale.x, StartPos.y + WorldScale.y, 0.f);

	Vector3 Vertices[2] =
	{
		StartPos,
		EndPos
	};

	CDevice::GetInst()->GetContext()->UpdateSubresource(
		m_MeshComponent->GetMesh()->GetMeshContainer()->VB.Buffer,
		0,
		NULL,
		Vertices,
		sizeof(Vector3) * 2,
		0
	);
	*/

	//cm_MeshComponent->GetMesh()->GetMeshContainer()->VB.Buffer;

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
