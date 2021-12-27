#pragma once

#include "Scene.h"

class CSceneManager
{
private:
	CScene* m_Scene;
	CScene* m_NextScene;
private :
	std::function<void (CScene* Scene, size_t SceneModeTypeID)> m_CreateSceneModeCallback;
	std::function<class CGameObject* (CScene* Scene, size_t ObjectTypeID)> m_CreateObjectCallback;
	std::function<class CComponent* (CGameObject* Object, size_t ComponentTypeID)> m_CreateComponentCallback;
	std::function<void (CComponent* Component, size_t AnimInstanceTypeID)> m_CreateAnimInstanceCallback;
public :
	void CallCreateSceneModeFunc(CScene* Scene, size_t SceneModeTypeID)
{
		if (m_CreateSceneModeCallback)
			m_CreateSceneModeCallback(Scene, SceneModeTypeID);
}
	class CGameObject* CallCreateObjectFunc(CScene* Scene, size_t ObjectTypeID)
	{
		if (m_CreateObjectCallback)
			return m_CreateObjectCallback(Scene, ObjectTypeID);
		return nullptr;
	}
	class CComponent* CallCreateComponentFunc(CGameObject* Object, size_t ComponentTypeID)
	{
		if (m_CreateComponentCallback)
			return m_CreateComponentCallback(Object, ComponentTypeID);
		return nullptr;
	}
	void CallCreateAnimInstanceFunc(CComponent* Component, size_t AnimInstanceTypeID)
	{
		if (m_CreateAnimInstanceCallback)
			m_CreateAnimInstanceCallback(Component, AnimInstanceTypeID);
	}


public:
	CScene* GetScene() const
	{
		return m_Scene;
	}
public :
	template<typename T>
	void SetCreateSceneModeCallback(T*Obj, void(T::*Func)(CScene* Scene, size_t SceneModeTypeID))
	{
		m_CreateSceneModeCallback = std::bind(Obj, Func, std::placeholders::_1, std::placeholders::_2);
	}
	template<typename T>
	void SetCreateObjectCallback(T* Obj, class CGameObject* (T::* Func)(CScene* Scene, size_t ObjectTypeID))
	{
		m_CreateObjectCallback = std::bind(Obj, Func, std::placeholders::_1, std::placeholders::_2);
	}
	template<typename T>
	void SetCreateComponentCallback(T* Obj, class CComponent* (T::* Func)(CGameObject* Object, size_t ComponentTypeID))
	{
		m_CreateComponentCallback = std::bind(Obj, Func, std::placeholders::_1, std::placeholders::_2);
	}
	template<typename T>
	void SetCreateAnimInstanceCallback(T* Obj, void(T::* Func)(CComponent* Component, size_t AnimInstanceTypeID))
	{
		m_CreateAnimInstanceCallback = std::bind(Obj, Func, std::placeholders::_1, std::placeholders::_2);
	}

public:
	void Start();
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);

public:
	template <typename T>
	bool CreateSceneMode(bool Current = true)
	{
		if (Current)
			return m_Scene->CreateSceneMode<T>();

		return m_NextScene->CreateSceneMode<T>();
	}

	DECLARE_SINGLE(CSceneManager)
};
