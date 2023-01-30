#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(HANDLE hFile, _ulong * pdwByte, CModel * pModel, HANDLE hAddFile, _ulong* pdwAddByte)
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

	if (INVALID_HANDLE_VALUE != hAddFile)
	{
		ReadFile(hAddFile, &m_fDuration, sizeof(_float), pdwAddByte, nullptr);

		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			vector<KEYFRAME>KeyFrames = m_Channels[i]->Get_KeyFrames();

			for (_uint j = 0; j < KeyFrames.size(); ++j)
			{
				KEYFRAME KeyFrame;
				ReadFile(hAddFile, &KeyFrame, sizeof(KEYFRAME), pdwAddByte, nullptr);

				m_Channels[i]->Set_KeyFrame(j, KeyFrame);
			}
		}

		_int TickPerSecondsSize, ChangeTimesSize, EventsSize;

		ReadFile(hAddFile, &TickPerSecondsSize, sizeof(_uint), pdwAddByte, nullptr);
		for (_int i = 0; i < TickPerSecondsSize; ++i)
		{
			_float fTickPerSecond;
			ReadFile(hAddFile, &fTickPerSecond, sizeof(_float), pdwAddByte, nullptr);

			m_TickPerSeconds.push_back(fTickPerSecond);
		}

		ReadFile(hAddFile, &ChangeTimesSize, sizeof(_uint), pdwAddByte, nullptr);
		for (_int i = 0; i < ChangeTimesSize; ++i)
		{
			_float fChangeTime;
			ReadFile(hAddFile, &fChangeTime, sizeof(_float), pdwAddByte, nullptr);

			m_ChangeTickTimes.push_back(fChangeTime);
		}

		ReadFile(hAddFile, &EventsSize, sizeof(_uint), pdwAddByte, nullptr);
		for (_int i = 0; i < EventsSize; ++i)
		{
			ANIMEVENT tEvent;
			ReadFile(hAddFile, &tEvent, sizeof(ANIMEVENT), pdwAddByte, nullptr);

			m_vecAnimEvent.push_back(tEvent);
			
			EVENT tCheckEvent;
			tCheckEvent.isPlay = false;
			tCheckEvent.iEventType = tEvent.eType;
			m_vecEvents.push_back(tCheckEvent);
		}
	}
	
	return S_OK;
}

