#pragma once

#include "Component.h"
#include "HierarchyNode.h"
#include "Animation.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_NONINSTANCE, TYPE_ANIM, TYPE_END };

public:
	typedef struct tagAnimDesc
	{
		_int iArraySize;
		_int* pNonLoopArray = nullptr;
	}ANIMDESC;

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshContainers(void) const { return m_iNumMeshes; }
	vector<class CMeshContainer*> Get_Meshes(void) { return m_Meshes; }
	vector<class CHierarchyNode*> Get_Bones(void) { return m_Bones; }
	vector<class CAnimation*> Get_Animations(void) { return m_Animations; }
	_uint Get_NumMaterials(void) const { return m_iNumMaterials; }

	class CHierarchyNode* Get_BonePtr(const char* pBoneName) const;
	_float4x4 Get_PivotFloat4x4(void) const { return m_PivotMatrix; }

	_bool Get_Finished(_uint iIndex) const { return m_Animations[iIndex]->Get_Finished(); }
	void Reset_NonLoop(_uint iIndex) { m_Animations[iIndex]->Reset_NonLoop(); }

public:
	void Set_CurrentAnimIndex(_uint iAnimIndex);
	void Set_AnimIndex(_uint iAnimIndex);
	
public:
	_bool Picking(_matrix WorldMatrix, _vector vCamWorldPos, _float3 * pPickPoint = nullptr);

public:
	virtual HRESULT Initialize_Prototype(TYPE eModelType, const char* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Initialize_Prototype_Binary(TYPE eModelType, const char* pModelFilePath);
	HRESULT SetUp_Material(class CShader* pShader, const char* pConstantName, _uint iMeshIndex, aiTextureType eType);
	HRESULT Play_Animation(_float fTimeDelta, vector<CAnimation::EVENTTYPE>* pEventTypeVector = nullptr);
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);

private:
	const aiScene* m_pAIScene = nullptr;
	Assimp::Importer m_Importer;

	_tchar m_FilePath[MAX_PATH] = TEXT("");

private:
	TYPE m_eModelType = TYPE_END;

	_uint m_iNumMeshes = 0;
	vector<class CMeshContainer*> m_Meshes;

private:
	_uint m_iNumMaterials = 0;
	vector<MODELMATERIAL> m_Materials;

private:
	_uint m_iNumBones = 0;
	vector<class CHierarchyNode*> m_Bones;

private:
	_uint m_iNumAnimations = 0;
	vector<class CAnimation*> m_Animations;

	_uint m_iCurrentAnimIndex = 0;
	_uint m_iPreAnimIndex = 0;

private:
	_float4x4 m_PivotMatrix;

	_float m_fCurrentTime = 0.f;

	ANIMDESC m_tAnimDesc;

private:
	HRESULT Create_MeshContainer(void);
	HRESULT Create_MeshContainer_Binary(HANDLE hFile, _ulong* pdwByte);
	HRESULT Create_Materials(const char* pModelFilePath);
	HRESULT Create_Materials_Binary(HANDLE hFile, _ulong* pdwByte);
	HRESULT Create_HierarchyNodes(const aiNode* pNode, CHierarchyNode* pParent = nullptr);
	HRESULT Create_HierarchyNodes_Binary(HANDLE hFile, _ulong* pdwByte, CHierarchyNode* pParent = nullptr);
	HRESULT Create_Animations(void);
	HRESULT Create_Animations_Binary(HANDLE hFile, _ulong* pdwByte, const _tchar* pAddDataFilePath = nullptr);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const char* pModelFilePath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END