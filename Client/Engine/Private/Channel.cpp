#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel)
{
	if (nullptr == hFile)
		return E_FAIL;

	BINCHANNEL BinChannel;
	ReadFile(hFile, &BinChannel, sizeof(BINCHANNEL), pdwByte, nullptr);

	memcpy(m_szName, BinChannel.szName, sizeof(char) * MAX_PATH);
	m_iNumKeyframes = BinChannel.iNumKeyFrames;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME KeyFrame;
		
		ReadFile(hFile, &KeyFrame, sizeof(KEYFRAME), pdwByte, nullptr);

		m_KeyFrames.push_back(KeyFrame);
	}

	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}

HRESULT CChannel::Bin_Initialize(DATA_BINCHANNEL * pAIChannel, CModel * pModel)
{
	strcpy_s(m_szName, pAIChannel->szName);

	m_iNumKeyframes = pAIChannel->iNumKeyFrames;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME			KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		KeyFrame = pAIChannel->pKeyFrames[i];

		m_KeyFrames.push_back(KeyFrame);
	}

	m_pBoneNode = pModel->Get_BonePtr(m_szName);
	Safe_AddRef(m_pBoneNode);

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(_float fCurrentTime)
{
	/* 던져진 시간에 맞는 뼈의 상태를 만들거야. 
	만들면 모델이 가지고 있는 뼈에 던질꺼야. */

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

	/*Linear*/
	XMStoreFloat3(&m_KeyFrame_Linear.vPosition, vPosition);
	XMStoreFloat4(&m_KeyFrame_Linear.vRotation, vRotation);
	XMStoreFloat3(&m_KeyFrame_Linear.vScale, vScale);

	_matrix	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBoneNode->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Reset()
{
	m_iCurrentKeyFrameIndex = 0;
}

bool CChannel::Linear_Interpolation(KEYFRAME NextKeyFrame, _float fLinearCurrentTime, _float fLinearTotalTime)
{
	_vector			vScale, vRotation, vPosition;

	if (fLinearCurrentTime >= fLinearTotalTime)
		return true;

	_float		fRatio = fLinearCurrentTime / fLinearTotalTime;


	_vector		vSourScale, vSourRotation, vSourPosition;
	_vector		vDestScale, vDestRotation, vDestPosition;

	vDestScale = XMLoadFloat3(&NextKeyFrame.vScale);
	vDestRotation = XMLoadFloat4(&NextKeyFrame.vRotation);
	vDestPosition = XMLoadFloat3(&NextKeyFrame.vPosition);

	vSourScale = XMLoadFloat3(&m_KeyFrame_Linear.vScale);
	vSourRotation = XMLoadFloat4(&m_KeyFrame_Linear.vRotation);
	vSourPosition = XMLoadFloat3(&m_KeyFrame_Linear.vPosition);


	vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
	vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
	vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
	vPosition = XMVectorSetW(vPosition, 1.f);


	/*XMStoreFloat3(&m_KeyFrame_Linear.vPosition, vPosition);
	XMStoreFloat4(&m_KeyFrame_Linear.vRotation, vRotation);
	XMStoreFloat3(&m_KeyFrame_Linear.vScale, vScale);*/

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	m_pBoneNode->Set_TransformationMatrix(TransformationMatrix);

	return false;
}

void CChannel::Set_KeyFrame(_int iIndex, KEYFRAME KeyFrame)
{
	m_KeyFrames[iIndex] = KeyFrame;
}

_bool CChannel::Compare_Name(const char * pName)
{
	if (!strcmp(m_szName, pName))
		return true;
	else
		return false;
}

//void CChannel::Get_ChannelData(DATA_BINCHANNEL * pChannelData)
//{
//	memcpy(&pChannelData->szName, m_szName, sizeof(char)*MAX_PATH);
//	pChannelData->iNumKeyFrames = m_iNumKeyframes;
//
//	pChannelData->pKeyFrames = new KEYFRAME[m_iNumKeyframes];
//
//	for (_uint i = 0; i < m_iNumKeyframes; ++i)
//	{
//		memcpy(&pChannelData->pKeyFrames[i], &m_KeyFrames[i], sizeof(KEYFRAME));
//	}
//}

//CChannel * CChannel::Create( CModel* pModel, aiNodeAnim * pAIChannel)
//{
//	CChannel*	pInstance = new CChannel();
//
//	if (FAILED(pInstance->Initialize(pModel, pAIChannel)))
//	{
//		ERR_MSG(TEXT("Failed to Created : CChannel"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

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


CChannel * CChannel::Bin_Create(DATA_BINCHANNEL * pAIChannel, CModel * pModel)
{
	CChannel*			pInstance = new CChannel();

	if (FAILED(pInstance->Bin_Initialize(pAIChannel, pModel)))
	{
		ERR_MSG(TEXT("Failed To Created : CChannel_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	Safe_Release(m_pBoneNode);
}
