#pragma once
#include "IMGUIWidget.h"

class CIMGUIButton :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIButton();
	virtual ~CIMGUIButton() override;

protected:
	std::function<void()> m_ClickCallback;
	bool                  m_Click;

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	template <typename T>
	void SetClickCallback(T* Obj, void (T::*Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}
};
