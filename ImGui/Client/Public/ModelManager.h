#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Model.h"
#include "NonAnim.h"
#include "TreasureBox.h"

BEGIN(Client)

class CModelManager final : public CBase
{
	DECLARE_SINGLETON(CModelManager)

private:
	CModelManager();
	virtual ~CModelManager() = default;

public:
	HRESULT Create_Model(LEVEL eLevel, const _tchar* pPrototypeTag, const _tchar* pLayerTag, ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, _fmatrix PivotMatrix, _bool bCreatePrototype = false);

public:
	const vector<class CNonAnim*> Get_CreatedModel() const { return m_CreatedModel; }

public:
	void Add_PrototypeTag(const _tchar* PrototypeTag);
	void Add_FileName(const _tchar* Layertag, const _tchar* FileName);
	void Add_CreatedModel( class CNonAnim* pNonAnimModel) { m_CreatedModel.push_back(pNonAnimModel); }
	void Set_AllPickedFalse();
	void Set_InitModelDesc(NONANIMDESC ModelDesc) { memcpy(&m_InitModelDesc, &ModelDesc, sizeof(NONANIMDESC)); }
	void Out_CreatedModel(CNonAnim* pGameObject);
	void Clear_Layer();
	vector<const _tchar*> Get_PrototypeTag() { return m_PrototypeTag; }

public:
	HRESULT Add_TreasureBox(const _tchar* pLayerTag, CTreasureBox::BOXTAG* BoxDesc);
	void	Add_TreasureBoxGroup(CTreasureBox* pObj) { m_TreasureBox.push_back(pObj); };
	_int	Get_TreasureBoxSize() { return (_int)m_TreasureBox.size(); }
	void	Out_TreasureBoxGroup(CTreasureBox* pGameObject);
	CTreasureBox*	Get_TreasureBox(_int index);
	CTreasureBox*	Get_LastTreasureBox() { return m_TreasureBox.back();};

private:
	const _tchar* Find_ModelTag(const _tchar* ModelTag);
	

private:
	map<const _tchar*, const _tchar*>  m_ModelTags;  //Prototype Tag & Path
	vector<const _tchar*> m_PrototypeTag;
	vector<class CNonAnim*> m_CreatedModel;
	NONANIMDESC  m_InitModelDesc;

	/* For Treasure Box*/
	vector<CTreasureBox*>			m_TreasureBox;

public:
	void Free() override;
};

END