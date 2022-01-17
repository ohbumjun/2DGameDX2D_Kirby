#pragma once

#include "UIWidget.h"
#include "../Resource/Shader/ProgressBarConstantBuffer.h"

class CUIProgressBar :
	public CUIWidget
{
	friend class CUIWindow;

protected:
	CUIProgressBar();
	CUIProgressBar(const CUIProgressBar& widget);
	virtual ~CUIProgressBar();

protected:
	WidgetImageInfo	m_Info;
	std::function<void()>	m_MouseOnCallback;
	float			m_Percent;
	ProgressBar_Dir	m_Dir;
	CProgressbarConstantBuffer* m_ProgressCBuffer;
	bool			m_StartMouseOn;

public:
	float GetPercent()	const
	{
		return m_Percent;
	}

public:
	void SetPercent(float Percent)
	{
		m_Percent = Percent;

		if (m_Percent > 1.f)
			m_Percent = 1.f;

		else if (m_Percent < 0.f)
			m_Percent = 0.f;

		m_ProgressCBuffer->SetPercent(m_Percent);
	}

	void SetDir(ProgressBar_Dir Dir)
	{
		m_Dir = Dir;
		m_ProgressCBuffer->SetDir(m_Dir);
	}


	virtual void Enable(bool bEnable)
	{
		CUIWidget::Enable(bEnable);
	}

	void SetTexture(Button_State State, CTexture* Texture)
	{
		m_Info.m_Texture = Texture;

		SetUseTexture(true);
	}

	bool SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	void SetTextureTint(const Vector4& Tint);
	void SetTextureTint(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void AddFrameData(const Vector2& Start, const Vector2& Size);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CUIProgressBar* Clone();

public:
	template <typename T>
	void SetMouseOnCallback(T* Obj, void (T::* Func)())
	{
		m_MouseOnCallback = std::bind(Func, Obj);
	}

	void SetMouseOnCallback(void (*Func)())
	{
		m_MouseOnCallback = std::bind(Func);
	}
};

