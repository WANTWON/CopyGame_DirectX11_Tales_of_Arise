#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
public:
	enum EVENTTYPE { EVENT_SOUND, EVENT_EFFECT, EVENT_COLLIDER, EVENT_STATE, EVENT_END };

public:
	typedef struct tagAnimationEvent
	{
		_bool isPlay = false;
		_float fStartTime;
		_float fEndTime;
		EVENTTYPE eType;
	}ANIMEVENT;

private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(class CModel* pModel, aiAnimation* pAIAnimation, _bool bLoop);
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte, _bool bLoop);
	void Invalidate_TransformationMatrix(_float fTimeDelta, vector<EVENTTYPE>* pEventTypeVector = nullptr);

public:
	const char* Get_Name(void) const { return m_szName; }
	vector<class CChannel*> Get_Channels(void) const { return m_Channels; }
	_bool Get_Finished(void) const { return m_isFinished; }
	_float Get_Duration(void) const { return m_fDuration; }
	_float Get_TickPerSecond(void) const { return m_fTickPerSecond; }
	vector<_float> Get_TickPerSeconds(void) const { return m_TickPerSeconds; }
	vector<_float> Get_ChangeTickTimes(void) const { return m_ChangeTickTimes; }
	_float Get_CurrentTime(void) const { return m_fCurrentTime; }
	vector<ANIMEVENT> Get_AnimEvents(void) const { return m_vecAnimEvent; }

	void Set_CurrentTime(_float fCurrentTime) { m_fCurrentTime = fCurrentTime; }
	void Add_EventCall(ANIMEVENT eEvent) { m_vecAnimEvent.push_back(eEvent); }
	void Add_TickPerSecond(_float fTime, _float fTick);
	void Fix_TickPerSecond(_int iTimeChoice, _float fAnimationTickTime, _float fAnimationTick);
	void Delete_TickPerSecond(_int iTimeChoice);
	void Add_Event(ANIMEVENT tEvent) { m_vecAnimEvent.push_back(tEvent); }
	void Fix_Event(_int iIndex, ANIMEVENT tEvent) { memcpy(&m_vecAnimEvent[iIndex], &tEvent, sizeof(ANIMEVENT)); }
	void Delete_Event(_int iIndex);

public:
	void Change_Duration(_float fDuration);

	void Reset_NonLoop(void);
	void Reset(void);

private:
	char m_szName[MAX_PATH] = "";

	_float m_fDuration = 0.f;

	_float m_fTickPerSecond = 0.f;
	_float m_fOriTickPerSecond = 0.f;
	_float m_fCurrentTime = 0.f;
	
	vector<_float> m_TickPerSeconds;
	vector<_float> m_ChangeTickTimes;
	_int m_iTickPerSecondIndex = 0;

	_uint m_iNumChannels = 0;
	vector<class CChannel*> m_Channels;

	_bool m_isFinished = false;
	_bool m_isLoop = true;

	vector<ANIMEVENT> m_vecAnimEvent;

public:
	static CAnimation* Create(class CModel* pModel, aiAnimation* pAIAnimation, _bool bLoop = true);
	static CAnimation* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel, HANDLE hAddFile, _ulong* pdwAddByte, _bool bLoop = true);
	virtual void Free(void) override;
};

END