#include "CameraComponent.h"
#include "../Device.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();

	// �� �༮�� ������ �ʰ� �����Ѵ�.
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

	// Start �Ҷ� Create Projection Matrix�� ����Ѵ�
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
	ī�޶� ��������� ��ȯ�� �Ǹ�, ī�޶� �����ϴ�
	x,y,z ���� ������ ��� ��ġ�ϰ� �ȴ�.
	
	��, ī�޶��� ���� ���������
	x : 1, 0, 0  y : 0, 1, 0  z : 0, 0, 1 �� ������ �ȴٴ� ���̴�
	�׸��� ������ ���� ���� �ϴ� ����� �����
	��� ���� ���� �����ϴ� ������ ����� ���� �� �ִ�.

	1 0 0
	0 1 0
	0 0 1

	ī�޶� X�� * ����� = 1, 0, 0
	ī�޶� y�� * ����� = 0, 1, 0
	ī�޶� z�� * ����� = 0, 0, 1

	X��x, X��y, X��z   0 0 0   1 0 0
	Y��x, Y��y, Y��z * 0 0 0 = 0 1 0
	Z��x, Z��y, Z��z	  0 0 0   0 0 1

	ī�޶� x,y,z �࿡ ���� ������� ���ϰ� �Ǹ�
	���� 1,0,0 // 0,1,0 // 0,0,1�̶�� ���� ���ϰ� �ȴٴ� ���̴�

	�������
	1) �̵� 2) ȸ�� ���
	���·� �����Ǿ�� �Ѵ�

	���� ī�޶��� ��ġ��
	������� ��ġ�� ������ �ΰ�

	�� ��ġ���� ī�޶��� �����
	������� ������ ȸ�����Ѿ� �Ѵٴ� ���̴�.

	1st //
	ù��° �̵� ����� ������ ����.
	1   0  0 0
	0   1  0 0
	0   0  1 0
	-x -y -z 1

	2nd //
	���� ����� ���, ��ġ��İ� ������� �����ϴٴ�
	Ư���� ���Ѵ�.

	�ٸ� ���� �ϸ�
	ī�޶��� ���� x,y,z ���� ���� ����̶�� ���̰�
	ī�޶��� 3���� ���� ������ ���� �����
	��ġ����̶�� ��
	�׸��� �װ��� �������, ��ġ���
	��, 3���� ���� column���� ���� ���� �ȴ�.

	���� �� ȸ�� �����
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1

	Final //
	1   0  0 0   Xx Yx Zx 0
	0   1  0 0 * Xy Yy Zy 0
	0   0  1 0   Xz Yz Zz 0
	-x -y -z 1   0  0  0  1

	���� �� ���
	Xx    Yx   Zx  0
	Xy    Yy   Zy  0
	Xz    Yz   Zz  0
	-X.P -Y.P -Z.P 1
	
	 */

}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// �� ���� ���ϴ� �� == �������� ���·� ������ش�
	m_matView.Identity();

	/*
		i�� 0�϶�
		Xx Xy Xz 0
		0  1  0  0
		0  0  1  0
		0  0  0  1

		i�� 1�϶�
		Xx Xy Xz 0
		Yx Yy Yz 0
		0  0  1  0
		0  0  0  1

		i�� 2�϶�
		Xx Xy Xz 0
		Yx Yy Yz 0
		Zx Zy Zz 0
		0  0  0  1
	*/

	// �Ʒ��� �ڵ��, �� ���� ������ �ϴ� ����� ����� ���̴�
	for (int i = 0; i < AXIS_MAX; i++)
	{
		Vector3 Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	// �� ȸ�� ����� �����ֱ� ���ؼ���
	// ���� ����� ��ġ �����־�� �Ѵ�.
	// (���� : ī�޶� ��ǥ�� ���忡���� ����������, �ٸ� Object ���忡����
	// �ش� ī�޶� ��ġ�� �������� �����ϴ� ���°� �� ���̴� )
	m_matView.Transpose();

	// �׸��� ī�޶� ���� ��ġ�� 0,0,0���� �ű�� �����̹Ƿ�
	// Pos�� - �� ���༭ ������ ���̴�.

	// ���������,
	/*
	Xx      Yx     Zx  0
	Xy      Yy     Zy  0
	Xz		  Yz     Zz  0
	- X.P - Y.P - Z.P 1

	�̷� ���°� �� ���̰� ���� ���ڸ� ��ĵ���
	���Ҵ�, �� ���, ��ġ���� �� == ����
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
