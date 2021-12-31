#include "CameraComponent.h"
#include "../Device.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();

	// 이 녀석은 보이지 않게 세팅한다.
	m_Render = false;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& com) :
CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_matProj = com.m_matProj;
	m_matView = com.m_matView;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_RS = com.m_RS;
}

CCameraComponent::~CCameraComponent()
{}

void CCameraComponent::CreateProjectionMatrix()
{
	
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	// Start 할때 Create Projection Matrix를 사용한다
	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_RS = CDevice::GetInst()->GetResolution();
	return true;
}

void CCameraComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	/*
	카메라가 뷰공간으로 변환이 되면, 카메라를 구성하는
	x,y,z 축은 월드의 축과 일치하게 된다.
	
	즉, 카메라의 축은 결과적으로
	x : 1, 0, 0  y : 0, 1, 0  z : 0, 0, 1 로 구성이 된다는 것이다
	그리고 각각의 축을 열로 하는 행렬을 만들면
	모든 열이 서로 직교하는 정직교 행렬을 만들어낼 수 있다.

	1 0 0
	0 1 0
	0 0 1

	카메라 X축 * 뷰행렬 = 1, 0, 0
	카메라 y축 * 뷰행렬 = 0, 1, 0
	카메라 z축 * 뷰행렬 = 0, 0, 1

	X축x, X축y, X축z   0 0 0   1 0 0
	Y축x, Y축y, Y축z * 0 0 0 = 0 1 0
	Z축x, Z축y, Z축z	  0 0 0   0 0 1

	카메라 x,y,z 축에 각각 뷰행렬을 곱하게 되면
	각각 1,0,0 // 0,1,0 // 0,0,1이라는 값을 지니게 된다는 것이다

	뷰행렬은
	1) 이동 2) 회전 행렬
	형태로 구성되어야 한다

	먼저 카메라의 위치를
	월드상의 위치로 가져다 두고

	그 위치에서 카메라의 축들을
	월드상의 축으로 회전시켜야 한다는 것이다.

	1st //
	첫번째 이동 행렬은 다음과 같다.
	1   0  0 0
	0   1  0 0
	0   0  1 0
	-x -y -z 1

	2nd //
	직교 행렬의 경우, 전치행렬과 역행렬이 동일하다는
	특성을 지닌다.

	다른 말로 하면
	카메라의 원래 x,y,z 축은 직교 행렬이라는 것이고
	카메라의 3개의 축을 행으로 갖는 행렬은
	전치행렬이라는 것
	그리고 그것의 역행렬은, 전치행렬
	즉, 3개의 축을 column으로 갖는 행이 된다.

	따라서 뷰 회전 행렬은
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1

	Final //
	1   0  0 0   Xx Yx Zx 0
	0   1  0 0 * Xy Yy Zy 0
	0   0  1 0   Xz Yz Zz 0
	-x -y -z 1   0  0  0  1

	최종 뷰 행렬
	Xx    Yx   Zx  0
	Xy    Yy   Zy  0
	Xz    Yz   Zz  0
	-X.P -Y.P -Z.P 1
	
	 */

}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// 각 축을 구하는 것 == 단위벡터 형태로 만들어준다
	m_matView.Identity();

	/*
		i가 0일때
		Xx Xy Xz 0
		0  1  0  0
		0  0  1  0
		0  0  0  1

		i가 1일때
		Xx Xy Xz 0
		Yx Yy Yz 0
		0  0  1  0
		0  0  0  1

		i가 2일때
		Xx Xy Xz 0
		Yx Yy Yz 0
		Zx Zy Zz 0
		0  0  0  1
	*/

	// 아래의 코드는, 각 축을 행으로 하는 행렬을 만드는 것이다
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// 뷰 회전 행렬을 구해주기 위해서는
	// 위의 행렬을 전치 시켜주어야 한다.
	// (한편 : 카메라 좌표계 입장에서는 자전이지만, 다른 Object 입장에서는
	// 해당 카메라 위치를 기준으로 공전하는 형태가 될 것이다 )
	m_matView.Transpose();

	// 그리고 카메라 축의 위치를 0,0,0으로 옮기는 과정이므로
	// Pos의 - 를 해줘서 더해줄 것이다.

	// 결과적으로,
	/*
	Xx      Yx     Zx  0
	Xy      Yy     Zy  0
	Xz		  Yz     Zz  0
	- X.P - Y.P - Z.P 1

	이런 형태가 될 것이고 가장 마자막 행렬들의
	원소는, 각 축과, 위치와의 곱 == 내적
	*/
	Vector3 Pos = GetWorldPos() * -1.f;

	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		m_matView[3][i] = Pos.Dot(Axis);
	}
}

void CCameraComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* pFile)
{
	fwrite(&m_CameraType, sizeof(Camera_Type), 1, pFile);
	fwrite(&m_RS, sizeof(Resolution), 1, pFile);
	fwrite(&m_ViewAngle, sizeof(float), 1, pFile);
	fwrite(&m_Distance, sizeof(float), 1, pFile);
	fwrite(&m_matProj, sizeof(Matrix), 1, pFile);
	fwrite(&m_matView, sizeof(Matrix), 1, pFile);

	CSceneComponent::Save(pFile);
}

void CCameraComponent::Load(FILE* pFile)
{
	fread(&m_CameraType, sizeof(Camera_Type), 1, pFile);
	fread(&m_RS, sizeof(Resolution), 1, pFile);
	fread(&m_ViewAngle, sizeof(float), 1, pFile);
	fread(&m_Distance, sizeof(float), 1, pFile);
	fread(&m_matProj, sizeof(Matrix), 1, pFile);
	fread(&m_matView, sizeof(Matrix), 1, pFile);

	CSceneComponent::Load(pFile);
}
