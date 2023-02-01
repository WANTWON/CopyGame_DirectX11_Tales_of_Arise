#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "GameInstance.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
	ZeroMemory(&m_ColliderDesc, sizeof(COLLIDERDESC));
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::Initialize_Prototype(TYPE eType)
{
	m_eType = eType;

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t iShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CCollider::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	_matrix ScaleMatrix, RotationMatrix, TranslationMatrix;

	ScaleMatrix = XMMatrixScaling(m_ColliderDesc.vScale.x, m_ColliderDesc.vScale.y, m_ColliderDesc.vScale.z);
	RotationMatrix = XMMatrixRotationX(m_ColliderDesc.vRotation.x) *
		XMMatrixRotationY(m_ColliderDesc.vRotation.y) *
		XMMatrixRotationZ(m_ColliderDesc.vRotation.z);
	TranslationMatrix = XMMatrixTranslation(m_ColliderDesc.vPosition.x, m_ColliderDesc.vPosition.y, m_ColliderDesc.vPosition.z);

	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		switch (m_eType)
		{
		case TYPE_AABB:
			m_pAABB[i] = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(0.5f, 0.5f, 0.5f));
			m_pAABB[i]->Transform(*m_pAABB[i], ScaleMatrix * TranslationMatrix);
			break;
		case TYPE_OBB:
			m_pOBB[i] = new BoundingOrientedBox(m_ColliderDesc.vPosition, _float3(m_ColliderDesc.vScale.x * 0.5f, m_ColliderDesc.vScale.y * 0.5f, m_ColliderDesc.vScale.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
			m_pOBB[i]->Transform(*m_pOBB[i], RotationMatrix);
			break;
		case TYPE_SPHERE:
			m_pSPHERE[i] = new BoundingSphere(_float3(0.f, 0.f, 0.f), 0.5f);
			m_pSPHERE[i]->Transform(*m_pSPHERE[i], ScaleMatrix * RotationMatrix * TranslationMatrix);
			break;
		}
	}

	return S_OK;
}

void CCollider::Tick(_fmatrix WorldMatrix)
{
	switch (m_eType)
	{
	case Engine::CCollider::TYPE_AABB:
		m_pAABB[BOUNDING_ORIGINAL]->Transform(*m_pAABB[BOUNDING_WORLD], Remove_Rotation(WorldMatrix));
		break;
	case Engine::CCollider::TYPE_OBB:
		m_pOBB[BOUNDING_ORIGINAL]->Transform(*m_pOBB[BOUNDING_WORLD], WorldMatrix);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		m_pSPHERE[BOUNDING_ORIGINAL]->Transform(*m_pSPHERE[BOUNDING_WORLD], WorldMatrix);
		break;
	}
}

HRESULT CCollider::Render(void)
{
	m_pBatch->Begin();

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pEffect->SetView(pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CGameInstance);

	m_pEffect->Apply(m_pContext);

	_vector vColor = m_isCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	switch (m_eType)
	{
	case Engine::CCollider::TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABB[BOUNDING_WORLD], vColor);
		break;
	case Engine::CCollider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB[BOUNDING_WORLD], vColor);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSPHERE[BOUNDING_WORLD], vColor);
		break;
	}

	m_pBatch->End();

	return S_OK;
}

_bool CCollider::Collision(CCollider * pTargetCollider)
{
	m_isCollision = false;

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isCollision = m_pAABB[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pAABB[BOUNDING_WORLD]);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isCollision = m_pAABB[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pOBB[BOUNDING_WORLD]);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isCollision = m_pAABB[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pSPHERE[BOUNDING_WORLD]);
		break;
	case Engine::CCollider::TYPE_OBB:
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isCollision = m_pOBB[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pAABB[BOUNDING_WORLD]);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isCollision = m_pOBB[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pOBB[BOUNDING_WORLD]);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isCollision = m_pOBB[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pSPHERE[BOUNDING_WORLD]);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		if (TYPE_AABB == pTargetCollider->m_eType)
			m_isCollision = m_pSPHERE[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pAABB[BOUNDING_WORLD]);
		if (TYPE_OBB == pTargetCollider->m_eType)
			m_isCollision = m_pSPHERE[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pOBB[BOUNDING_WORLD]);
		if (TYPE_SPHERE == pTargetCollider->m_eType)
			m_isCollision = m_pSPHERE[BOUNDING_WORLD]->Intersects(*pTargetCollider->m_pSPHERE[BOUNDING_WORLD]);
		break;
	}
	return m_isCollision;
}

