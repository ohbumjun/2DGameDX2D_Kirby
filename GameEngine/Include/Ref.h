#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	virtual ~CRef();

protected:
	std::string m_Name;
	int         m_RefCount;
	bool        m_Enable;
	bool        m_Active;
	size_t      m_TypeID;
	std::function<void(CRef* DeleteObject)> m_DeleteCallback;

public:
	virtual void Enable(bool bEnable)
	{
		m_Enable = bEnable;
	}

	virtual void Destroy()
	{
		m_Active = false;
	}

	bool IsActive() const
	{
		return m_Active;
	}

	bool IsEnable() const
	{
		return m_Enable;
	}

	void AddRef()
	{
		++m_RefCount;
	}

	int GetRefCount() const
	{
		return m_RefCount;
	}

	int Release()
	{
		--m_RefCount;

		if (m_RefCount <= 0)
		{
			if (m_DeleteCallback)
				m_DeleteCallback(this);
			else 
				delete this;
			return 0;
		}

		return m_RefCount;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	template <typename T>
	bool CheckType()
	{
		return m_TypeID == typeid(T).hash_code();
	}
	size_t GetTypeID() const
	{
		return m_TypeID;
	}
	template<typename T>
	void SetDeleteCallback(T* Obj, void(T::*Func)(CRef* Object))
	{
		m_DeleteCallback = std::bind(Func, Obj, std::placeholders::_1);
	}
public :
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};
