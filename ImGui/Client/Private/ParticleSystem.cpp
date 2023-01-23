#include "stdafx.h"

#include "ParticleSystem.h"
#include "GameInstance.h"

CParticleSystem::CParticleSystem(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem & rhs)
	: CEffect(rhs)
{
}

HRESULT CParticleSystem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSystem::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	InitializeParticleSystem();
	InitializeBuffers();

	return S_OK;
}

int CParticleSystem::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	KillParticles();					/* Release old Particles. */

	if (m_bPlay)
		EmitParticles(fTimeDelta);		/* Emit new Particles.	*/

	UpdateParticles(fTimeDelta);		/* Update Particles. */

	UpdateBuffers();
	
	return OBJ_NOEVENT;
}

void CParticleSystem::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
	{
		/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);*/		/* Non-Alphablend */
		/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLENDLIGHTS, this);*/	/* Alphablend with Normals */
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);			/* Alphablend */

		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	}
}

HRESULT CParticleSystem::Render()
{
	if (!m_pVertexBuffer || !m_pShaderCom)
		return E_FAIL;

	__super::Render();

	m_pShaderCom->Begin(0);
	RenderBuffers();

	return S_OK;
}

HRESULT CParticleSystem::Ready_Components(void * pArg)
{
	__super::Ready_Components(pArg);

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 0.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, m_tEffectDesc.wcTexturePrototypeId, (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleSystem::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticleSystem::InitializeParticleSystem()
{
	/* Create the Particle List. */
	m_Particles = new ParticleType[m_tParticleDesc.m_iMaxParticles];

	/* Initialize the Particle List. */
	for (_int i = 0; i < m_tParticleDesc.m_iMaxParticles; i++)
		m_Particles[i].bActive = false;

	/* Initialize the current Particle Count to zero since none are emitted yet. */
	m_fCurrentParticleCount = 0;

	/* Clear the initial Accumulated Time. */
	m_fAccumulatedTime = 0.f;

	return S_OK;
}

HRESULT CParticleSystem::InitializeBuffers()
{
	unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	/* Set the maximum number of vertices in the vertex array. */
	m_iVertexCount = m_tParticleDesc.m_iMaxParticles * 6;

	/* Set the maximum number of indices in the index array. */
	m_iIndexCount = m_iVertexCount;

	/* Create the vertex array for the particles that will be rendered. */
	m_Vertices = new VertexType[m_iVertexCount];

	/* Create the index array. */
	indices = new unsigned long[m_iIndexCount];

	/* Initialize vertex array to zeros at first. */
	memset(m_Vertices, 0, (sizeof(VertexType) * m_iVertexCount));

	/* Initialize the index array. */
	for (i = 0; i < m_iIndexCount; i++)
		indices[i] = i;

	/* Set up the description of the dynamic vertex buffer. */
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	/* Give the subresource structure a pointer to the vertex data. */
	vertexData.pSysMem = m_Vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	/* Now finally create the vertex buffer. */
	m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	/* Set up the description of the static index buffer. */
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	/* Give the subresource structure a pointer to the index data. */
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	/* Create the index buffer. */
	m_pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);

	/* Release the index array since it is no longer needed. */
	Safe_Delete_Array(indices);

	return S_OK;
}

void CParticleSystem::EmitParticles(_float fTimeDelta)
{
	bool bEmitParticle, bFound;
	float fPositionX, fPositionY, fPositionZ, fRed, fGreen, fBlue, fVelocity, fSize;
	int iIndex, i, j;

	/* Increment the frame time. */
	m_fAccumulatedTime += fTimeDelta;

	/* Set emit particle to false for now. */
	bEmitParticle = false;

	/* Check if it is time to emit a new particle or not. */
	if (m_fAccumulatedTime > (1.f / m_tParticleDesc.m_fParticlesPerSecond))
	{
		m_fAccumulatedTime = 0.f;
		bEmitParticle = true;
	}

	/* If there are Particles to emit then emit one per frame. */
	if ((bEmitParticle == true) && (m_fCurrentParticleCount < (m_tParticleDesc.m_iMaxParticles - 1)))
	{
		m_fCurrentParticleCount++;

		/* Now generate the randomized particle properties. */
		fPositionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_tParticleDesc.m_fParticleDeviationX;
		fPositionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_tParticleDesc.m_fParticleDeviationY;
		fPositionZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_tParticleDesc.m_fParticleDeviationZ;

		fRed = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		fGreen = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		fBlue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		fVelocity = m_tParticleDesc.m_fParticleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_tParticleDesc.m_fParticleVelocityVariation;
		
		fSize = m_tParticleDesc.m_fParticleSize + (((float)rand() - (float)rand()) / RAND_MAX) * m_tParticleDesc.m_fParticleSizeVariation;

		/* Now since the Particles need to be rendered from back to front for blending we have to sort the particle array.
		We will sort using Z depth so we need to find where in the list the particle should be inserted. */
		iIndex = 0;
		bFound = false;
		while (!bFound)
		{
			if ((m_Particles[iIndex].bActive == false) || (m_Particles[iIndex].fPositionZ < fPositionZ))
				bFound = true;
			else
				iIndex++;
		}

		/* Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle. */
		i = m_fCurrentParticleCount;
		j = i - 1;

		while (i != iIndex)
		{
			m_Particles[i].fPositionX = m_Particles[j].fPositionX;
			m_Particles[i].fPositionY = m_Particles[j].fPositionY;
			m_Particles[i].fPositionZ = m_Particles[j].fPositionZ;
			m_Particles[i].fRed = m_Particles[j].fRed;
			m_Particles[i].fGreen = m_Particles[j].fGreen;
			m_Particles[i].fBlue = m_Particles[j].fBlue;
			m_Particles[i].fVelocity = m_Particles[j].fVelocity;
			m_Particles[i].fSize = m_Particles[j].fSize;
			m_Particles[i].fLife = m_Particles[j].fLife;
			m_Particles[i].bActive = m_Particles[j].bActive;

			i--;
			j--;
		}

		/* Now insert it into the Particle array in the correct depth order. */
		m_Particles[iIndex].fPositionX = fPositionX;
		m_Particles[iIndex].fPositionY = fPositionY;
		m_Particles[iIndex].fPositionZ = fPositionZ;
		m_Particles[iIndex].fRed = fRed;
		m_Particles[iIndex].fGreen = fGreen;
		m_Particles[iIndex].fBlue = fBlue;
		m_Particles[iIndex].fVelocity = fVelocity;
		m_Particles[iIndex].fSize = fSize;
		m_Particles[iIndex].fLife = 0.f;
		m_Particles[iIndex].bActive = true;
	}
}

void CParticleSystem::UpdateParticles(_float fTimeDelta)
{
	/* Each frame we update all the Particles by making them move using their Position, Velocity, and TimeDelta. */
	for (_uint i = 0; i < m_fCurrentParticleCount; i++)
	{
		m_Particles[i].fPositionY = m_Particles[i].fPositionY + (m_Particles[i].fVelocity * fTimeDelta);
		m_Particles[i].fLife += fTimeDelta;
	}

	return;
}

void CParticleSystem::KillParticles()
{
	for (_uint i = 0; i < m_tParticleDesc.m_iMaxParticles; i++)
	{
		if ((m_Particles[i].bActive == true) && (m_Particles[i].fLife > m_tParticleDesc.m_fParticlesLifetime))
		{
			m_Particles[i].bActive = false;
			m_fCurrentParticleCount--;

			/* Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly. */
			for (_uint j = i; j < m_tParticleDesc.m_iMaxParticles - 1; j++)
			{
				m_Particles[j].fPositionX = m_Particles[j + 1].fPositionX;
				m_Particles[j].fPositionY = m_Particles[j + 1].fPositionY;
				m_Particles[j].fPositionZ = m_Particles[j + 1].fPositionZ;
				m_Particles[j].fRed = m_Particles[j + 1].fRed;
				m_Particles[j].fGreen = m_Particles[j + 1].fGreen;
				m_Particles[j].fBlue = m_Particles[j + 1].fBlue;
				m_Particles[j].fVelocity = m_Particles[j + 1].fVelocity;
				m_Particles[j].fSize = m_Particles[j + 1].fSize;
				m_Particles[j].fLife = m_Particles[j + 1].fLife;
				m_Particles[j].bActive = m_Particles[j + 1].bActive;
			}
		}
	}

	return;
}

HRESULT CParticleSystem::UpdateBuffers()
{
	int index, i;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	/* Initialize vertex array to zeros at first. */
	memset(m_Vertices, 0, (sizeof(VertexType) * m_iVertexCount));

	/* Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles. */
	index = 0;

	for (i = 0; i < m_fCurrentParticleCount; i++)
	{
		/* Bottom Left */
		m_Vertices[index].vPosition = _float3(m_Particles[i].fPositionX - m_Particles[i].fSize, m_Particles[i].fPositionY - m_Particles[i].fSize, m_Particles[i].fPositionZ);
		m_Vertices[index].vTexture = _float2(0.0f, 1.0f);
		m_Vertices[index].vColor = _float4(m_Particles[i].fRed, m_Particles[i].fGreen, m_Particles[i].fBlue, 1.0f);
		index++;
		/* Top Left */
		m_Vertices[index].vPosition = _float3(m_Particles[i].fPositionX - m_Particles[i].fSize, m_Particles[i].fPositionY + m_Particles[i].fSize, m_Particles[i].fPositionZ);
		m_Vertices[index].vTexture = _float2(0.0f, 0.0f);
		m_Vertices[index].vColor = _float4(m_Particles[i].fRed, m_Particles[i].fGreen, m_Particles[i].fBlue, 1.0f);
		index++;
		/* Bottom Right */
		m_Vertices[index].vPosition = _float3(m_Particles[i].fPositionX + m_Particles[i].fSize, m_Particles[i].fPositionY - m_Particles[i].fSize, m_Particles[i].fPositionZ);
		m_Vertices[index].vTexture = _float2(1.0f, 1.0f);
		m_Vertices[index].vColor = _float4(m_Particles[i].fRed, m_Particles[i].fGreen, m_Particles[i].fBlue, 1.0f);
		index++;
		/* Bottom right. */
		m_Vertices[index].vPosition = _float3(m_Particles[i].fPositionX + m_Particles[i].fSize, m_Particles[i].fPositionY - m_Particles[i].fSize, m_Particles[i].fPositionZ);
		m_Vertices[index].vTexture = _float2(1.0f, 1.0f);
		m_Vertices[index].vColor = _float4(m_Particles[i].fRed, m_Particles[i].fGreen, m_Particles[i].fBlue, 1.0f);
		index++;
		/* Top left. */
		m_Vertices[index].vPosition = _float3(m_Particles[i].fPositionX - m_Particles[i].fSize, m_Particles[i].fPositionY + m_Particles[i].fSize, m_Particles[i].fPositionZ);
		m_Vertices[index].vTexture = _float2(0.0f, 0.0f);
		m_Vertices[index].vColor = _float4(m_Particles[i].fRed, m_Particles[i].fGreen, m_Particles[i].fBlue, 1.0f);
		index++;
		/* Top right. */
		m_Vertices[index].vPosition = _float3(m_Particles[i].fPositionX + m_Particles[i].fSize, m_Particles[i].fPositionY + m_Particles[i].fSize, m_Particles[i].fPositionZ);
		m_Vertices[index].vTexture = _float2(1.0f, 0.0f);
		m_Vertices[index].vColor = _float4(m_Particles[i].fRed, m_Particles[i].fGreen, m_Particles[i].fBlue, 1.0f);
		index++;
	}

	/* Lock the Vertex Buffer. */
	result = m_pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	/* Get a pointer to the data in the vertex buffer. */
	verticesPtr = (VertexType*)mappedResource.pData;

	/* Copy the data into the Vertex Buffer. */
	memcpy(verticesPtr, (void*)m_Vertices, (sizeof(VertexType) * m_iVertexCount));

	/* Unlock the vertex buffer. */
	m_pContext->Unmap(m_pVertexBuffer, 0);

	return S_OK;
}

void CParticleSystem::RenderBuffers()
{
	unsigned int stride;
	unsigned int offset;

	/* Set Vertex Buffer stride and offset. */
	stride = sizeof(VertexType);
	offset = 0;

	/* Set the vertex buffer to active in the input assembler so it can be rendered. */
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	/* Set the index buffer to active in the input assembler so it can be rendered. */
	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	/* Set the type of primitive that should be rendered from this vertex buffer. */
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Draw */
	m_pContext->DrawIndexed(m_iIndexCount, 0, 0);

	return;
}

CParticleSystem * CParticleSystem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CParticleSystem* pInstance = new CParticleSystem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CParticleSystem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CParticleSystem::Clone(void * pArg)
{
	CParticleSystem* pInstance = new CParticleSystem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CParticleSystem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleSystem::Free()
{
	__super::Free();

	Safe_Release(m_pVertexBuffer);
	Safe_Release(m_pIndexBuffer);
	
	Safe_Delete_Array(m_Particles);
	Safe_Delete_Array(m_Vertices);

	Safe_Release(m_pTextureCom);
}