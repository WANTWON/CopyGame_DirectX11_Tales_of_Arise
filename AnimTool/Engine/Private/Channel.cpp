#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(CModel* pModel, aiNodeAnim * pAIChannel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3 vScale, vPosition;
	_float4 vRotation;

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mScalingKeys[i].mTime;
		}

		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = pAIChannel->mRotationKeys[i].mTime;
		}

		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}

HRESULT CChannel::Initialize(HANDLE hFile, _ulong * pdwByte, CModel * pModel)
{
	if (nullptr == hFile)
		return E_FAIL;
	
	BINCHANNEL BinChannel;
	ReadFile(hFile, &BinChannel, sizeof(BINCHANNEL), pdwByte, nullptr);

	memcpy(m_szName, BinChannel.szName, sizeof(char) * MAX_PATH);
	m_iNumKeyFrames = BinChannel.iNumKeyFrames;

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME KeyFrame;

		ReadFile(hFile, &KeyFrame, sizeof(KEYFRAME), pdwByte, nullptr);

		m_KeyFrames.push_back(KeyFrame);
	}

	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(_float fCurrentTime)
{
	_vector vScale, vRotation, vPosition;

	KEYFRAME LastKeyFrame = m_KeyFrames.back();

	if (fCurrentTime >= LastKeyFrame.fTime)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPosition = XMLoadFloat3(&LastKeyFrame.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}
	else
	{
		while (fCurrentTime >= m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTime)
			++m_iCurrentKeyFrameIndex;
		
		_vector vSourScale, vSourRotation, vSourPosition;
		_vector vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrentKeyFrameIndex + 1].vPosition);

		_float fRatio = (fCurrentTime - m_KeyFrames[m_iCurrentKeyFrameIndex].fTime) /
			(m_KeyFrames[m_iCurrentKeyFrameIndex + 1].fTime - m_KeyFrames[m_iCurrentKeyFrameIndex].fTime);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);

		m_ChangeKeyFrame.fTime = fCurrentTime;
		XMStoreFloat3(&m_ChangeKeyFrame.vScale, vScale);
		XMStoreFloat4(&m_ChangeKeyFrame.vRotation, vRotation);
		XMStoreFloat3(&m_ChangeKeyFrame.vPosition, vPosition);
	}

	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBoneNode->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::ChangeKeyFrameTime(_float fDuration)
{
	for (auto& pKeyFrame : m_KeyFrames)
		pKeyFrame.fTime *= fDuration;
}

void CChannel::Set_KeyFrames(_float fDuration/*_int iIndex, KEYFRAME KeyFrame*/)
{
	//m_KeyFrames[iIndex] = KeyFrame;
	for (_int i = 0; i < m_KeyFrames.size(); ++i)
		m_KeyFrames[i].fTime *= fDuration;
}

CChannel * CChannel::Create(CModel* pModel, aiNodeAnim * pAIChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pModel, pAIChannel)))
	{
		ERR_MSG(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel * CChannel::Create(HANDLE hFile, _ulong * pdwByte, CModel * pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(hFile, pdwByte, pModel)))
	{
		ERR_MSG(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free(void)
{
	Safe_Release(m_pBoneNode);
}
