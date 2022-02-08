#pragma once

#include "Material.h"
#include "../../GameInfo.h"
#include "../Shader/MaterialConstantBuffer.h"

class CMaterialManager
{
	friend class CResourceManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	std::unordered_map<std::string, CSharedPtr<CMaterial>> m_mapMaterial;
	class CMaterialConstantBuffer*                         m_CBuffer; // 왜 이걸 얘가 ? --> 그런데, 이거 공유하면 안될 것 같은데...? PaperBurn 공통 적용되버린다.

public:
	bool       Init();
	CMaterial* FindMaterial(const std::string& Name);
	void       ReleaseMaterial(const std::string& Name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& Name)
	{
		T* Material = static_cast<T*>(FindMaterial(Name));

		if (Material)
			return false;

		Material = new T;

		Material->SetConstantBuffer(m_CBuffer);

		Material->SetName(Name);

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return true;
	}
	template <typename T>
	T* CreateMaterialEmpty()
	{
		T* Material = new T;
		Material->SetConstantBuffer(m_CBuffer);
		return Material;
	}
};
