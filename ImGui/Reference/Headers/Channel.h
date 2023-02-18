#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	char* Get_ChannelName() { return m_szName; }
	_uint Get_CurrentKeyframe() { return m_iCurrentKeyFrameIndex; }

public:
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	HRESULT Bin_Initialize(DATA_BINCHANNEL* pAIChannel, class CModel* pModel); // 추가
	void Invalidate_TransformationMatrix(_float fCurrentTime, const char* pBoneName);
	void Reset();
	void Reset_Linear(void) { m_KeyFrame_Linear = m_KeyFrames[0]; }
	_bool Linear_Interpolation(KEYFRAME NextKeyFrame, _float fLinearCurrentTime, _float fLinearTotalTime, const char* pBoneName);

public: /*Get*/
	KEYFRAME	Get_StartKeyFrame(void) { return m_KeyFrames[0]; }
	KEYFRAME	Get_LinearKeyFrame(void) { return m_KeyFrame_Linear; }
	vector<KEYFRAME> Get_KeyFrames(void) { return m_KeyFrames; }
	class CHierarchyNode* Get_BoneNode(void) { return m_pBoneNode; }

public: /*Set*/
	void Set_KeyFrame(_float fDurationRatio);

private:
	char					m_szName[MAX_PATH] = "";
	class CHierarchyNode*	m_pBoneNode = nullptr;

private:
	_uint					m_iNumKeyframes = 0;
	vector<KEYFRAME>		m_KeyFrames;

	_uint					m_iCurrentKeyFrameIndex = 0;
	KEYFRAME				m_KeyFrame_Linear;

	_vector					m_vPrePos;

//public: // For. Data
//	void Get_ChannelData(DATA_BINCHANNEL* pChannelData);

public:
	//static CChannel* Create(class CModel* pModel, aiNodeAnim* pAIChannel);
	static CChannel* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	static CChannel* Bin_Create(DATA_BINCHANNEL* pAIChannel, class CModel* pModel);	// 추가
	virtual void Free() override;
};

END