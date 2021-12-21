#pragma once

#include "../../GameInfo.h"

class CShaderManager
{
	friend class CResourceManager;

private:
	CShaderManager();
	~CShaderManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CShader>>         m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>> m_mapConstantBuffer;

public:
	bool           Init();
	class CShader* FindShader(const std::string& Name);
	void           ReleaseShader(const std::string& Name);

	bool CreateConstantBuffer(const std::string& Name, int Size, int Register,
	                          int ConstantBufferShaderType = static_cast<int>(ConstantBuffer_Shader_Type::All));
	class CConstantBuffer* FindConstantBuffer(const std::string& Name);

public:
	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		T* Shader = static_cast<T*>(FindShader(Name));

		if (Shader)
			return false;

		Shader = new T;

		if (!Shader->Init())
		{
			SAFE_RELEASE(Shader);
			return false;
		}

		m_mapShader.insert(std::make_pair(Name, Shader));

		return true;
	}
};
