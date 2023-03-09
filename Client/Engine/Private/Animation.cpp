#include "..\Public\Animation.h"
#include "Channel.h"
#include "HierarchyNode.h"

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

	if (INVALID_HANDLE_VALUE != hAddFile && (pdwAddByte != 0))
	{
		_float fDuration;
		_bool isReadDuration = ReadFile(hAddFile, &fDuration, sizeof(_float), pdwAddByte, nullptr);

		if (isReadDuration && (0 == (*pdwAddByte)))
			return S_OK;

		_float fDurationRatio = fDuration / m_fDuration;

		for (_uint i = 0; i < m_iNumChannels; ++i)
			m_Channels[i]->Set_KeyFrame(fDurationRatio);

		m_fDuration = fDuration;

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

_bool CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta, _bool isLoop, const char* pBoneName)
{
	//애니메이션 구간별 재생 속도 변화
	_int iSize = _int(m_ChangeTickTimes.size() - 1);
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
				m_fTickPerSecond = m_TickPerSeconds[m_iTickPerSecondIndex - 1];
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
		if (m_vecAnimEvent[i].fStartTime <= m_fCurrentTime && m_vecAnimEvent[i].fEndTime >= m_fCurrentTime)
			m_vecAnimEvent[i].isPlay = true;
		else
			m_vecAnimEvent[i].isPlay = false;
	}

	if (m_fCurrentTime >= m_fDuration)
	{
		m_isFinished = true;

		if (isLoop)
		{
			m_iTickPerSecondIndex = 0;
			m_fCurrentTime = 0.f;
		}
	}


	for (auto& pChannel : m_Channels)
	{
		if (m_isFinished && isLoop)
		{
			pChannel->Reset();
			pChannel->Reset_Linear();
		}

		pChannel->Invalidate_TransformationMatrix(m_fCurrentTime, pBoneName);
	
		KEYFRAME ChannelKeyFrame = pChannel->Get_LinearKeyFrame();

		_vector vScale = XMLoadFloat3(&ChannelKeyFrame.vScale);
		_vector vRotation = XMLoadFloat4(&ChannelKeyFrame.vRotation);
		_vector vPosition = XMLoadFloat3(&ChannelKeyFrame.vPosition);

		_matrix	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pChannel->Get_BoneNode()->Set_TransformationMatrix(TransformationMatrix);
	}

	if (m_isFinished && isLoop)
		m_isFinished = false;

	return m_isFinished; //루프를 돌지 않고 애니메이션이 끝났을 때
}

_bool CAnimation::Animation_Linear_Interpolation(_float fTimeDelta, CAnimation * NextAnim, const char* pBoneName)
{
	/* 현재 재생중인 시간. */
	vector<CChannel*> NextAnimChannels = NextAnim->Get_Channels();

	m_bLinearFinished = false;

	if (!m_bLinearFinished)
	{
		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			if (m_Channels[i]->Linear_Interpolation(NextAnimChannels[i]->Get_StartKeyFrame(), m_fLinear_CurrentTime, m_fTotal_Linear_Duration, pBoneName))
			{
				m_bLinearFinished = true;
				m_Channels[i]->Reset();
			}
		}

		m_fLinear_CurrentTime += fTimeDelta;
	}

	if (m_bLinearFinished)
	{
		m_iTickPerSecondIndex = 0;
		m_fLinear_CurrentTime = 0.f;

		return true;
	}
	
	return false;
}

_bool CAnimation::Is_Keyframe(char * pChannelName, _uint iKeyframe)
{
	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)
	{
		return !strcmp(pChannelName, pChannel->Get_ChannelName());
	});

	if (iter == m_Channels.end())
		return false;
	else
	{
		if ((*iter)->Get_CurrentKeyframe() == iKeyframe)
			return true;
		else
			return false;
	}
}

_bool CAnimation::Under_Keyframe(char * pChannelName, _uint iKeyframe)
{
	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)
	{
		return !strcmp(pChannelName, pChannel->Get_ChannelName());
	});

	if (iter == m_Channels.end())
		return false;
	else
	{
		if ((*iter)->Get_CurrentKeyframe() < iKeyframe)
			return true;
		else
			return false;
	}
}

_bool CAnimation::Over_Keyframe(char * pChannelName, _uint iKeyframe)
{
	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)
	{
		return !strcmp(pChannelName, pChannel->Get_ChannelName());
	});

	if (iter == m_Channels.end())
		return false;
	else
	{
		if ((*iter)->Get_CurrentKeyframe() > iKeyframe)
			return true;
		else
			return false;
	}
}

_bool CAnimation::Between_Keyframe(char * pChannelName, _uint iKeyframeLower, _uint iKeyframeUpper)
{
	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)
	{
		return !strcmp(pChannelName, pChannel->Get_ChannelName());
	});

	if (iter == m_Channels.end())
		return false;
	else
	{
		if (((*iter)->Get_CurrentKeyframe() >= iKeyframeLower) && ((*iter)->Get_CurrentKeyframe() <= iKeyframeUpper))
			return true;
		else
			return false;
	}
}

void CAnimation::Set_TimeReset()
{
	for (auto& pChannel : m_Channels)
		pChannel->Reset();
	
	//m_fCurrentTime = 0.f;
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

	for (auto& pChannel : m_Channels)
		pChannel->Reset_Linear();

	for (auto& Event : m_vecAnimEvent)
		Event.isPlay = false;

	m_iTickPerSecondIndex = 0;
	m_fCurrentTime = 0.f;
	m_isFinished = false;
}

void CAnimation::Reset_ChannelKeyFrame(void)
{
	for (auto& pChannel : m_Channels)
		pChannel->Reset_Linear();
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
