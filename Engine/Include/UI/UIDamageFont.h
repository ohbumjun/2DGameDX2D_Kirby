#pragma once
#include "UIWidget.h"

class CUIDamageFont :
    public CUIWidget
{
    friend class CUIWindow;
protected:
    CUIDamageFont();
    CUIDamageFont(const CUIDamageFont& Font);
    virtual ~CUIDamageFont() override;
private :
    WidgetImageInfo m_Info;
    int m_DamageNumber;
    float m_LifeTime;
    std::vector<int> m_vecNumber;
private :
    float m_DirX;
    float m_SpeedX;
    bool m_PhysicsSimulate;
    float m_FallTime;
    float m_JumpVelocity;
    float m_FallStartY;
    bool m_IsGround;
    bool m_Jump;
public :
    void SetLifeTime(float LifeTime)
{
        m_LifeTime = LifeTime;
}
    void SetDamage(int Damage)
{
        m_DamageNumber = Damage;
}
    void SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTextureTint(const Vector4& Color);
    void SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& StartPos, const Vector2& Size);
    void AddFrameData(int Count);
public :
    void SetJumpVelocity(float Velocity)
	{
        m_JumpVelocity = Velocity;
	}
    void SetPhysicsSimulate(bool Enable)
    {
        m_PhysicsSimulate = Enable;
    }

    void Jump()
	{
		if (!m_Jump)
		{
            m_Jump = true;
            m_IsGround = false;
		}
        m_FallStartY = m_Pos.y;
	}
public :
    virtual void Start()override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Render() override;
};

/*
등속 운동 -> 운동거리 = 속력 ( v ) * 운동시간 ( t )

속력 : 단위 시간 당 이동한 거리 ( 이동거리 / 걸린 시간 )

속력은 스칼라값이기 때문에, 방향을 나타낼 수 없다.

반면, 속도는 ?
속도 = ( 변위 / 걸린 시간 ) --> 변위 : 중간의 이동거리, 방향 생각 x, 그저 출발점과 도착점의 직선거리와 방향

방향을 포함 -> 벡터값.

정리하자면,
속력은 단위 시간동안 이동한 거리
속도는 단위 시간당 출발점과의 거리와 방향 

-------------------------------------------------------

 
 */