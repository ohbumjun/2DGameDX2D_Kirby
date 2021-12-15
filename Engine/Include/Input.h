#pragma once

#include "GameInfo.h"

#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe

struct keyState
{
	unsigned char	Key;
	bool	State[KeyState_Max];

	keyState() :
		State{}
	{
	}
};

struct KeyInfo
{
	std::string	Name;
	keyState	State;
	std::function<void(float)>	Callback[KeyState_Max];

	bool	Ctrl;
	bool	Alt;
	bool	Shift;

	KeyInfo() :
		Ctrl(false),
		Alt(false),
		Shift(false)
	{
	}
};


class CInput
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	IDirectInput8* m_Input;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;
	unsigned char	m_KeyArray[256];
	DIMOUSESTATE	m_MouseState;
	Input_Type	m_InputType;

private:
	std::unordered_map<std::string, KeyInfo*>	m_mapKeyInfo;
	std::vector<keyState>	m_vecKeyState;
	std::vector<unsigned char>	m_vecAddKey;
	bool	m_Ctrl;
	bool	m_Alt;
	bool	m_Shift;
	Vector2		m_MousePos;
	Vector2		m_MouseMove;
	bool m_LMouseDown;
	bool m_LMousePush;
	bool m_LMouseUp;

public:
	Vector2 GetMousePos()	const
	{
		return m_MousePos;
	}

	Vector2 GetMouseMove()	const
	{
		return m_MouseMove;
	}
	bool GetLMouseDown() const
	{
		return m_LMouseDown;
	}
	bool GetLMousePush() const
	{
		return m_LMousePush;
	}
	bool GetLMouseUp() const
	{
		return m_LMouseUp;
	}

public:
	bool CreateKey(const std::string& Name, unsigned char Key);
	bool SetCtrlKey(const std::string& Name, bool State);
	bool SetAltKey(const std::string& Name, bool State);
	bool SetShiftKey(const std::string& Name, bool State);

private:
	KeyInfo* FindKeyInfo(const std::string& Name);

private:
	bool InitWindow();
	bool InitDirectInput();

public:
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float DeltaTime);


private:
	void ReadDirectInputKeyboard();
	void ReadDirectInputMouse();
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float DeltaTime);



public:
	void ClearCallback();
	template <typename T>
	void SetKeyCallback(const std::string& Name, Key_State State, T* Obj, void(T::* Func)(float))
	{
		KeyInfo*	Info = FindKeyInfo(Name);

		if (!Info)
			return;

		Info->Callback[State] = std::bind(Func, Obj, std::placeholders::_1);
	}

private:
	unsigned char ConvertKey(unsigned char Key);

	DECLARE_SINGLE(CInput)
};

