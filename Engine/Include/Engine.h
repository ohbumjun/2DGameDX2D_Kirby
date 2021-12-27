#pragma once

#include "GameInfo.h"

class CEngine
{
private:
	HINSTANCE     m_hInst;
	HWND          m_hWnd;
	Resolution    m_RS;
	static bool   m_Loop;
	float         m_ClearColor[4];
	class CTimer* m_Timer;
	bool          m_Start;
	bool m_Play;

public:
	HWND GetWindowHandle() const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance() const
	{
		return m_hInst;
	}

	Resolution GetResolution() const
	{
		return m_RS;
	}
	bool IsPlay() const
	{
		return m_Play;
	}
	void SetPlay(bool Play)
	{
		m_Play = Play;
	}

public:
	bool Init(HINSTANCE    hInst, const TCHAR* Name, unsigned int Width,
	          unsigned int Height, int         IconID,
	          bool         WindowMode = true);
	bool Init(HINSTANCE    hInst, HWND  hWnd, unsigned int Width,
	          unsigned int Height, bool WindowMode = true);
	int  Run();
	void Logic();

private:
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(float DeltaTime);

private:
	ATOM                    Register(const TCHAR* Name, int IconID);
	BOOL                    Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine)
};
