#include "CameraComponent.h"
#include "../Engine.h"
#include "../Scene/Scene.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
	m_CameraType = Camera_Type::Camera2D;
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

	// View Transform 진행하기
	// basis를 세팅하기 위해, 단위 행렬로 만들어준다.
	matView.Identity();

	// 각 Row를 행으로 하는 행렬 작성
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&matView[i][0], &Axis, sizeof(Vector3));
	}

	// Transpose 시켜주기
	matView.Transpose();

	// 이동행렬 까지 적용 = 마지막 행 작성 == 각 축과 현재 위치의 내적 형태
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

	// Limit Area
	Vector2 WorldRS = m_Scene->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	Vector3 CameraOriginPos = GetWorldPos();

	// Up
	if (CameraOriginPos.y + RS.Height * 0.5f >= WorldRS.y)
	{
		CameraOriginPos.y = WorldRS.y - (RS.Height * 0.5f);

		SetWorldPos(CameraOriginPos);
	}
	else if (CameraOriginPos.y < 0.f)
	{
		CameraOriginPos.y = 0.0f;

		SetWorldPos(CameraOriginPos);
	}

	// Down

}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// View Transform 진행하기
	// basis를 세팅하기 위해, 단위 행렬로 만들어준다.
	m_matView.Identity();

	// 각 Row를 행으로 하는 행렬 작성
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// Transpose 시켜주기
	m_matView.Transpose();

	// 이동행렬 까지 적용 = 마지막 행 작성 == 각 축과 현재 위치의 내적 형태
	Vector3 Pos = GetWorldPos() * -1.f;

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		m_matView[3][i] = Pos.Dot(Axis);
	}

}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}
