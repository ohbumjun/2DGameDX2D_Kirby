#include "Input.h"
#include "Device.h"
#include "Engine.h"
#include "Component/CameraComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneCollision.h"
#include "Scene/CameraManager.h"

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_hInst(nullptr),
	m_hWnd(nullptr),
	m_Input(nullptr),
	m_Keyboard(nullptr),
	m_Mouse(nullptr),
	m_KeyArray{},
	m_LMouseDown(false),
	m_LMouseClick(false),
	m_RMouseClick(false)
{
	m_vecKeyState.resize(256);

	for (int i = 0; i < 256; ++i)
	{
		m_vecKeyState[i].Key = static_cast<unsigned char>(i);
	}
}

CInput::~CInput()
{
	auto iter    = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_RELEASE(m_Keyboard);
	SAFE_RELEASE(m_Mouse);
	SAFE_RELEASE(m_Input);
}

bool CInput::CreateKey(const std::string& Name, unsigned char Key)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (Info)
		return false;

	Info = new KeyInfo;

	Info->Name = Name;

	unsigned char ConvertkeyValue = ConvertKey(Key);

	Info->State.Key = ConvertkeyValue;

	m_mapKeyInfo.insert(std::make_pair(Name, Info));

	bool Add = false;

	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecAddKey[i] == ConvertkeyValue)
		{
			Add = true;
			break;
		}
	}

	if (!Add)
		m_vecAddKey.push_back(ConvertkeyValue);

	return true;
}

bool CInput::SetCtrlKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Ctrl = State;

	return true;
}

bool CInput::SetAltKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Alt = State;

	return true;
}

bool CInput::SetShiftKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Shift = State;

	return true;
}

KeyInfo* CInput::FindKeyInfo(const std::string& Name)
{
	auto iter = m_mapKeyInfo.find(Name);

	if (iter == m_mapKeyInfo.end())
		return nullptr;

	return iter->second;
}

bool CInput::InitWindow()
{
	return true;
}

bool CInput::InitDirectInput()
{
	if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, nullptr)))
		return false;

	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr)))
		return false;

	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	return true;
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hInst = hInst;
	m_hWnd  = hWnd;

	m_InputType = Input_Type::Direct;


	HRESULT result = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8,
	                                    (void**)&m_Input, nullptr);

	if (FAILED(result))
		m_InputType = Input_Type::Window;

	switch (m_InputType)
	{
	case Input_Type::Direct:
		if (!InitDirectInput())
			return false;
		break;
	case Input_Type::Window:
		if (!InitWindow())
			return false;
		break;
	}


	return true;
}

void CInput::Update(float DeltaTime)
{
	if (m_InputType == Input_Type::Direct)
	{
		ReadDirectInputKeyboard();
		ReadDirectInputMouse();
	}

	// 마우스 입력처리를 한다. --> Mouse 위치를 잡아준다
	UpdateMouse(DeltaTime);

	// UI와 마우스 충돌 여부를 조사한다.
	m_CollisionWidget = CSceneManager::GetInst()->GetScene()->GetCollision()->CollisionWidget();

	// 키 상태를 업데이트 해준다.
	UpdateKeyState();

	// 키보드 키 입력처리를 한다.
	UpdateKeyInfo(DeltaTime);

}

void CInput::ReadDirectInputKeyboard()
{
	HRESULT result = m_Keyboard->GetDeviceState(256, m_KeyArray);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_Keyboard->Acquire();
	}
}

void CInput::ReadDirectInputMouse()
{
	HRESULT result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_Mouse->Acquire();
	}
}

void CInput::UpdateMouse(float DeltaTime)
{
	// IMGUI Window 창 위에 있으면 인식 x
	// if (ImGui::GetIO().WantCaptureMouse)
	//	return;

	POINT MouseWindowPos;

	GetCursorPos(&MouseWindowPos);
	ScreenToClient(m_hWnd, &MouseWindowPos);

	Vector2 Ratio = CDevice::GetInst()->GetViewportAspectRatio();

	Vector2 MousePos = Vector2(MouseWindowPos.x * Ratio.x, MouseWindowPos.y * Ratio.y);

	// 위 아래 전환 ( 아래 기준 좌표 )
	MousePos.y = CDevice::GetInst()->GetResolution().Height - MousePos.y;

	m_MouseMove = MousePos - m_MousePos;

	m_MousePos         = MousePos;
	m_MouseWorldPos = m_MousePos;

	// 2D 공간에서는 월드 공간에서의 마우스 좌표를 구한다
	// 이를 위해서는 카메라의 Left Bottom ( World상의 Pos 에 해당)을 더해준다
	// m_MousePos 는 현재 스크린 상의 Mouse Pos 정보를 담고 있기 때문이다.
	if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		m_MouseWorldPos += Camera->GetLeftBottom();
	}

	// Mouse State Update 하기 
	if (m_MouseState.rgbButtons[0] & 0x80)
	{
		if (!m_LMouseDown && !m_LMousePush)
		{
			m_LMouseDown = true;
			m_LMousePush = true;
			m_LMouseUp   = false;
		}
		else
		{
			m_LMouseDown = false;
		}
	}
	else if (m_LMousePush)
	{
		m_LMouseDown = false;
		m_LMousePush = false;
		m_LMouseUp   = true;
	}
	else if (m_LMouseUp)
	{
		m_LMouseUp = false;
	}
}

