#include "UIDamageFont.h"

#include "UIWindow.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/ViewPort.h"

CUIDamageFont::CUIDamageFont() :
	m_LifeTime(1.f),
	// m_LifeTime(10.f),
	m_DamageNumber(0),
	m_PhysicsSimulate(false),
	m_SpeedX(1.f),
	m_FallTime(0.f),
	m_JumpVelocity(0.f),
	m_FallStartY(0.f),
	m_IsGround(false),
	m_Jump(false)
{}

CUIDamageFont::CUIDamageFont(const CUIDamageFont& Font) :
CUIWidget(Font)
{
	*this = Font;
	m_IsGround = false;
}

CUIDamageFont::~CUIDamageFont()
{}

void CUIDamageFont::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (m_Owner->GetViewPort())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTexture(Name, FileName, PathName);
		Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);
		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	if (Texture)
	{
		m_Info.m_Texture = Texture;
		SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		SetUseTexture(true);
	}
}

void CUIDamageFont::SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
	const std::string& PathName)
{
	CTexture* Texture = nullptr;

	/*
	if (m_Owner && m_Owner->GetViewPort() && m_Owner->GetViewPort()->GetScene())
	{
		m_Owner->GetViewPort()->GetScene()->GetResource()->LoadTexture(Name, vecFileName, PathName);
		Texture = m_Owner->GetViewPort()->GetScene()->GetResource()->FindTexture(Name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);
		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}
	*/

	CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName);
	Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (Texture)
	{
		m_Info.m_Texture = Texture;
		SetSize((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		SetUseTexture(true);
	}
}

void CUIDamageFont::SetTextureTint(const Vector4& Color)
{
	m_Info.m_Tint = Color;
}

void CUIDamageFont::SetTextureTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Info.m_Tint = Vector4(r / 255.f, g / 255.f, a / 255.f, a / 255.f);
}

void CUIDamageFont::AddFrameData(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData FrameData = {};
	FrameData.Start = StartPos;
	FrameData.Size  = Size;

	m_Info.m_vecFrameData.push_back(FrameData);
}

void CUIDamageFont::AddFrameData(int Count)
{
	for (int i = 0; i < Count; i++)
	{
		AnimationFrameData FrameData = {};
		FrameData.Start = Vector2(0.f, 0.f);
		FrameData.Size = Vector2((float)m_Info.m_Texture->GetWidth(), (float)m_Info.m_Texture->GetHeight());
		m_Info.m_vecFrameData.push_back(FrameData);
	}
}

void CUIDamageFont::Start()
{
	CUIWidget::Start();

	m_DirX = rand() % 2 == 0 ? 1.f : -1.f;
	m_SpeedX = (float)(rand() % 300);

	SetJumpVelocity(30.f);
	SetPhysicsSimulate(true);
	Jump();
}

bool CUIDamageFont::Init()
{
	if (!CUIWidget::Init())
		return false;

	if (m_Owner && m_Owner->GetViewPort() && m_Owner->GetViewPort()->GetScene())
		m_Shader = m_Owner->GetViewPort()->GetScene()->GetResource()->FindShader("NumberShader");
	else
		m_Shader = CResourceManager::GetInst()->FindShader("NumberShader");

	std::vector<TCHAR*> vecFileName;
	for (int i = 0; i < 10; i++)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(FileName);
	}

	SetTexture("Number", vecFileName);
	AddFrameData(10);

	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	return true;
}

void CUIDamageFont::Update(float DeltaTime)
{
	CUIWidget::Update(DeltaTime);

	/*
	������ �
	- ���� �������δ�, ��ü�� �ۿ��ϴ� ���� 0�̹Ƿ�, ��ӵ� �
	- ���� �������δ�, ��ü�� �߷��� �ۿ��ϹǷ�, ��ӵ� �
	���� ��ģ ��

	ó�� ���� ��ġ�� 0 �̶�� ���� ��,
	�ð� t ���� ��ü�� ���� �������� �̵��� �Ÿ� x => ������� �ӵ� * �ð� ( t )
	�ð� t ���� ��ü�� ���� �������� �̵��� �Ÿ� y => 1/2 * ���ӵ� * �ð�^2
	 */

	// ���� ���� �
	m_Pos.x += m_DirX * m_SpeedX * DeltaTime;

	// ���� ���� �
	if (m_PhysicsSimulate && !m_IsGround)
	{
		m_FallTime += DeltaTime * 10.f;

		// �̰� �����ϱ�
		// �ö󰡴� ������ + 
		// �������� ������ - 
		float Velocity = 0.f;
		if (m_Jump)
		{
			Velocity = m_JumpVelocity * m_FallTime;
		}

		// ���� ���� , ���� �ö󰡴� �� + �Ʒ��� �߷��� �ۿ��ϴ� �� 2���� ���������� ������ �����̴�.
		m_Pos.y = (m_FallStartY + (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime));
	}

	m_LifeTime -= DeltaTime;

	if (m_LifeTime < 0.f)
	{
		Destroy();
	}
}

void CUIDamageFont::Render()
{
	int TempNum = m_DamageNumber;

	std::stack<int> stkNums;

	if (TempNum == 0)
	{
		stkNums.push(0);
	}
	else
	{
		while (TempNum > 0)
		{
			stkNums.push(TempNum % 10);
			TempNum /= 10;
		}
	}

	m_vecNumber.clear();

	while (!stkNums.empty())
	{
		m_vecNumber.push_back(stkNums.top());
		stkNums.pop();
	}

	int CurrentNum = 0, Frame = 0;

	for (int i = 0; i < m_vecNumber.size(); i++)
	{
		Frame = 0;
		CurrentNum = m_vecNumber[i];

		if (m_Info.m_Texture)
		{
			switch (m_Info.m_Texture->GetImageType())
			{
			case Image_Type::Atlas :
				{
				AnimationFrameData FrameData = m_Info.m_vecFrameData[CurrentNum];

				Vector2 StartUV = (FrameData.Start) / Vector2((float)m_Info.m_Texture->GetWidth(),
					(float)m_Info.m_Texture->GetHeight());
				Vector2 EndUV = (FrameData.Start + FrameData.Size) / Vector2((float)m_Info.m_Texture->GetWidth(),
					(float)m_Info.m_Texture->GetHeight());

				m_CBuffer->SetStartUV(StartUV);
				m_CBuffer->SetEndUV(EndUV);

				}
				break;
			case Image_Type::Frame:
				{
				AnimationFrameData FrameData = m_Info.m_vecFrameData[CurrentNum];

				Vector2 StartUV = Vector2(0.f, 0.f);
				Vector2 EndUV = Vector2(1.f, 1.f);

				m_CBuffer->SetStartUV(StartUV);
				m_CBuffer->SetEndUV(EndUV);

				Frame = CurrentNum;
				}
				break;
			}
			m_Info.m_Texture->SetShader(0, (int)Buffer_Shader_Type::Pixel, Frame);
		}

		m_Tint = m_Info.m_Tint;
		m_RenderPos.x += (i * m_Size.x);

		CUIWidget::Render();

	}

}
