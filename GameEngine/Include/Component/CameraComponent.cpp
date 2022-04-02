#include "CameraComponent.h"
#include "../Engine.h"
#include "../Scene/Scene.h"
#include "../GameObject/LifeObject.h"

CCameraComponent::CCameraComponent() :
	m_AdjustRatio(true),
	m_FollowPlayer(false),
	m_FollowTarget(false),
	m_FollowTargetTime(3.0f),
	m_FollowTargetTimeMax(3.0f),
	m_CameraShakeTime(0.f),
	// m_CameraShakeTimeMax(0.1f),
	m_CameraShakeTimeMax(0.5f),
	m_IsCameraShake(false)
{
	SetTypeID<CCameraComponent>();
	m_ComponentType = Component_Type::SceneComponent;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
	m_CameraType = Camera_Type::Camera2D;

	m_Render = false;

	m_RatioAdjustOffSet = 2.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& Camera)
{
	m_CameraType = Camera.m_CameraType;
	m_ViewAngle = Camera.m_ViewAngle;
	m_Distance = Camera.m_Distance;
	m_matView = Camera.m_matView;
	m_matProj = Camera.m_matProj;
	m_RS = Camera.m_RS;
	m_Ratio = Camera.m_Ratio;
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

	matView.Transpose();

	// 이동행렬 까지 적용 = 마지막 행 작성 == 각 축과 현재 위치의 내적 형태
	// Vector3 Pos = GetWorldPos() * ScrollRatio * -1.f;
	Vector3 Pos = Vector3(GetWorldPos().x * -1.f * ScrollRatio, GetWorldPos().y * -1.f * ScrollRatio, GetWorldPos().z * -1.f);

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
	case Camera_Type::Camera2D:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle),
			m_RS.Width / (float)m_RS.Height, 0.1f, m_Distance);
		break;
	case Camera_Type::CameraUI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	}
}