HRESULT CAnimation::Bin_Initialize(DATA_BINANIM * pAIAnimation, CModel * pModel)
{
	strcpy_s(m_szName, pAIAnimation->szName);

	m_fDuration = pAIAnimation->fDuration;

	m_fTickPerSecond = pAIAnimation->fTickPerSecond;

	/* 현재 애니메이션에서 제어해야할 뼈들의 갯수를 저장한다. */
	m_iNumChannels = pAIAnimation->iNumChannels;

	m_isLoop = pAIAnimation->bLoop;

	/* 현재 애니메이션에서 제어해야할 뼈정보들을 생성하여 보관한다. */
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel*		pChannel = CChannel::Bin_Create(&pAIAnimation->pBinChannel[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta, _bool isLoop, const char* pRootName)
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

	/* 현재 재생중인 시간. */
	m_fCurrentTime += m_fTickPerSecond * fTimeDelta;

	//애니메이션 이벤트
	for (_int i = 0; i < m_vecAnimEvent.size(); ++i)
	{
		if (m_vecAnimEvent[i].fStartTime < m_fCurrentTime && m_vecAnimEvent[i].fEndTime > m_fCurrentTime)
			m_vecEvents[i].isPlay = true;
		else
			m_vecEvents[i].isPlay = false;
	}

	if (m_fCurrentTime > m_fDuration)
	{
		m_isFinished = true;
		m_iTickPerSecondIndex = 0;
		if (!isLoop)
		{
			for (auto& pChannel : m_Channels)
			{
				if ((nullptr != pRootName) && !strcmp(pChannel->Get_Name(), pRootName))
					pChannel->Invalidate_TransformationMatrix(m_fCurrentTime, &m_vRotation, &m_vPosition);
				else
					pChannel->Invalidate_TransformationMatrix(m_fCurrentTime);
			}

			Set_TimeReset();

			return m_isFinished;
		}
		Set_TimeReset();
	}
	else
		m_isFinished = false;

	for (auto& pChannel : m_Channels)
	{
		if ((nullptr != pRootName) && !strcmp(pChannel->Get_Name(), pRootName))
			pChannel->Invalidate_TransformationMatrix(m_fCurrentTime, &m_vRotation, &m_vPosition);
		else
			pChannel->Invalidate_TransformationMatrix(m_fCurrentTime);
	}

	if (true == m_isFinished && true == isLoop)
		m_isFinished = false; //루프를 돌 때는 무조건 false로 반환하게 하려고

	return m_isFinished; //루프를 돌지 않고 애니메이션이 끝났을 때
}

_bool CAnimation::Animation_Linear_Interpolation(_float fTimeDelta, CAnimation * NextAnim, const char* pRootName)
{
	/* 현재 재생중인 시간. */
	vector<CChannel*> NextAnimChannels = NextAnim->Get_Channels();

	m_bLinearFinished = false;
	m_fLinear_CurrentTime += fTimeDelta;

	if (!m_bLinearFinished)
	{
		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			_bool isFinishInterpolation = false;

			if ((nullptr != pRootName) && !strcmp(m_Channels[i]->Get_Name(), pRootName))
				isFinishInterpolation = m_Channels[i]->Linear_Interpolation(NextAnimChannels[i]->Get_StartKeyFrame(), m_fLinear_CurrentTime, m_fTotal_Linear_Duration, &m_vRotation, &m_vPosition);
			else
				isFinishInterpolation = m_Channels[i]->Linear_Interpolation(NextAnimChannels[i]->Get_StartKeyFrame(), m_fLinear_CurrentTime, m_fTotal_Linear_Duration);
			
			if (isFinishInterpolation)
			{
				m_fLinear_CurrentTime = 0.f;
				m_bLinearFinished = true;
				m_Channels[i]->Reset();
			}
		}
	}
	else
		m_iTickPerSecondIndex = 0;

	return m_bLinearFinished;
}

void CAnimation::Set_TimeReset()
{
	for (auto& pChannel : m_Channels)
	{
		pChannel->Reset();
	}
	m_fCurrentTime = 0.f;
}

void CAnimation::Reset(void)
{
	for (auto& pChannel : m_Channels)
	{
		pChannel->Reset();
	}

	for (auto& Event : m_vecEvents)
		Event.isPlay = false;

	m_iTickPerSecondIndex = 0;
	m_fCurrentTime = 0.f;
}

//void CAnimation::Get_AnimData(DATA_BINANIM * pAnimData)
//{
//	pAnimData->fDuration = m_fDuration;
//	pAnimData->iNumChannels = m_iNumChannels;
//	pAnimData->fTickPerSecond = m_fTickPerSecond;
//	pAnimData->bLoop = m_isLoop;
//	strcpy_s(pAnimData->szName, m_szName);
//
//	pAnimData->pBinChannel = new DATA_BINCHANNEL[m_iNumChannels];
//
//	for (_int i = 0; i < m_iNumChannels; ++i)
//	{
//		m_Channels[i]->Get_ChannelData(&pAnimData->pBinChannel[i]);
//	}
//}

//CAnimation * CAnimation::Create( CModel* pModel, aiAnimation * pAIAnimation)
//{
//	CAnimation*	pInstance = new CAnimation();
//
//	if (FAILED(pInstance->Initialize(pModel, pAIAnimation)))
//	{
//		ERR_MSG(TEXT("Failed to Created : CAnimation"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CAnimation * CAnimation::Create(HANDLE hFile, _ulong * pdwByte, CModel * pModel, HANDLE hAddFile, _ulong* pdwAddByte)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(hFile, pdwByte, pModel, hAddFile, pdwAddByte)))
	{
		ERR_MSG(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation * CAnimation::Bin_Create(DATA_BINANIM * pAIAnimation, CModel * pModel)
{
	CAnimation*			pInstance = new CAnimation();

	if (FAILED(pInstance->Bin_Initialize(pAIAnimation, pModel)))
	{
		ERR_MSG(TEXT("Failed To Created : CAnimation_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
