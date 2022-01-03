#pragma once
#include "SceneComponent.h"
class CCameraComponent :
    public CSceneComponent
{
    friend class CCameraManager;
public :
    CCameraComponent();
    CCameraComponent(const CCameraComponent& Camera);;
    virtual ~CCameraComponent() override;
private :
    Camera_Type m_CameraType;
    Resolution m_RS;
    float m_ViewAngle;
    float m_Distance;
    Vector2 m_Ratio;
    Matrix m_matView;
    Matrix m_matProj;
public :
    Camera_Type GetCameraType() const
{
        return m_CameraType;
}
    Resolution GetResolution() const
{
        return m_RS;
}
    Matrix GetProjMatrix() const
{
        return m_matProj;
}
    Matrix GetViewMatrix() const
{
        return m_matView;
}
    Vector2 GetLeftBottom() const
{
        Vector2 LB;
        LB.x = GetWorldPos().x;
        LB.y = GetWorldPos().y;
        return LB;
}
public :
    void SetCameraType(Camera_Type Type)
{
        m_CameraType = Type;
		CreateProjectionMatrix();
}
    void SetViewAngle(float Angle)
{
        m_ViewAngle = Angle;
        if (m_CameraType == Camera_Type::Camera3D)
        {
            CreateProjectionMatrix();
        }
}
    void SetDistance(float Distance)
{
        m_Distance = Distance;
        CreateProjectionMatrix();
}
public :
    // 2D Àü¿ë
    void OnViewPortCenter()
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
	        float z = GetRelativePos().z;
	        SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);

	        m_Ratio.x = 0.5f;
	        m_Ratio.y = 0.5f;
		}
	}
    void SetViewPortRatio(float RatioX, float RatioY)
	{
        if (RatioX > 1.f || RatioX < 0.f)
            RatioX = 0.f;
        if (RatioY > 1.f || RatioY < 0.f)
            RatioY = 0.f;

		if (m_CameraType == Camera_Type::Camera2D)
		{
	        float z = GetRelativePos().z;
	        SetRelativePos(m_RS.Width * -RatioX, m_RS.Height * -RatioY, z);

	        m_Ratio.x = RatioX;
	        m_Ratio.y = RatioY;
		}
	}
public :
    void CreateProjectionMatrix();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void PostUpdate(float DeltaTime) override;
public :
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};


