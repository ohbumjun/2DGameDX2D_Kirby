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
    // Camera 으 경우
    // 2D, 3D, UI 용 Camera를 분리해주어야 한다.
    Camera_Type m_CameraType;
    Resolution m_RS;
    // 카메라의 화각 : 어디까지 보이게 할 것인가
    float m_ViewAngle;
    // 볼수 있는 한계거리
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
    // 3D 용을 만들 때에는
    // 투영행렬을 다시 한번 만들어준다
    void SetViewAngle(float Angle)
    {
        m_ViewAngle = Angle;
        if (m_CameraType == Camera_Type::Camera3D)
            CreateProjectionMatrix();
    }
    // 거리가 변해도 투영행렬을 만들어주어야 한다
    void SetDistance(float Distance)
    {
        m_Distance = Distance;
        CreateProjectionMatrix();
    }
    // 카메라 타입이 변하면 투영행렬을 또다시 만들어준다.
    void SetCameraType(Camera_Type Type)
    {
        m_CameraType = Type;
        CreateProjectionMatrix();
    }
    // 2D 전용 : 대상을 화면 가운데에 세팅해두기
    void OnViewPortCenter()
    {
	    if (m_CameraType == Camera_Type::Camera2D)
	    {
            float z = GetRelativePos().z;
            SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);
	    }
    }
    // 2D 전용 : 얼마 비율 정도 화면상에 배치할 것인가
    // 일종의 화면 전체에서의 Pivot 개념이라고 생각하면 된다.
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


