#pragma once

#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	const char* Get_Name(void) const { return m_szName; }
	void Set_OffsetMatrix(_fmatrix OffsetMatrix) { XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix); }
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) { XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix); }
	void Set_Children(_uint iChildren) { m_iChildren = iChildren; }
	_uint Get_Children(void) const { return m_iChildren; }

	_matrix Get_OffsetMatrix(void) const { return XMLoadFloat4x4(&m_OffsetMatrix); }
	_matrix Get_TransformationMatrix(void) const { return XMLoadFloat4x4(&m_TransformationMatrix); }
	_matrix Get_CombinedTransformationMatrix(void) const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

public:
	HRESULT Initialize(const aiNode* pNode, CHierarchyNode* pParent);
	HRESULT Initialize(BINBONE tBone, CHierarchyNode* pParent);
	void Invalidate_CombinedTransformationMatrix(void);

private:
	char m_szName[MAX_PATH] = "";

	CHierarchyNode* m_pParent = nullptr;

	_float4x4 m_OffsetMatrix;
	_float4x4 m_TransformationMatrix;
	_float4x4 m_CombinedTransformationMatrix;

	_uint m_iChildren;

public:
	static CHierarchyNode* Create(const aiNode* pNode, CHierarchyNode* pParent);
	static CHierarchyNode* Create(BINBONE tBone, CHierarchyNode* pParent);
	virtual void Free(void) override;
};

END	