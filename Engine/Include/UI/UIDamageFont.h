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
��� � -> ��Ÿ� = �ӷ� ( v ) * ��ð� ( t )

�ӷ� : ���� �ð� �� �̵��� �Ÿ� ( �̵��Ÿ� / �ɸ� �ð� )

�ӷ��� ��Į���̱� ������, ������ ��Ÿ�� �� ����.

�ݸ�, �ӵ��� ?
�ӵ� = ( ���� / �ɸ� �ð� ) --> ���� : �߰��� �̵��Ÿ�, ���� ���� x, ���� ������� �������� �����Ÿ��� ����

������ ���� -> ���Ͱ�.

�������ڸ�,
�ӷ��� ���� �ð����� �̵��� �Ÿ�
�ӵ��� ���� �ð��� ��������� �Ÿ��� ���� 

-------------------------------------------------------

 
 */