#include "Engine.h"
#include "Device.h"
#include "IMGUIManager.h"
#include "Input.h"
#include "PathManager.h"
#include "Timer.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Collision/CollisionManager.h"
#include "Resource/Shader/GlobalConstantBuffer.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;

CEngine::CEngine() :
	m_ClearColor{1.f, 1.f, 1.f, 1.f},
	m_Timer(nullptr),
	m_Start(false),
	m_Play(true),
	m_Space(Engine_Space::Space2D),
	m_MouseState(Mouse_State::Normal),
	m_ShowCursor(true),
	m_GlobalAccTime(0.f),
	m_GlobalCBuffer(nullptr)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(100);

	//m_ClearColor[1] = 1.f;
}

CEngine::~CEngine()
{
	CSceneManager::DestroyInst();

	CInput::DestroyInst();

	CRenderManager::DestroyInst();

	CPathManager::DestroyInst();

	CResourceManager::DestroyInst();

	CIMGUIManager::DestroyInst();

	CCollisionManager::DestroyInst();

	CDevice::DestroyInst();

	SAFE_DELETE(m_Timer);

	SAFE_DELETE(m_GlobalCBuffer);
}

void CEngine::SetMouseState(Mouse_State State)
{
	if (m_MouseWidget[(int)m_MouseState])
	{
		// 초기화
	}
	m_MouseState = State;
}

bool CEngine::Init(HINSTANCE    hInst, const TCHAR* Name, unsigned int Width,
				   unsigned int Height, int         IconID, bool       WindowMode)
{
	m_hInst = hInst;

	m_RS.Width  = Width;
	m_RS.Height = Height;

	Register(Name, IconID);
	Create(Name);

	return Init(hInst, m_hWnd, Width, Height, WindowMode);
}

bool CEngine::Init(HINSTANCE    hInst, HWND         hWnd,
                   unsigned int Width, unsigned int Height, bool WindowMode)
{
	m_hInst = hInst;
	m_hWnd  = hWnd;

	m_RS.Width  = Width;
	m_RS.Height = Height;

	m_Timer = new CTimer;

	// Device 초기화
	if (!CDevice::GetInst()->Init(m_hWnd, Width, Height, WindowMode))
		return false;

	if (!CPathManager::GetInst()->Init())
		return false;

	// 리소스 관리자 초기화
	if (!CResourceManager::GetInst()->Init())
		return false;

	// 입력 관리자 초기화
	if (!CInput::GetInst()->Init(m_hInst, m_hWnd))
		return false;


	// IMGUI 관리자 초기화
	if (!CIMGUIManager::GetInst()->Init(m_hWnd))
		return false;


	// 렌더링 관리자 초기화
	if (!CRenderManager::GetInst()->Init())
		return false;

	// 장면 관리자 초기화
	if (!CSceneManager::GetInst()->Init())
		return false;

	// 충돌 관리자 초기화
	if (!CCollisionManager::GetInst()->Init())
		return false;

	// 상수 버퍼
	m_GlobalCBuffer = new CGlobalConstantBuffer;

	if (!m_GlobalCBuffer->Init())
		return false;

	Vector2 RS = Vector2((float)m_RS.Width, (float)m_RS.Height);
	m_GlobalCBuffer->SetResolution(RS);

	// GlobalNoiseTexture 세팅
	CResourceManager::GetInst()->LoadTexture("NoiseTexture", TEXT("noise_01.png"));
	m_GlobalNoiseTexture = CResourceManager::GetInst()->FindTexture("NoiseTexture");
	m_GlobalNoiseTexture->SetShader(100, (int)Buffer_Shader_Type::All, 0);
	m_GlobalCBuffer->SetNoiseResolution(Vector2((float)m_GlobalNoiseTexture->GetWidth(),
		(float)m_GlobalNoiseTexture->GetHeight()));

	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	while (m_Loop)
	{
		// GetMessage는 메세지가 없을 경우 다른일을 할 수 없다.
		// 메세지가 올때까지 대기하고 있는 시간을 윈도우의 데드타임이라고 한다.
		// 실제로 메세지가 있는 시간보다 없는 시간이 훨씬 길다.
		// 그래서 게임은 윈도우의 데드타임동안 게임이 동작될 수 있게 제작한다.
		// PeekMessage는 메세지큐에서 메세지를 얻어온다.
		// 그런데 만약 메세지가 없다면 false를 리턴하면서 바로 빠져나오고
		// 메세지가 있다면 true를 리턴하면서 해당 메세지를 꺼내온다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// WM_KEYDOWN 이라는 메세지가 있다. 이는 키보드 키를 눌렀을때 발생되는 메세지
			// 이다. TranslateMessage 함수에서는 메세지를 전달해주면 WM_KEYDOWN 인지를
			// 판단해주고 눌려진 키가 문자 키인지를 판단해서 일반 무자 키라면 WM_CHAR라는
			// 메시지를 만들어서 메세지 큐에 추가해주게 된다.
			TranslateMessage(&msg);

			// DispatchMessage 함수는 메세지를 윈도우 프로시저로 보내준다.
			DispatchMessage(&msg);
		}

		// 윈도우가 데드타임일 경우 여기로 들어오게 된다.
		else
		{
			Logic();
		}
	}

	return static_cast<int>(msg.wParam);
}

