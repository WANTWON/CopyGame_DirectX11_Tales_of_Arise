#pragma once

#include "Base.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	_bool Key_Pressing(_int _Key);
	_bool Key_Up(_int _Key);
	_bool Key_Down(_int _Key);

private:
	bool m_bKeyState[0xff];

public:
	virtual void Free(void);
};

END