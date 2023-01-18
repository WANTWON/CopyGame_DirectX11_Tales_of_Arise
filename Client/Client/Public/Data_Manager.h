#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CData_Manager final : public CBase
{
	DECLARE_SINGLETON(CData_Manager)

public:
	enum DATA_TYPE { DATA_ANIM, DATA_NONANIM, DATA_PARTS, DATA_END };


public:
	CData_Manager();
	virtual ~CData_Manager() = default;

	// For. Static
public:
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	//HRESULT Conv_Bin_Model(CModel* pModel, char* cModelName, DATA_TYPE eTYPE);
	HRESULT SaveSceneData(DATA_BINSCENE* pScene, char* cModelName, DATA_TYPE eTYPE);
	HRESULT ReadSceneData(char* pFileName, DATA_BINSCENE* pScene, DATA_TYPE eTYPE);

	//HRESULT Create_Try_BinModel(const _tchar* pModelName, LEVEL eLEVEL, DATA_TYPE eTYPE);


private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	// Text
public:
	void TCtoC(const TCHAR* pTC, char*pC) { WideCharToMultiByte(CP_ACP, 0, pTC, MAX_PATH, pC, MAX_PATH, NULL, NULL); }
	void CtoTC(const char* pC, TCHAR* pTC) { MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pC, MAX_PATH, pTC, MAX_PATH); }

public:
	void Add_ModelTag(_tchar* pModeltag) { m_pModelTags.push_back(pModeltag); }

private:
	vector<_tchar*>		m_pModelTags;
private:
	list<_tchar*> m_ManagedTChar;
	list<char*> m_ManagedChar;

public:
	virtual void Free() override;
};

END

