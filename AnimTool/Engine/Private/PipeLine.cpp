#include "..\Public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

void CPipeLine::Tick(void)
{
	XMStoreFloat4(&m_vCamPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_PipeLineMatrix[D3DTS_VIEW])).r[3]);

	for (_uint i = 0; i < D3DTS_END; ++i)
		XMStoreFloat4x4(&m_PipeLineMatirx_TP[i], XMMatrixTranspose(XMLoadFloat4x4(&m_PipeLineMatrix[i])));
}

void CPipeLine::Free(void)
{
}