_bool CCollider::Collision_AABB(CCollider * pTargetCollider)
{
	_float3 vSourMin, vSourMax;
	_float3 vDestMin, vDestMax;

	vSourMin = Compute_Min();
	vSourMax = Compute_Max();

	vDestMin = pTargetCollider->Compute_Min();
	vDestMax = pTargetCollider->Compute_Max();

	m_isCollision = true;

	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		m_isCollision = false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		m_isCollision = false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		m_isCollision = false;

	return m_isCollision;
}

_bool CCollider::Collision_OBB(CCollider * pTargetCollider)
{
	OBBDESC OBBDesc[2];

	OBBDesc[0] = Compute_OBBDesc();
	OBBDesc[1] = pTargetCollider->Compute_OBBDesc();

	m_isCollision = true;

	_float fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vCenterAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vCenterAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vCenterAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vCenterAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vCenterAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vCenterAxis[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				m_isCollision = false;
		}
	}

	return m_isCollision;
}

void CCollider::Change_Scale(_float3 vScale)
{
	_matrix ScaleMatrix;

	ScaleMatrix = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		_float3 vCenter;

		switch (m_eType)
		{
		case TYPE_AABB:
			vCenter = m_pAABB[i]->Center;
			Safe_Delete(m_pAABB[i]);
			m_pAABB[i] = new BoundingBox(vCenter, _float3(0.5f, 0.5f, 0.5f));
			m_pAABB[i]->Transform(*m_pAABB[i], ScaleMatrix);
			break;
		case TYPE_SPHERE:
			vCenter = m_pSPHERE[i]->Center;
			Safe_Delete(m_pSPHERE[i]);
			m_pSPHERE[i] = new BoundingSphere(vCenter, 0.5f);
			m_pSPHERE[i]->Transform(*m_pSPHERE[i], ScaleMatrix);
			break;
		}
	}
}

_matrix CCollider::Remove_Rotation(_fmatrix Matrix)
{
	_matrix TransformMatrix = Matrix;

	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[0]));
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[1]));
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(Matrix.r[2]));

	return TransformMatrix;
}

_float3 CCollider::Compute_Min(void)
{
	if (nullptr == m_pAABB[BOUNDING_WORLD])
		return _float3(0.f, 0.f, 0.f);

	return _float3(m_pAABB[BOUNDING_WORLD]->Center.x - m_pAABB[BOUNDING_WORLD]->Extents.x,
		m_pAABB[BOUNDING_WORLD]->Center.y - m_pAABB[BOUNDING_WORLD]->Extents.y,
		m_pAABB[BOUNDING_WORLD]->Center.z - m_pAABB[BOUNDING_WORLD]->Extents.z);
}

_float3 CCollider::Compute_Max(void)
{
	if (nullptr == m_pAABB[BOUNDING_WORLD])
		return _float3(0.f, 0.f, 0.f);

	return _float3(m_pAABB[BOUNDING_WORLD]->Center.x + m_pAABB[BOUNDING_WORLD]->Extents.x,
		m_pAABB[BOUNDING_WORLD]->Center.y + m_pAABB[BOUNDING_WORLD]->Extents.y,
		m_pAABB[BOUNDING_WORLD]->Center.z + m_pAABB[BOUNDING_WORLD]->Extents.z);
}

CCollider::OBBDESC CCollider::Compute_OBBDesc(void)
{
	OBBDESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	_float3 vPoints[8];

	m_pOBB[BOUNDING_WORLD]->GetCorners(vPoints);

	XMStoreFloat3(&OBBDesc.vCenter, (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[4])) * 0.5f);

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[3])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[6])));

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], XMLoadFloat3(&OBBDesc.vAlignAxis[0]) * m_pOBB[BOUNDING_WORLD]->Extents.x);
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], XMLoadFloat3(&OBBDesc.vAlignAxis[1]) * m_pOBB[BOUNDING_WORLD]->Extents.y);
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], XMLoadFloat3(&OBBDesc.vAlignAxis[2]) * m_pOBB[BOUNDING_WORLD]->Extents.z);

	return OBBDesc;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		ERR_MSG(TEXT("Failed to Created : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free(void)
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);

	for (_uint i = 0; i < BOUNDING_END; ++i)
	{
		Safe_Delete(m_pAABB[i]);
		Safe_Delete(m_pOBB[i]);
		Safe_Delete(m_pSPHERE[i]);
	}
}