void CEngine::Logic()
{
	if (!m_Start)
	{
		m_Start = true;
		CSceneManager::GetInst()->Start();
	}

	m_Timer->Update();

	float DeltaTime = m_Timer->GetDeltaTime();

	if (!m_Play)
		DeltaTime = 0.f;

	m_GlobalAccTime += DeltaTime;

	m_GlobalCBuffer->SetDeltaTime(DeltaTime);
	m_GlobalCBuffer->SetAccTime(m_GlobalAccTime);

	m_GlobalCBuffer->UpdateCBuffer();

	CInput::GetInst()->Update(DeltaTime);

	CIMGUIManager::GetInst()->Update(DeltaTime);

	if (Update(DeltaTime))
		return;

	if (PostUpdate(DeltaTime))
		return;

	Render(DeltaTime);
}

void CEngine::Exit()
{
	DestroyWindow(m_hWnd);
}

bool CEngine::Update(float DeltaTime)
{
	if (CSceneManager::GetInst()->Update(DeltaTime))
		return true;

	CResourceManager::GetInst()->Update();

	if (m_MouseWidget[(int)m_MouseState])
	{
		// 마우스 보임 여부 결정학
		Vector2 MousePos = CInput::GetInst()->GetMousePos();

		// 마우스가 윈도우창을 벗어났다면, 기본 cursor가 보이게 세팅한다
		// 그렇지 않고, Window 창 안에 있다면 기본 cursor는 안보이게 하고,
		// Mouse Widget 만 보이게 세팅한다.
		if (MousePos.x < 0.f || MousePos.x > m_RS.Width || MousePos.y < 0.f || MousePos.y > m_RS.Height)
		{
			if (!m_ShowCursor)
			{
				ShowCursor(true);
				m_ShowCursor = true;
			}
		}
		else // 범위 안에 있다면
		{
			if (m_ShowCursor)
			{
				ShowCursor(false);
				m_ShowCursor = false;
			}
		}

		// Mouse Widget의 위치를 조정한다.
		Vector2 WindowSize = m_MouseWidget[(int)m_MouseState]->GetWindowSize();
		MousePos.y -= WindowSize.y;
		m_MouseWidget[(int)m_MouseState]->SetPos(MousePos);

		m_MouseWidget[(int)m_MouseState]->Update(DeltaTime);
	}

	return false;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	if (CSceneManager::GetInst()->PostUpdate(DeltaTime))
		return true;

	if (m_MouseWidget[(int)m_MouseState])
	{
		m_MouseWidget[(int)m_MouseState]->PostUpdate(DeltaTime);
	}

	return false;
}

bool CEngine::Render(float DeltaTime)
{
	CDevice::GetInst()->RenderStart();
	CDevice::GetInst()->ClearRenderTarget(m_ClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	/*CMesh* Mesh = CResourceManager::GetInst()->FindMesh("SpriteMesh");
	CShader* Shader = CResourceManager::GetInst()->FindShader("ColorMeshShader");

	Shader->SetShader();

	Mesh->Render();*/
	CRenderManager::GetInst()->Render();

	CIMGUIManager::GetInst()->Render();

	CDevice::GetInst()->Flip();

	return true;
}

ATOM CEngine::Register(const TCHAR* Name, int IconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	// 작업영역이 바뀔때 다시 그리게끔 해준다. 가로와 세로 모두 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지가 큐에 들어왔을때 해당 메세지를 꺼내오고 꺼내온 메세지를 인자로 전달해서
	// 호출해줄 함수를 지정한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra  = 0;
	wcex.cbWndExtra  = 0;

	// Window Instance를 지정한다.
	wcex.hInstance = m_hInst;

	// 실행파일이 만들어질때 사용할 아이콘 이미지를 지정한다.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));


	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = nullptr;
	// 유니코드 문자열을 사용할때 L"" 을 붙여서 사용을 한다.
	// TEXT 매크로는 "" 앞에 L 을 붙여서 L"" 로 만들어준다.
	// 유니코드일때는 이렇게 해주고 multibyte  일때는 그냥 "" 로 만들어준다.
	wcex.lpszClassName = Name;
	wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IconID));

	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR* Name)
{
	m_hWnd = CreateWindowW(Name, Name, WS_OVERLAPPEDWINDOW,
	                       0, 0, m_RS.Width, m_RS.Height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	// 클라이언트 영역을 1280, 720으로 만들어준다.
	// RECT : 사각형을 표현하는 구조체이다.
	// left, top, right, bottom 4개의 값으로 구성되어 있다.
	RECT rc = {0, 0, static_cast<LONG>(m_RS.Width), static_cast<LONG>(m_RS.Height)};

	// 1번인자 : 클라이언트 영역의 크기를 전달한다.
	// 이 함수가 완료되면 rc에는 실제 클라이언트 영역이 원하는 크기 될 수 있는
	// 전체 윈도우의 left, top, right, bottom 정보가 들어오게 된다.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 위에서 얻어온 Rect를 이용해서 윈도우 크기를 지정한다.
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
	             rc.bottom - rc.top,
	             SWP_NOZORDER);

	// SW_SHOW : 윈도우창을 화면에 보여준다.
	// SW_HIDE : 창을 만들긴 하지만 화면에 보여주는건 아니다.
	ShowWindow(m_hWnd, SW_SHOW);

	// UpdateWindow 함수는 갱신할 부분이 있다면 갱신하라는 명령을 내려주는 함수이다.
	UpdateWindow(m_hWnd);

	return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return 1;

	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;

			// HDC : 화면에 출력하기 위한 그리기 도구이다.
			HDC hdc = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
