#pragma once
#include "SceneComponent.h"
class CCameraComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCameraManager;
protected:
    CCameraComponent();
    CCameraComponent(const CCameraComponent& com);
    virtual ~CCameraComponent();
protected:
    // Camera �� ���
    // 2D, 3D, UI �� Camera�� �и����־�� �Ѵ�.
    Camera_Type m_CameraType;
    Resolution m_RS;
    // ī�޶��� ȭ�� : ������ ���̰� �� ���ΰ�
    float m_ViewAngle;
    // ���� �ִ� �Ѱ�Ÿ�
    float m_Distance;
    Matrix m_matProj;
    Matrix m_matView;
public:
    Camera_Type GetCameraType() const
    {
        return m_CameraType;
    }
    Matrix GetProjectionMatrix() const
    {
        return m_matProj;
    }
    Matrix GetViewMatrix() const
    {
        return m_matView;
    }
public :
    // 3D ���� ���� ������
    // ��������� �ٽ� �ѹ� ������ش�
    void SetViewAngle(float Angle)
    {
        m_ViewAngle = Angle;
        if (m_CameraType == Camera_Type::Camera3D)
            CreateProjectionMatrix();
    }
    // �Ÿ��� ���ص� ��������� ������־�� �Ѵ�
    void SetDistance(float Distance)
    {
        m_Distance = Distance;
        CreateProjectionMatrix();
    }
    // ī�޶� Ÿ���� ���ϸ� ��������� �Ǵٽ� ������ش�.
    void SetCameraType(Camera_Type Type)
    {
        m_CameraType = Type;
        CreateProjectionMatrix();
    }
    // 2D ���� : ����� ȭ�� ����� �����صα�
    void OnViewPortCenter()
    {
	    if (m_CameraType == Camera_Type::Camera2D)
	    {
            float z = GetRelativePos().z;
            SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);
	    }
    }
    // 2D ���� : �� ���� ���� ȭ��� ��ġ�� ���ΰ�
    // ������ ȭ�� ��ü������ Pivot �����̶�� �����ϸ� �ȴ�.
    void SetViewPortRatio(float x, float y)
    {
	    if (m_CameraType == Camera_Type::Camera2D)
	    {
            float z = GetRelativePos().z;
            SetRelativePos(m_RS.Width * -x, m_RS.Height * -y, z);
	    }
    }
public:
    void CreateProjectionMatrix();
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CCameraComponent* Clone();
public :
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);

};


