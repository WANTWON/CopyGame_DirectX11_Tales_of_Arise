#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(CModel* pModel, aiAnimation * pAIAnimation, _bool bLoop)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_fDuration = (_float)pAIAnimation->mDuration;
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim* pAIChannel = pAIAnimation->mChannels[i];

		CChannel* pChannel = CChannel::Create(pModel, pAIChannel);
		if (nullptr == pChannel)
			return E_FAIL;
		
		m_Channels.push_back(pChannel);
	}

	m_isLoop = bLoop;

	return S_OK;
}

HRESULT CAnimation::Initialize(HANDLE hFile, _ulong * pdwByte, CModel * pModel, HANDLE hAddFile, _ulong* pdwAddByte, _bool bLoop)
{
	if (nullptr == hFile)
		return E_FAIL;
	
	BINANIM BinAnim;

	ReadFile(hFile, &BinAnim, sizeof(BINANIM), pdwByte, nullptr);

	memcpy(&m_szName, &BinAnim.szName, sizeof(char) * MAX_PATH);
	m_fDuration = (_float)BinAnim.dbDuration;
	m_fTickPerSecond = (_float)BinAnim.dbTickPerSecond;
	m_fOriTickPerSecond = m_fTickPerSecond;

	m_iNumChannels = BinAnim.iNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(hFile, pdwByte, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	m_isLoop = bLoop;

	if (INVALID_HANDLE_VALUE != hAddFile && (pdwAddByte != 0))
	{
		_float fDuration;
		_bool isReadDuration = ReadFile(hAddFile, &fDuration, sizeof(_float), pdwAddByte, nullptr);

		if (isReadDuration && (0 == (*pdwAddByte)))
			return S_OK;

		_float fDurationRation = fDuration / m_fDuration;

		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			m_Channels[i]->Set_KeyFrames(fDurationRation);
			/*vector<KEYFRAME>KeyFrames = m_Channels[i]->Get_KeyFrames();

			for (_uint j = 0; j < KeyFrames.size(); ++j)
			{
				KEYFRAME KeyFrame;
				if (ReadFile(hAddFile, &KeyFrame, sizeof(KEYFRAME), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
					return S_OK;

				m_Channels[i]->Set_KeyFrames(j, KeyFrame); 
			}*/
		}

		m_fDuration = fDuration;

		_int TickPerSecondsSize, ChangeTimesSize, EventsSize;

		if (ReadFile(hAddFile, &TickPerSecondsSize, sizeof(_uint), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
			return S_OK;

		for (_int i = 0; i < TickPerSecondsSize; ++i)
		{
			_float fTickPerSecond;
			if (ReadFile(hAddFile, &fTickPerSecond, sizeof(_float), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
				return S_OK;

			m_TickPerSeconds.push_back(fTickPerSecond);
		}

		if (ReadFile(hAddFile, &ChangeTimesSize, sizeof(_uint), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
			return S_OK;

		for (_int i = 0; i < ChangeTimesSize; ++i)
		{
			_float fChangeTime;
			if (ReadFile(hAddFile, &fChangeTime, sizeof(_float), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
				return S_OK;

			m_ChangeTickTimes.push_back(fChangeTime);
		}

		if (ReadFile(hAddFile, &EventsSize, sizeof(_uint), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
			return S_OK;

		for (_int i = 0; i < EventsSize; ++i)
		{
			ANIMEVENT tEvent;
			if (ReadFile(hAddFile, &tEvent, sizeof(ANIMEVENT), pdwAddByte, nullptr) && (0 == (*pdwAddByte)))
				return S_OK;
			/*LOADEVENT tLoadEvent;
			ReadFile(hAddFile, &tLoadEvent, sizeof(LOADEVENT), pdwAddByte, nullptr);

			tEvent.isPlay = tLoadEvent.isPlay;
			tEvent.fStartTime = tLoadEvent.fStartTime;
			tEvent.fEndTime = tLoadEvent.fEndTime;
			tEvent.eType = tLoadEvent.eType;*/
			/*ReadFile(hAddFile, &tEvent.isPlay, sizeof(_bool), pdwAddByte, nullptr);
			ReadFile(hAddFile, &tEvent.fStartTime, sizeof(_float), pdwAddByte, nullptr);
			ReadFile(hAddFile, &tEvent.fEndTime, sizeof(_float), pdwAddByte, nullptr);
			ReadFile(hAddFile, &tEvent.eType, sizeof(EVENTTYPE), pdwAddByte, nullptr);
			*/
			//memcpy(&tEvent.szName, "", sizeof(char) * MAX_PATH);

			m_vecAnimEvent.push_back(tEvent);
		}
	}

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta, vector<EVENTTYPE>* pEventTypeVector)
{
	//애니메이션 구간별 재생 속도 변화
	_int iSize = m_ChangeTickTimes.size() - 1;
	if ((iSize >= 0) && (m_iTickPerSecondIndex <= iSize))
	{
		if (m_ChangeTickTimes[m_iTickPerSecondIndex] <= m_fCurrentTime)
		{
			m_fTickPerSecond = m_TickPerSeconds[m_iTickPerSecondIndex];
			if (m_iTickPerSecondIndex != iSize)
				++m_iTickPerSecondIndex;
		}
		else
		{
			if (m_iTickPerSecondIndex >= 1)
			{
				m_fTickPerSecond = m_TickPerSeconds[m_iTickPerSecondIndex - 1];
			}
			else
				m_fTickPerSecond = m_fOriTickPerSecond;
		}
	}
	else if (0 > iSize)
		m_fTickPerSecond = m_fOriTickPerSecond;

	m_fCurrentTime += m_fTickPerSecond * fTimeDelta;

	//애니메이션 이벤트
	if (nullptr != pEventTypeVector)
	{
		for (auto& pEvent : m_vecAnimEvent)
		{
			if (pEvent.fStartTime < m_fCurrentTime && pEvent.fEndTime > m_fCurrentTime)
				pEventTypeVector->push_back(pEvent.eType);
		}
	}

	//애니메이션 재생
	if (m_fCurrentTime >= m_fDuration)
	{
		for (auto& pChannel : m_Channels)
		{
			pChannel->Reset();
		}
		m_fCurrentTime = 0.f;
		m_iTickPerSecondIndex = 0;

		m_isFinished = true;
	}

	for (auto& pChannel : m_Channels)
	{
		if (true == m_isFinished && true == m_isLoop)
			pChannel->Reset();
		
		pChannel->Invalidate_TransformationMatrix(m_fCurrentTime);
	}

	if (true == m_isFinished && true == m_isLoop)
		m_isFinished = false;
}

void CAnimation::Add_TickPerSecond(_float fTime, _float fTick)
{
	m_ChangeTickTimes.push_back(fTime);
	m_TickPerSeconds.push_back(fTick);

	for (_int i = 1; i < m_ChangeTickTimes.size(); ++i)
	{
		for (_int j = 0; j < i; ++j)
		{
			if (m_ChangeTickTimes[i] < m_ChangeTickTimes[j])
			{
				_float fTemp = m_ChangeTickTimes[i];
				m_ChangeTickTimes[i] = m_ChangeTickTimes[j];
				m_ChangeTickTimes[j] = fTemp;

				fTemp = m_TickPerSeconds[i];
				m_TickPerSeconds[i] = m_TickPerSeconds[j];
				m_TickPerSeconds[j] = fTemp;
			}
		}
	}
}

void CAnimation::Fix_TickPerSecond(_int iTimeChoice, _float fAnimationTickTime, _float fAnimationTick)
{
	m_ChangeTickTimes[iTimeChoice] = fAnimationTickTime;
	m_TickPerSeconds[iTimeChoice] = fAnimationTick;
}

void CAnimation::Delete_TickPerSecond(_int iTimeChoice)
{
	auto iterTime = m_ChangeTickTimes.begin();
	auto iterTick = m_TickPerSeconds.begin();

	for (_int i = 0; i < iTimeChoice; ++i)
	{
		++iterTime;
		++iterTick;
	}

	m_ChangeTickTimes.erase(iterTime);
	m_TickPerSeconds.erase(iterTick);

	Reset();
}

void CAnimation::Delete_Event(_int iIndex)
{
	auto iter = m_vecAnimEvent.begin();

	for (_int i = 0; i < iIndex; ++i)
		++iter;

	m_vecAnimEvent.erase(iter);
}

void CAnimation::Change_Duration(_float fDuration)
{
	for (auto& pChannel : m_Channels)
		pChannel->ChangeKeyFrameTime(fDuration / m_fDuration);

	for (_int i = 0; i < m_TickPerSeconds.size(); ++i)
	{
		m_TickPerSeconds[i] *= (fDuration / m_fDuration);
		m_ChangeTickTimes[i] *= (fDuration / m_fDuration);
	}

	for (_int i = 0; i < m_vecAnimEvent.size(); ++i)
	{
		m_vecAnimEvent[i].fStartTime *= (fDuration / m_fDuration);
		m_vecAnimEvent[i].fEndTime *= (fDuration / m_fDuration);
	}

	m_fDuration = fDuration;
}

void CAnimation::Reset_NonLoop(void)
{
	for (auto& pChannel : m_Channels)
		pChannel->Reset();

	m_isFinished = false;
}

void CAnimation::Reset(void)
{
	for (auto& pChannel : m_Channels)
		pChannel->Reset();

	m_fCurrentTime = 0.f;
	m_iTickPerSecondIndex = 0;
	m_isFinished = false;
}

CAnimation * CAnimation::Create(CModel* pModel, aiAnimation * pAIAnimation, _bool bLoop)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pModel, pAIAnimation, bLoop)))
	{
		ERR_MSG(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Create(HANDLE hFile, _ulong * pdwByte, CModel * pModel, HANDLE hAddFile, _ulong* pdwAddByte, _bool bLoop)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(hFile, pdwByte, pModel, hAddFile, pdwAddByte,  bLoop)))
	{
		ERR_MSG(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free(void)
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}

