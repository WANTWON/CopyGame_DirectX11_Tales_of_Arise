#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
public:
	CBase();
	virtual ~CBase() = default;

public:
	unsigned long AddRef(void);
	unsigned long Release(void);

private:
	unsigned long m_dwRefCnt = 0;

public:
	virtual void Free(void) = 0;
};

END