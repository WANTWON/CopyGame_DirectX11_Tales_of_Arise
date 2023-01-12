#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"



CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(CModel* pModel, aiNodeAnim * pAIChannel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	/* 이 애니메이션 하나르 ㄹ구동할때 시간별로 표현되었어야할 상태(KEYFRAME) */
	/* 이 애니메이션 하나르 ㄹ구동할때 몇개의 상태로 표현되어야하는가? m_iNumKeyframes */
	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME				Keyframe;
		ZeroMemory(&Keyframe, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			Keyframe.fTime = pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			Keyframe.fTime = pAIChannel->mRotationKeys[i].mTime;
		}

		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			Keyframe.fTime = pAIChannel->mPositionKeys[i].mTime;
		}

		Keyframe.vScale = vScale;
		Keyframe.vRotation = vRotation;
		Keyframe.vPosition = vPosition;

		m_KeyFrames.push_back(Keyframe);
	}

	/* 이 뼈의 현재 상태행렬을 만들면. 메시에가 가지고 있는 뼈에던지는게 맞아보이지만 
	실제론 모델의 주소를 공유하고 있기때문에 모델이 가지고 있는 뼈에 던진다.  */
	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(_float fCurrentTime)
{
	/* 던져진 시간에 맞는 뼈의 상태를 만들거야. 
	만들면 모델이 가지공 ㅣㅆ는 뼈에 던질꺼야. */

	_vector			vScale, vRotation, vPosition;

	KEYFRAME		LastKeyframe = m_KeyFrames.back();

	if (fCurrentTime >= LastKeyframe.fTime)	
	{
		vScale = XMLoadFloat3(&LastKeyframe.vScale);
		vRotation = XMLoadFloat4(&LastKeyframe.vRotation);
		vPosition = XMLoadFloat3(&LastKeyframe.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (fCurrentTime >= m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTime)
			++m_iCurrentKeyFrameIndex;			

		_vector		vSourScale, vSourRotation, vSourPosition;
		_vector		vDestScale, vDestRotation, vDestPosition;
		
		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vPosition);
			
		_float		fRatio = (fCurrentTime - m_KeyFrames[m_iCurrentKeyFrameIndex].fTime) / 
			(m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTime - m_KeyFrames[m_iCurrentKeyFrameIndex].fTime);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	
	m_pBoneNode->Set_TransformationMatrix(TransformationMatrix);
	
}

void CChannel::Reset()
{
	m_iCurrentKeyFrameIndex = 0;
}

CChannel * CChannel::Create( CModel* pModel, aiNodeAnim * pAIChannel)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pModel, pAIChannel)))
	{
		ERR_MSG(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pBoneNode);
}
