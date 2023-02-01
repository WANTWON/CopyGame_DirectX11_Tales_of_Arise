#pragma once

/* 뼈. */
#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}
	// 추가
	const char* Get_ParentName() const {
		if (nullptr == m_pParent)
			return "";
		return m_pParent->Get_Name();
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix);

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}
	// 추가
	XMFLOAT4X4 Get_OffSetMatrixXM() {
		return m_OffsetMatrix;
	}
	// 추가
	_float4x4 Get_OriTransformationMatrix() { return m_OriTransformationMatrix; }
	// 추가
	CHierarchyNode* Get_Parent() { return m_pParent; }

	// 루트노드 위한 함수들 - 이동 행렬, 회전 행렬 반환
	_vector Get_Translation(void) { return m_vecTranslation; }
	_float Get_RotationRadian(void) { return m_fRotationRadian; }

public:
	HRESULT Initialize(BINBONE tBone, CHierarchyNode* pParent);
	HRESULT Bin_Initialize(DATA_BINNODE* pNode); // 추가
	void Invalidate_CombinedTransformationmatrix(const char* pBoneName, _bool isInterpolation);
	void Set_FindParent(CHierarchyNode* pNode); // 추가

private:
	char				m_szName[MAX_PATH] = "";
	char				m_szParentName[MAX_PATH] = ""; // 추가
	CHierarchyNode*		m_pParent = nullptr;

	/* 기본은 항등, 메시안에서(aiBone) 값을 채워준다. */
	_float4x4			m_OffsetMatrix;
	_float4x4			m_OriTransformationMatrix; // 추가

	_float4x4			m_TransformationMatrix;

	/* m_TransformationMatrix * m_pParent->m_CombinedTransformationMatrix */
	_float4x4			m_CombinedTransformationMatrix;

	/* 이동, 회전값 살리기 위한 매트릭스 */
	_vector m_vecTranslation;
	_float m_fRotationRadian;
	_matrix m_PreTransforamtionMatrix;
	_matrix m_FirstTransformMatrix;

private:
	/* 루트 본 이동 값 제거, 루트 본 이동, 회전 행렬 저장 함수 */
	void Set_RootMotionMatrix(const char* pBoneName, _bool isInterpolation);

public:
	//static CHierarchyNode* Create(const aiNode* pNode, CHierarchyNode* pParent);
	static CHierarchyNode* Create(BINBONE tBone, CHierarchyNode* pParent);
	static CHierarchyNode* Bin_Create(DATA_BINNODE* pNode); // 추가
	virtual void Free() override;
};

END