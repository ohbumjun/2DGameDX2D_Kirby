#pragma once
#include "IMGUIWidget.h"

class CIMGUILabel :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUILabel();
	virtual ~CIMGUILabel() override;

protected:
	ImVec2 m_Align;

public:
	void SetAlign(float x, float y)
	{
		m_Align = ImVec2(x, y);
	}

public:
	virtual bool Init() override;
	virtual void Render() override;
};
