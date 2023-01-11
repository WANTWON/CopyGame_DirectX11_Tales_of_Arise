#pragma once


#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
public:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(unsigned int iLevelIndex, class CLevel* pNewLevel);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);

public:
	class CLevel* Get_CurrentLevel() { return m_pCurrentLevel; }
	_uint Get_CurrentLevelIndex() { return m_iLevelIndex; }
	_uint Get_DestinationLevelIndex() { return m_iDestinationIndex; }
	void Set_DestinationLevel(_uint LevelIndex) { m_iDestinationIndex = LevelIndex; }
	_uint Get_PastLevelIndex() { return m_iPastLevelIndex; }

private:
	class CLevel*				m_pCurrentLevel = nullptr;
	_uint						m_iLevelIndex;
	_uint						m_iDestinationIndex;
	_uint						m_iPastLevelIndex;

public:
	virtual void Free() override;
};

END

