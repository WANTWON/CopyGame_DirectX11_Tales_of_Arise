#include "UI_Portraitfront.h"


BEGIN(Client)

class CUI_Portraitfront_left final : public CUI_Portraitfront
{
private:
	CUI_Portraitfront_left(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Portraitfront_left(const CUI_Portraitfront_left& rhs);
	virtual ~CUI_Portraitfront_left() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Glow();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CUI_Portraitfront_left* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;






};

END