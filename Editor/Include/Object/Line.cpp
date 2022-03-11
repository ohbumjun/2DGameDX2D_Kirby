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
	오른쪽 위의 방식으로 하려면, 적어도 둘중 하나는 World Scale이 음수
	오른쪽 아래여야 하려면, WorldScale 둘다 음수 혹은 둘다 양수

	오른쪽 위일 때 시작점은
	x,y 둘다 제일 낮은 점
	그리고 끝점은 x,y 둘다 큰 점

	오른쪽 아래일 때 시작점은
	X는 작은 것, Y는 큰 것
	끝점은 X는 큰 것, Y는 작은 것

	1) 오른쪽 아래로 선 긋기 ---
	그러면 오른쪽 아래로 선을 그을 때는 Y만 - 가 된다.
	그래서 오른쪽 위로 그려지는 것이다.

	그러면 이때는
	StartPos의 경우,
	WorldScale은 Y도 양수로 바꿔줘야 하고

	StartPos 의 Y는 EndPos.y로 수정해줘야 한다.
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
			2) 오른쪽 위로 선 긋기 --
			이때는 Y가 + 인데, Y를 -로 바꿔줘야 한다.
			그리고 StartPos의 Y도 EndPos.y로 다시 세팅해줘야 한다.
		*/
		XScale = Pos.x - m_StartPos.x;
		YScale = m_StartPos.y - Pos.y;

		m_DrawType = Line_DrawType::RightUp;
	}
	else if (Pos.x < m_StartPos.x && Pos.y < m_StartPos.y)
	{
		/*
		3) 왼쪽 아래로 선 긋기 ---
		XScale 과 YScale 모두 음수가 나올 것이다

		XScale은 양수로 바꿔야 하며
		StartPos.x 는 EndPos.x 로 바꿔야 한다.
		 */
		XScale = m_StartPos.x - Pos.x;
		YScale = Pos.y - m_StartPos.y;

		m_DrawType = Line_DrawType::LeftBottom;
	}
	else if (Pos.x < m_StartPos.x && Pos.y < m_StartPos.y)
	{
		/*
		3) 왼쪽 아래로 선 긋기 ---
		XScale 과 YScale 모두 음수가 나올 것이다

		XScale은 양수로 바꿔야 하며
		StartPos.x 는 EndPos.x 로 바꿔야 한다.
		 */
		XScale = m_StartPos.x - Pos.x;
		YScale = Pos.y - m_StartPos.y;

		m_DrawType = Line_DrawType::LeftBottom;
	}
	else if (Pos.x < m_StartPos.x && Pos.y > m_StartPos.y)
	{
		/*
		4) 왼쪽 위로 선 긋기 ---
			XScale은 음수가 나올 것이다.
		YScale은 양수가 나올 것이다

		YScale은 양수를 음수로 바꿔주고
		x,y 둘다 EndPos로
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

	// Drag Layer에 세팅하기 
	m_MeshComponent->SetLayerName("DragLayer");
	CRenderManager::GetInst()->AddRenderList(m_MeshComponent);

	m_MeshComponent->SetMesh("Line");
	// m_MeshComponent->SetMesh("FrameRect");
	m_MeshComponent->GetMaterial()->SetShader("PosMeshShader");
	// m_MeshComponent->SetPivot(0.001f, 0.001f, 0.f);
	m_MeshComponent->SetBaseColor(0.f, 1.f, 0.f, 1.f);

	m_MeshComponent->SetWorldPos(200.f, 200.f, 0.f);
	// m_MeshComponent->SetWorldScale(-200.f, -200.f, 0.f); // 오른쪽 아래 0.200 --> 200.0
	// m_MeshComponent->SetWorldScale(-200.f, 200.f, 0.f); // 오른쪽 위 0.200 -> 200.400
	m_MeshComponent->SetWorldScale(200.f, 200.f, 0.f); // 오른쪽 아래 --> 200.400 에서 400.200 으로 
	// m_MeshComponent->SetWorldScale(200.f, -200.f, 0.f); // 오른쪽 위 --> 200.0 에서 400.200 으로

	/*
	오른쪽 위의 방식으로 하려면, 적어도 둘중 하나는 World Scale이 음수
	오른쪽 아래여야 하려면, WorldScale 둘다 음수 혹은 둘다 양수

	오른쪽 위일 때 시작점은
	x,y 둘다 제일 낮은 점
	그리고 끝점은 x,y 둘다 큰 점

	오른쪽 아래일 때 시작점은
	X는 작은 것, Y는 큰 것
	끝점은 X는 큰 것, Y는 작은 것 

	1) 오른쪽 아래로 선 긋기 ---
	그러면 오른쪽 아래로 선을 그을 때는 Y만 - 가 된다.
	그래서 오른쪽 위로 그려지는 것이다.

	그러면 이때는
	StartPos의 경우,
	WorldScale은 Y도 양수로 바꿔줘야 하고

	StartPos 의 Y는 EndPos.y로 수정해줘야 한다.

	2) 오른쪽 위로 선 긋기 --
	이때는 Y가 + 인데, Y를 -로 바꿔줘야 한다.
	그리고 StartPos의 Y도 EndPos.y로 다시 세팅해줘야 한다.

	2) 오른쪽 위로 선 긋기 ---

	3) 왼쪽 아래로 선 긋기 ---
	// m_MeshComponent->SetWorldScale(200.f, -200.f, 0.f); // 오른쪽 위 --> 200.0 에서 400.200 으로

	XScale 과 YSCale 모두 음수가 나올 것이다

	XScale은 양수로 바꿔야 하며
	StartPos.x 는 EndPox.s 로 바꿔야 한다.

	4) 왼쪽 위로 선 긋기 ---
	// m_MeshComponent->SetWorldScale(-200.f, -200.f, 0.f); // 오른쪽 아래 0.200 --> 200.0

	XScale은 음수가 나올 것이다.
	YScale은 양수가 나올 것이다

	YScale은 양수를 음수로 바꿔주고
	x,y 둘다 EndPos로 
	
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
