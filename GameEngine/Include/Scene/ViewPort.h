#pragma once

#include "../GameInfo.h"
#include "../UI/UIWindow.h"

class CViewPort
{
	friend class CScene;
protected:
	CViewPort();
	~CViewPort();
private :
	class CScene* m_Scene;
	std::list<CSharedPtr<class CUIWindow>> m_WindowList;
public :
	bool Init();
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render();
public :
	template<typename T>
	T* FindUIWindow(const std::string& Name)
{
		auto iter = m_WindowList.begin();
		auto iterEnd = m_WindowList.end();
		for (; iter != iterEnd;++iter)
		{
			if ((*iter)->GetName() == Name)
				return (T*)(*iter);
		}
		return nullptr;
}
	template<typename T>
	T* CreateUIWindow(const std::string& Name)
{
		T* Window = new T;
		Window->SetName(Name);
		Window->SetScene(m_Scene);

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		Window->SetOwner(this);
		return Window;
}
};

