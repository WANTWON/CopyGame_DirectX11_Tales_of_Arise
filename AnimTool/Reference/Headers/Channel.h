#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(class CModel* pModel, aiNodeAnim* pAIChannel);
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	void Invalidate_TransformationMatrix(_float fCurrentTime);
	void Reset(void) { m_iCurrentKeyFrameIndex = 0; }
	
public:
	const char* Get_Name(void) const { return m_szName; }
	vector<KEYFRAME> Get_KeyFrames(void) { return m_KeyFrames; }
	KEYFRAME Get_ChangeKeyFrame(void) { return m_ChangeKeyFrame; }
	class CHierarchyNode* Get_BoneNode(void) const { return m_pBoneNode; }
	_uint Get_ScalingKeys(void) const { return m_iNumScalingKeys; }
	_uint Get_RotationKeys(void) const { return m_iNumRotationKeys; }
	_uint Get_PositionKeys(void) const { return m_iNumPositionKeys; }

public:
	void ChangeKeyFrameTime(_float fDuration);
	void Set_KeyFrames(_float fDuration/*_int iIndex, KEYFRAME KeyFrame*/);

private:
	char m_szName[MAX_PATH] = "";
	class CHierarchyNode* m_pBoneNode = nullptr;
	
private:
	_uint m_iNumKeyFrames = 0;
	vector<KEYFRAME> m_KeyFrames;

	_uint m_iCurrentKeyFrameIndex = 0;

	_uint m_iNumScalingKeys, m_iNumRotationKeys, m_iNumPositionKeys;
	KEYFRAME m_ChangeKeyFrame;

public:
	static CChannel* Create(class CModel* pModel, aiNodeAnim* pAIChannel);
	static CChannel* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	virtual void Free(void) override;
};

END