bool CCameraComponent::LimitCameraAreaInsideWorld()
{
	if (!m_AdjustRatio)
		return false;

	bool Outside = false;

	// Limit Area
	Vector2 WorldRS = m_Scene->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	Vector3 CameraOriginPos = GetWorldPos();

	// Up
	if (CameraOriginPos.y + RS.Height >= WorldRS.y)
	{
		CameraOriginPos.y = WorldRS.y - RS.Height - 0.0001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	// Down
	if (CameraOriginPos.y < 0.f)
	{
		CameraOriginPos.y = 0.0001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	// Right
	if (CameraOriginPos.x + RS.Width > WorldRS.x)
	{
		CameraOriginPos.x = WorldRS.x - RS.Width - 0.0001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	// Left
	if (CameraOriginPos.x < 0.f)
	{
		CameraOriginPos.x = 0.0001f;

		SetWorldPos(CameraOriginPos);

		Outside = true;
	}

	return Outside;
}

void CCameraComponent::AdjustCameraPosToRatio()
{
	// 현재 Player를 따라가고 있는 중이라면 X
	if (m_FollowPlayer)
		return;

	if (m_FollowTarget)
		return;

	if (!m_AdjustRatio)
		return;

	// Player가 Is Ground 혹은 Side Collision 일때는 비율 조정 X
	if (GetGameObject() == m_Scene->GetPlayerObject())
	{
		CLifeObject* OwnerObject = (CLifeObject*)GetGameObject();

		if (OwnerObject->IsBottomCollided() || OwnerObject->IsSideCollided())
			return;
	}

	// Player가 Is Ground 혹은 Side Collision 일때는 비율 조정 X
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

	// 오른쪽 경계에 걸리면 RelativeX 는 더 - 가 된다.
	// 오른쪽 경계에 막혔던 상황 --> 이전보다 왼쪽으로 온 상황
	// 왼쪽으로 온 만큼, 기존 RelativeX 값에 + 해준다.
	if (CurRelativePos.x < m_RS.Width * m_Ratio.x * -1.f - m_RatioAdjustOffSet)
	{
		NewRelativePos.x = m_PrevRelativePos.x + WorldPosDiff;
	}
	// 왼쪽 경계에 걸리면 ReleativeX 는 더 + 가 된다 ( 덜 - 가 된다. )
	// 왼쪽 경계에 막혔던 상황 --> 이전보다 오른쪽으로 온 상황
	// 오른쪽으로 온만큼 기존 RelativeX 값에 - 해준다.
	else if (CurRelativePos.x > m_RS.Width * m_Ratio.x * -1.f + m_RatioAdjustOffSet)
	{
		NewRelativePos.x = m_PrevRelativePos.x - WorldPosDiff;
	}

	// 위쪽 경계에 걸리면, ReleativeY는 더 - 가 된다
	// 위쪽 경계에 막혔던 상황 --> 이전보다 아래로 온 상황
	// 아래로 온 만큼, 기존 RelativeY 값에 + 해준다.
	if (CurRelativePos.y < m_RS.Height * m_Ratio.y * -1.f - m_RatioAdjustOffSet)
	{
		NewRelativePos.y = m_PrevRelativePos.y + WorldPosDiff;
	}
	// 아래쪽 경계에 걸리면 RelativeY 는 더 + 가 된다 ( 덜 + 가 된다. )
	// 아래쪽 경계에 막혔던 상황 --> 이전보다 위로 온 상황
	// 위로 온 만큼, 기존 RelativeY 값에 - 해준다.
	else if (CurRelativePos.y > m_RS.Height * m_Ratio.y * -1.f + m_RatioAdjustOffSet)
	{
		NewRelativePos.y = m_PrevRelativePos.y - WorldPosDiff;
	}

	SetRelativePos(NewRelativePos);
}

void CCameraComponent::FollowPlayerPos(float DeltaTime)
{
	if (m_FollowPlayer && m_Scene->GetPlayerObject())
	{
		CLifeObject* Player = (CLifeObject*)m_Scene->GetPlayerObject();

		Vector3  PlayerWorldPos = Player->GetWorldPos();

		Resolution RS = CEngine::GetInst()->GetResolution();

		Vector3 TargetMovePos = Vector3(PlayerWorldPos.x - (float)RS.Width * 0.5f, PlayerWorldPos.y - (float)RS.Height * 0.5f, 0.f);

		if (TargetMovePos.x < 0.f)
			TargetMovePos.x = 0.f;

		if (TargetMovePos.y < 0.f)
			TargetMovePos.y = 0.f;

		Vector3 TraceDir = TargetMovePos - GetWorldPos();

		float DistDiff = TraceDir.Length();

		TraceDir.Normalize();

		AddWorldPos(Vector3(TraceDir * DeltaTime * 550.f));

		if (DistDiff < 1.f)
		{
			SetInheritParentWorldPosChange(true);

			m_FollowPlayer = false;

			Player->GetRootComponent()->AddChild(this);

			Player->SetPhysicsSimulate(true);
		}
	}
}

void CCameraComponent::FollowTarget(float DeltaTime)
{
	if (m_FollowTarget && m_FollowTargetBoss)
	{
		Vector3  TargetWorldPos = m_FollowTargetBoss->GetWorldPos();

		Resolution RS = CEngine::GetInst()->GetResolution();

		Vector3 TargetMovePos = Vector3(TargetWorldPos.x - (float)RS.Width * 0.5f, TargetWorldPos.y - (float)RS.Height * 0.5f, 0.f);

		if (TargetMovePos.x < 0.f)
			TargetMovePos.x = 0.f;

		if (TargetMovePos.y < 0.f)
			TargetMovePos.y = 0.f;

		Vector3 TraceDir = TargetMovePos - GetWorldPos();

		float DistDiff = TraceDir.Length();

		TraceDir.Normalize();

		AddWorldPos(Vector3(TraceDir * DeltaTime * 550.f));

		bool IsOutOfRange = LimitCameraAreaInsideWorld();

		if (DistDiff < 1.f || IsOutOfRange)
		{
			if (m_FollowTargetTime > 0)
			{
				m_FollowTargetTime -= DeltaTime;
			}
			else
			{
				// �ٽ� Player�� ���ư��� �����Ѵ�.
				m_FollowTargetTime = m_FollowTargetTimeMax;

				m_FollowTargetBoss = nullptr;
				m_FollowTarget = false;

				m_FollowPlayer = true;

				// �׸��� ������ BossHUD�� ���̰� �����Ѵ�.
				m_Scene->GetViewPort()->StartBossHUDCallback();
			}
		}
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

	bool IsOutSideWorld = LimitCameraAreaInsideWorld();

	if (!IsOutSideWorld && m_Parent)
	{
		AdjustCameraPosToRatio();
	}

	// Camera ���� ������ 2�� �Ѵ�.
	LimitCameraAreaInsideWorld();

	FollowPlayerPos(DeltaTime);

	FollowTarget(DeltaTime);

	UpdateShakeEffect(DeltaTime);
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	m_matView.Identity();

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	m_matView.Transpose();

	Vector3	Pos = GetWorldPos() * -1.f;

	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);

		m_matView[3][i] = Pos.Dot(Axis);
	}

	m_PrevRelativePos = GetRelativePos();
	m_PrevWorldPos = GetWorldPos();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}


void CCameraComponent::ApplyShakeEffect(bool HardShake)
{
	m_IsCameraShake = true;
	m_CameraShakeTime = m_CameraShakeTimeMax;

	if (HardShake)
	{
		m_CameraShakeTime *= 2.f;
	}
}

void CCameraComponent::ApplyWeakShakeEffect()
{
	m_IsCameraShake = true;
	m_CameraShakeTime = m_CameraShakeTimeMax * 0.5f;
}

void CCameraComponent::UpdateShakeEffect(float DeltaTime)
{
	if (!m_IsCameraShake)
		return;

	m_CameraShakeTime -= DeltaTime;

	m_CameraShakePassTime += DeltaTime;

	if (m_CameraShakePassTime >= m_CameraShakeTime * 0.1f)
	{
		if (m_ShakeRight)
			m_ShakeRight = false;
		else
			m_ShakeRight = true;

		m_CameraShakePassTime = 0.f;
	}

	if (m_ShakeRight)
	{
		AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * 700.f);
	}
	else
	{
		AddWorldPos(Vector3(-1.f, 0.f, 0.f) * DeltaTime * 700.f);
	}


	if (m_CameraShakeTime < 0.f)
	{
		m_IsCameraShake = false;

		// SetInheritParentWorldPosChange(true);
	}
}