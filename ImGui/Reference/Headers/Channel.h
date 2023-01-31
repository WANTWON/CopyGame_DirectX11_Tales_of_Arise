#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	HRESULT Bin_Initialize(DATA_BINCHANNEL* pAIChannel, class CModel* pModel); // 추가
	void Invalidate_TransformationMatrix(_float fCurrentTime, _vector* pRotation = nullptr, _vector* pPosition = nullptr);
	void Reset();
	_bool Linear_Interpolation(KEYFRAME PreKeyFrame, _float fLinearCurrentTime, _float fLinearTotalTime, _vector* pRotation = nullptr, _vector* pPosition = nullptr);

public: /*Get*/
	KEYFRAME	Get_StartKeyFrame(void) { return m_KeyFrames[0]; }
	KEYFRAME	Get_KeyFrameLinear(void) { return m_KeyFrame_Linear; }
	vector<KEYFRAME> Get_KeyFrames(void) { return m_KeyFrames; }
	const char* Get_Name() const { return m_szName; }

public: /*Set*/
	void Set_KeyFrame(_int iIndex, KEYFRAME KeyFrame);
	
public:
	_bool Compare_Name(const char* pName);

private:
	char					m_szName[MAX_PATH] = "";
	class CHierarchyNode*	m_pBoneNode = nullptr;

private:
	_uint					m_iNumKeyframes = 0;
	vector<KEYFRAME>		m_KeyFrames;

	_uint					m_iCurrentKeyFrameIndex = 0;
	KEYFRAME				m_KeyFrame_Linear;

//public: // For. Data
//	void Get_ChannelData(DATA_BINCHANNEL* pChannelData);

public:
	//static CChannel* Create(class CModel* pModel, aiNodeAnim* pAIChannel);
	static CChannel* Create(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	static CChannel* Bin_Create(DATA_BINCHANNEL* pAIChannel, class CModel* pModel);	// 추가
	virtual void Free() override;
};

END