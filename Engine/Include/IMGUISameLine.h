#pragma once
#include "IMGUIWidget.h"

class CIMGUISameLine :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUISameLine();
	virtual ~CIMGUISameLine() override;

protected:
	// OffsetX �� ������ â�� �������κ��� ������� ��ġ��ŭ ����߸� �Ŀ� �׷������� ���Ѵ�.
	float m_OffsetX;

	// ���� ��ġ�κ��� ������ �Ÿ��� �ǹ��Ѵ�.
	float m_Spacing;

public:
	void SetOffsetX(float OffsetX)
	{
		m_OffsetX = OffsetX;
	}

	void SetSpacing(float Spacing)
	{
		m_Spacing = Spacing;
	}

public:
	virtual bool Init() override;
	virtual void Render() override;
};
