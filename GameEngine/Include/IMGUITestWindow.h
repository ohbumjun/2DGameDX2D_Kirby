#pragma once
#include "IMGUIWindow.h"

class CIMGUITestWindow :
	public CIMGUIWindow
{
public:
	CIMGUITestWindow();
	virtual ~CIMGUITestWindow() override;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;


public:
	void ClickButton1();
	void ClickButton2();
};
