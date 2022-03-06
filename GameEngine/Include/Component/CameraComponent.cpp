#include "CameraComponent.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../GameObject/LifeObject.h"

CCameraComponent::CCameraComponent() 
{
	SetTypeID<CCameraComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
	m_CameraType = Camera_Type::Camera2D;

	m_RatioAdjustOffSet = 2.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& Camera)
{
	m_CameraType = Camera.m_CameraType;
	m_ViewAngle    = Camera.m_ViewAngle;
	m_Distance      = Camera.m_Distance;
	m_matView      = Camera.m_matView;
	m_matProj       = Camera.m_matProj;
	m_RS              = Camera.m_RS;
	m_Ratio           = Camera.m_Ratio;
}

CCameraComponent::~CCameraComponent()
{}

Matrix CCameraComponent::GetRatioViewMatrix(float ScrollRatio)
{
	Matrix matView = m_matView;

	// View Transform �����ϱ�
	// basis�� �����ϱ� ����, ���� ��ķ� ������ش�.
	matView.Identity();

	// �� Row�� ������ �ϴ� ��� �ۼ�
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&matView[i][0], &Axis, sizeof(Vector3));
	}

	// Transpose �����ֱ�
	matView.Transpose();

	// �̵���� ���� ���� = ������ �� �ۼ� == �� ��� ���� ��ġ�� ���� ����
	Vector3 Pos = GetWorldPos() * ScrollRatio * -1.f;

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		matView[3][i] = Pos.Dot(Axis);
	}

	return matView;
}

void CCameraComponent::CreateProjectionMatrix()
{
	switch (m_CameraType)
	{
	case Camera_Type::Camera2D :
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 
			0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle),(float)(m_RS.Width / m_RS.Height), 0.f, (float)m_Distance);
		break;
	case Camera_Type::CameraUI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width,
			0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	}
}

bool CCameraComponent::LimitCameraAreaInsideWorld()
{
	bool Outside = false;

	// Limit Area
	Vector2 WorldRS = m_Scene->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	Vector3 CameraOriginPos = GetWorldPos();

	// Up
	if (CameraOriginPos.y + RS.Height >= WorldRS.y)
	{
		CameraOriginPos.y = WorldRS.y - RS.Height - 0.001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	// Down
	else if (CameraOriginPos.y < 0.f)
	{
		CameraOriginPos.y = 0.001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	// Right
	else if (CameraOriginPos.x + RS.Width > WorldRS.x)
	{
		CameraOriginPos.x = WorldRS.x - RS.Width - 0.001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	// Left
	else if (CameraOriginPos.x < 0.f)
	{
		CameraOriginPos.x = 0.001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	return Outside;
}

void CCameraComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_CameraType, sizeof(Camera_Type), 1, pFile);
	fwrite(&m_matView, sizeof(Matrix), 1, pFile);
	fwrite(&m_matProj, sizeof(Matrix), 1, pFile);
	fwrite(&m_ViewAngle, sizeof(float), 1, pFile);
	fwrite(&m_Distance, sizeof(float), 1, pFile);
	fwrite(&m_RS, sizeof(Resolution), 1, pFile);
	fwrite(&m_Ratio, sizeof(Vector2), 1, pFile);
}

void CCameraComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_CameraType, sizeof(Camera_Type), 1, pFile);
	fread(&m_matView, sizeof(Matrix), 1, pFile);
	fread(&m_matProj, sizeof(Matrix), 1, pFile);
	fread(&m_ViewAngle, sizeof(float), 1, pFile);
	fread(&m_Distance, sizeof(float), 1, pFile);
	fread(&m_RS, sizeof(Resolution), 1, pFile);
	fread(&m_Ratio, sizeof(Vector2), 1, pFile);
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_RS = CEngine::GetInst()->GetResolution();

	return true;
}

void CCameraComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	bool IsOutSideWorld = LimitCameraAreaInsideWorld();


	if (!IsOutSideWorld && m_Parent)
	{
		// todo : �� ���� ���� �ذ�
		// Player�� Is Ground Ȥ�� Side Collision �϶��� ���� ���� X
		if (GetGameObject() == m_Scene->GetPlayerObject())
		{
			CLifeObject* OwnerObject = (CLifeObject*)GetGameObject();

			if (OwnerObject->IsBottomCollided() || OwnerObject->IsSideCollided())
				return;
		}

		// �ڱ� ������ ������ ������ ���⼭ ��ٸ��� �ؾ� �Ѵ� .. ?
		Resolution RS = CEngine::GetInst()->GetResolution();

		Vector3 CurRelativePos = GetRelativePos();
		Vector3 CurWorldPos = GetWorldPos();

		float WorldPosDiff = m_PrevWorldPos.Distance(CurWorldPos);

		Vector3 NewRelativePos = CurRelativePos;

		/*
		if (m_RS.Width * m_Ratio.x * -1.f - m_RatioAdjustOffSet < CurRelativePos.x &&
			CurRelativePos.x < m_RS.Width * m_Ratio.x * -1.f + m_RatioAdjustOffSet &&
			m_RS.Height * m_Ratio.y * -1.f - m_RatioAdjustOffSet < CurRelativePos.y &&
			CurRelativePos.y < m_RS.Height * m_Ratio.y * -1.f + m_RatioAdjustOffSet)
			return;
			*/

		// ������ ��迡 �ɸ��� RelativeX �� �� - �� �ȴ�.
		// ������ ��迡 ������ ��Ȳ --> �������� �������� �� ��Ȳ
		// �������� �� ��ŭ, ���� RelativeX ���� + ���ش�.
		if (CurRelativePos.x < m_RS.Width * m_Ratio.x * -1.f - m_RatioAdjustOffSet)
		{
			NewRelativePos.x = m_PrevRelativePos.x + WorldPosDiff;
		}
		// ���� ��迡 �ɸ��� ReleativeX �� �� + �� �ȴ� ( �� - �� �ȴ�. )
		// ���� ��迡 ������ ��Ȳ --> �������� ���������� �� ��Ȳ
		// ���������� �¸�ŭ ���� RelativeX ���� - ���ش�.
		else if (CurRelativePos.x > m_RS.Width * m_Ratio.x * -1.f + m_RatioAdjustOffSet)
		{
			NewRelativePos.x = m_PrevRelativePos.x - WorldPosDiff;
		}

		// ���� ��迡 �ɸ���, ReleativeY�� �� - �� �ȴ�
		// ���� ��迡 ������ ��Ȳ --> �������� �Ʒ��� �� ��Ȳ
		// �Ʒ��� �� ��ŭ, ���� RelativeY ���� + ���ش�.
		if (CurRelativePos.y < m_RS.Height * m_Ratio.y * -1.f - m_RatioAdjustOffSet)
		{
			NewRelativePos.y = m_PrevRelativePos.y + WorldPosDiff;
		}
		// �Ʒ��� ��迡 �ɸ��� RelativeY �� �� + �� �ȴ� ( �� + �� �ȴ�. )
		// �Ʒ��� ��迡 ������ ��Ȳ --> �������� ���� �� ��Ȳ
		// ���� �� ��ŭ, ���� RelativeY ���� - ���ش�.
		else if (CurRelativePos.y > m_RS.Height * m_Ratio.y * -1.f + m_RatioAdjustOffSet)
		{
			NewRelativePos.y = m_PrevRelativePos.y - WorldPosDiff;
		}

		SetRelativePos(NewRelativePos);
	}

	// Camera ���� ������ 2�� �Ѵ�.
	LimitCameraAreaInsideWorld();
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// View Transform �����ϱ�
	// basis�� �����ϱ� ����, ���� ��ķ� ������ش�.
	m_matView.Identity();

	// �� Row�� ������ �ϴ� ��� �ۼ�
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// Transpose �����ֱ�
	m_matView.Transpose();

	// �̵���� ���� ���� = ������ �� �ۼ� == �� ��� ���� ��ġ�� ���� ����
	Vector3 Pos = GetWorldPos() * -1.f;

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		m_matView[3][i] = Pos.Dot(Axis);
	}

	m_PrevRelativePos = GetRelativePos();
	m_PrevWorldPos    = GetWorldPos();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}
