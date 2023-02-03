#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

void CHierarchyNode::Set_TransformationMatrix(_fmatrix TransformationMatrix)
{
	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
}

HRESULT CHierarchyNode::Initialize(BINBONE tBone, CHierarchyNode* pParent)
{
	memcpy(&m_szName, tBone.szName, sizeof(char) * MAX_PATH);

	/* m_OffsetMatrix 뼈의 로컬영역에서 정점의 로컬영역으로 변환해주기위한 행렬.  */
	/* 나중에 채울꺼니까 일단 항등으로 초기화. */
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	/* 언제든 바뀔 수 있다. 애니메이션이 재생되면. */
	/* m_TransformationMatrix : 이 뼈만의 상태. (부모기준으로 표현된) (부모의 상태를 제외된) */
	m_TransformationMatrix = tBone.Transformation;
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	// 최초 변환, 이전 변환 행렬 저장
	m_PreTransforamtionMatrix = XMLoadFloat4x4(&m_TransformationMatrix);
	m_FirstTransformMatrix = m_PreTransforamtionMatrix;

	/* m_CombinedTransformationMatrix : 이 뼈 + 부모의 상태. (원점기준으로 표현된) (부모의 상태를 포함한) */
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	/* 자식의 주소를 가진다.(x)
	부모의 주소를 가지지? : 나의 m_CombinedTransformationMatrix를 만들기위해서는 부모의 m_CombinedTransformationMatrix가 필요했기 때문.
	그래서 부모의 주소가 필요해. */
	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

HRESULT CHierarchyNode::Bin_Initialize(DATA_BINNODE * pNode)
{
	strcpy_s(m_szName, pNode->cName);
	strcpy_s(m_szParentName, pNode->cParent);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	if (pNode->mTransform._11 == -431602080.)
		XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixIdentity());
	else
		memcpy(&m_TransformationMatrix, &pNode->mTransform, sizeof(_float4x4));


	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CHierarchyNode::Invalidate_CombinedTransformationmatrix(const char* pBoneName, _bool isInterpolation)
{
	Set_RootMotionMatrix(pBoneName, isInterpolation);

	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	else
		m_CombinedTransformationMatrix = m_TransformationMatrix;
}

void CHierarchyNode::Set_FindParent(CHierarchyNode * pNode)
{
	const char* pName = pNode->Get_Name();
	if (!strcmp(m_szParentName, pName))
	{
		m_pParent = pNode;
		Safe_AddRef(m_pParent);
		return;
	}
	m_pParent = nullptr;
}

//CHierarchyNode * CHierarchyNode::Create(const aiNode* pNode, CHierarchyNode* pParent)
//{
//	CHierarchyNode*		pInstance = new CHierarchyNode();
//
//	if (FAILED(pInstance->Initialize(pNode, pParent)))
//	{
//		ERR_MSG(TEXT("Failed to Created : CTransform"));
//		Safe_Release(pInstance);
//	}
//	return pInstance;	
//}

void CHierarchyNode::Set_RootMotionMatrix(const char* pBoneName, _bool isInterpolation)
{
	// 루트 본 이름 비교로 찾기
	if ((nullptr != pBoneName) && !strcmp(m_szName, pBoneName))
	{
		_matrix AfterTransformMatrix = XMLoadFloat4x4(&m_TransformationMatrix);

		_vector vPreQuat, vAfterQuat;
		vPreQuat = XMQuaternionNormalize(XMQuaternionRotationMatrix(m_PreTransforamtionMatrix));
		vAfterQuat = XMQuaternionNormalize(XMQuaternionRotationMatrix(AfterTransformMatrix));

		// 쿼터니언 내적을 통한 두 쿼터니언 사이 각 구함
		m_fRotationRadian = acosf(XMVectorGetX(XMQuaternionDot(vPreQuat, vAfterQuat))) * 2.f;

		// 이전 변환 행렬 저장
		m_PreTransforamtionMatrix = AfterTransformMatrix;

		// 루트 본 이동, 회전 값 제거
		AfterTransformMatrix = m_FirstTransformMatrix;
		AfterTransformMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		XMStoreFloat4x4(&m_TransformationMatrix, AfterTransformMatrix);
	}
}

CHierarchyNode * CHierarchyNode::Create(BINBONE tBone, CHierarchyNode * pParent)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(tBone, pParent)))
	{
		ERR_MSG(TEXT("Failed to Created : CHierarchyNode"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHierarchyNode * CHierarchyNode::Bin_Create(DATA_BINNODE * pNode)
{
	CHierarchyNode*			pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Bin_Initialize(pNode)))
	{
		ERR_MSG(TEXT("Failed To Created : CHierarchyNode_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}