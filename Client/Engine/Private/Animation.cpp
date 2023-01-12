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

		/* 이뼈가 이 애니메이션 하나르 ㄹ구동할때 시간별로 표현되었어야할 상태(KEYFRAME)  */
		CChannel*		pChannel = CChannel::Create(pModel, pAIAnimation->mChannels[i]);

		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta)
{
	/* 현재 재생중인 시간. */
	m_fCurrentTime += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTime >= m_fDuration)
	{
		m_fCurrentTime = 0.f;

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

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
