#pragma once

#include "../../GameInfo.h"

class CParticleManager
{
	friend class CResourceManager;

private :
	std::unordered_map<std::string, CSharedPtr<class CParticle>> m_mapParticle;
public :
	bool Init();
	bool CreateParticle(const std::string& Name);
	CParticle* FindParticle(const std::string& Name);
	void ReleaseParticle(const std::string& Name);

	DECLARE_SINGLE(CParticleManager);
};

