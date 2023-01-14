#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(CModel* pModel, aiAnimation * pAIAnimation)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	/* 애니메이션 재생에 필요한 총 시간. */
	m_fDuration = (_float)pAIAnimation->mDuration;

	/* 초당 애니메이션 재생 속도 */
	m_fTickPerSecond = (_float)pAIAnimation->mTicksPerSecond;

	/* 애니메이션을 표현하는데 사용하는 뼈의 갯수. */
	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		aiNodeAnim*		pAIChannel = pAIAnimation->mChannels[i];

		/* 이뼈가 이 애니메이션 하나를 구동할때 시간별로 표현되었어야할 상태(KEYFRAME)  */
		CChannel*		pChannel = CChannel::Create(pModel, pAIAnimation->mChannels[i]);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
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

_bool CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta, _bool isLoop)
{
	/* 현재 재생중인 시간. */
	m_fCurrentTime += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTime >= m_fDuration)
	{
		Set_TimeReset();
		m_isFinished = true;
		if (!isLoop)
			return m_isFinished;
	}
	else
		m_isFinished = false;

	for (auto& pChannel : m_Channels)
	{
		pChannel->Invalidate_TransformationMatrix(m_fCurrentTime);
	}

	if (true == m_isFinished && true == isLoop)
		m_isFinished = false; //루프를 돌 때는 무조건 false로 반환하게 하려고

	return m_isFinished; //루프를 돌지 않고 애니메이션이 끝났을 때
}

_bool CAnimation::Animation_Linear_Interpolation(_float fTimeDelta, CAnimation * NextAnim)
{
	/* 현재 재생중인 시간. */
	vector<CChannel*> NextAnimChannels = NextAnim->Get_Channels();

	m_bLinearFinished = false;
	m_fLinear_CurrentTime += fTimeDelta;

	if (!m_bLinearFinished)
	{
		for (_uint i = 0; i<m_iNumChannels; ++i)
		{
			if (m_Channels[i]->Linear_Interpolation(NextAnimChannels[i]->Get_StartKeyFrame(), m_fLinear_CurrentTime, m_fTotal_Linear_Duration))
			{
				m_fLinear_CurrentTime = 0.f;
				m_bLinearFinished = true;
				m_Channels[i]->Reset();
			}

		}
	}
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

void CAnimation::Get_AnimData(DATA_BINANIM * pAnimData)
{
	pAnimData->fDuration = m_fDuration;
	pAnimData->iNumChannels = m_iNumChannels;
	pAnimData->fTickPerSecond = m_fTickPerSecond;
	pAnimData->bLoop = m_isLoop;
	strcpy_s(pAnimData->szName, m_szName);

	pAnimData->pBinChannel = new DATA_BINCHANNEL[m_iNumChannels];

	for (_int i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Get_ChannelData(&pAnimData->pBinChannel[i]);
	}
}

CAnimation * CAnimation::Create( CModel* pModel, aiAnimation * pAIAnimation)
{
	CAnimation*	pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pModel, pAIAnimation)))
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
