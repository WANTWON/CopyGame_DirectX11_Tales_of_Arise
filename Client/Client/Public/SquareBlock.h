#pragma once

#include "NonAnim.h"


BEGIN(Client)

class CSquareBlock final : public CNonAnim
{
public:
	enum TYPE {SQUARE_BLOCK , LOCK_BLOCK, TAIL_STATUE, CRANEGAME_FENCE, WARP_HOLE };

	typedef struct BlockTag
	{
		_float3 vInitPosition = _float3(0.f, 0.f, 0.f);
		TYPE	eType = SQUARE_BLOCK;
		_float  fAngle = 0.f;
	}BLOCKDESC;

private:
	CSquareBlock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSquareBlock(const CSquareBlock& rhs);
	virtual ~CSquareBlock() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;

	void Set_Open(_bool type) { m_bOpen = type; }

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	void Tick_SquareBlock(_float fTimeDelta);
	void Tick_LockBlock(_float fTimeDelta);
	void Tick_TailStatue(_float fTimeDelta);
	void Tick_CraneGameFence(_float fTimeDelta);

private:
	BLOCKDESC m_BlockDesc;
	_bool	  m_bOpen = false;
	_bool	  m_bCollisionSet = false;
	_bool	  m_bPlayGame = false;

public:
	static CSquareBlock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END