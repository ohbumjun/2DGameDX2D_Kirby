#pragma once
class CMonsterAttackEffectFactory
{
private :
	CMonsterAttackEffectFactory();
	~CMonsterAttackEffectFactory();
	CMonsterAttackEffectFactory(const CMonsterAttackEffectFactory& Factory) = delete;
private :
	static CMonsterAttackEffectFactory* m_Inst;


};

