#pragma once

#include "GameInfo.h"
#include "Timer.h"
#include "UI/UIWindow.h"

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
	int m_ShowCursor;
	Engine_Space m_Space;
	Mouse_State m_MouseState;
	CSharedPtr<class CUIWindow> m_MouseWidget[(int)Mouse_State::Max];
	class CGlobalConstantBuffer* m_GlobalCBuffer;
	float m_GlobalAccTime;
	CSharedPtr<class CTexture> m_GlobalNoiseTexture;

public:
	CUIWindow* GetMouseWidget() const
	{
		return m_MouseWidget[(int)m_MouseState];
	}
	Engine_Space GetEngineSpace() const
	{
		return m_Space;
	}
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
	float GetFPS() const
	{
		return m_Timer->GetFPS();
	}
public :
	void SetMouseState(Mouse_State State);

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
	void Exit();

private:
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(float DeltaTime);

private:
	ATOM                    Register(const TCHAR* Name, int IconID);
	BOOL                    Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine)
public :
	template<typename T>
	T* CreateMouseUIWindow(Mouse_State State, const std::string& Name)
	{
		T* MouseWindow = new T;
		MouseWindow->SetName(Name);
		if (!MouseWindow->Init())
		{
			SAFE_DELETE(MouseWindow);
			return nullptr;
		}
		m_MouseWidget[(int)State] = MouseWindow;
		return MouseWindow;
	}
};