void CInput::UpdateKeyState()
{
	switch (m_InputType)
	{
	case Input_Type::Direct:
		if (m_KeyArray[DIK_LCONTROL] & 0x80)
			m_Ctrl = true;

		else
			m_Ctrl = false;

		if (m_KeyArray[DIK_LALT] & 0x80)
			m_Alt = true;

		else
			m_Alt = false;

		if (m_KeyArray[DIK_LSHIFT] & 0x80)
			m_Shift = true;

		else
			m_Shift = false;

		if (m_MouseState.rgbButtons[0] & 0x80)
			m_LMouseClick = true;
		else
			m_LMouseClick = false;

		if (m_MouseState.rgbButtons[1] & 0x80)
			m_RMouseClick = true;
		else
			m_RMouseClick = false;

		break;
	case Input_Type::Window:
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
			m_Ctrl = true;

		else
			m_Ctrl = false;

		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_Alt = true;

		else
			m_Alt = false;

		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			m_Shift = true;

		else
			m_Shift = false;
		break;
	}

	// 등록된 키를 반복하며 해당 키가 눌러졌는지를 판단한다.
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		unsigned char Key = m_vecAddKey[i];

		bool KeyPush = false;

		// 즉, 원리는 이러하다
		// 먼저 마우스 왼쪽 오른쪽 버튼이 눌러지면
		// 무조건 눌러진 것으로 인식은 시킨다
		// 그래야만, Widget 들 입장에서는 마우스 클릭 여부를 무조건적으로
		// 알아낼 수 있기 때문이다.

		// 반면, 몇몇 Object 들은 ,아니, 주로 Player 일테지만
		// 마우스 왼쪽 버튼 콜백에, Player 클래스 내의 함수를 등록해놓는 경우들이 있다
		// 이 경우에는, 아무리 마우스가 눌린다고 한들
		// 해당 콜백은 동작하지 않게 해야 한다
		// 이를 위해서 m_CollisionWidget 을 두어,
		// 위젯과 충돌했다면, 설령 마우스 클릭을 했더라도
		// KeyState 입장에서는 눌러진 것으로 인식하지 않게
		// 세팅하고자 하는 것이다.

		switch (m_InputType)
		{
		case Input_Type::Direct:
			switch (Key)
			{
			case DIK_MOUSELBUTTON:
				// Widget 과 충돌이 일어나지 않았을 때에만 => !m_CollisionWidget
				if ((m_MouseState.rgbButtons[0] & 0x80) && !m_CollisionWidget)
				{
					m_LMouseClick = true;
					KeyPush = true;
				}
				break;
			case DIK_MOUSERBUTTON:
				if ((m_MouseState.rgbButtons[1] & 0x80) && !m_CollisionWidget)
				{
					m_RMouseClick = true;
					KeyPush = true;
				}
				break;
			case DIK_MOUSEWHEEL:
				break;
			default: // 키보드 키를 알아볼 경우
				if (m_KeyArray[Key] & 0x80)
				{
					KeyPush = true;
				}
				break;
			}
			break;
		case Input_Type::Window:
			if (GetAsyncKeyState(Key) & 0x8000)
			{
				KeyPush = true;
			}
			break;
		}


		if (KeyPush)
		{
			if (!m_vecKeyState[Key].State[KeyState_Down] &&
				!m_vecKeyState[Key].State[KeyState_Push])
			{
				m_vecKeyState[Key].State[KeyState_Down] = true;
				m_vecKeyState[Key].State[KeyState_Push] = true;
			}

			else
				m_vecKeyState[Key].State[KeyState_Down] = false;
		}

		else if (m_vecKeyState[Key].State[KeyState_Push])
		{
			m_vecKeyState[Key].State[KeyState_Up]   = true;
			m_vecKeyState[Key].State[KeyState_Down] = false;
			m_vecKeyState[Key].State[KeyState_Push] = false;
		}

		else if (m_vecKeyState[Key].State[KeyState_Up])
		{
			m_vecKeyState[Key].State[KeyState_Up] = false;
		}
	}
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
	auto iter    = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();


	for (; iter != iterEnd; ++iter)
	{
		unsigned char Key = iter->second->State.Key;

		// Imgui Window 위에 올라올 시의 Mouse Event는 Scene이나 Project에는 적용안되게 세팅한다.
		bool ImGuiMouseHovered = ImGui::GetIO().WantCaptureMouse;

		if (m_vecKeyState[Key].State[KeyState_Down] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Down])
			{
				if (!ImGui::GetIO().WantCaptureMouse)
					iter->second->Callback[KeyState_Down](DeltaTime);
			}
		}

		if (m_vecKeyState[Key].State[KeyState_Push] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Push])
			{
				if (!ImGui::GetIO().WantCaptureMouse)
					iter->second->Callback[KeyState_Push](DeltaTime);
			}
		}

		if (m_vecKeyState[Key].State[KeyState_Up] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Up])
			{
				if (!ImGui::GetIO().WantCaptureMouse)
					iter->second->Callback[KeyState_Up](DeltaTime);
			}
		}
	}
}

void CInput::ClearCallback()
{
	auto iter    = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < KeyState_Max; ++i)
		{
			iter->second->Callback[i] = nullptr;
		}
	}
}

unsigned char CInput::ConvertKey(unsigned char Key)
{
	if (m_InputType == Input_Type::Direct)
	{
		switch (Key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
		//case VK_RETURN:		
			return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;
	}

	return Key;
}
