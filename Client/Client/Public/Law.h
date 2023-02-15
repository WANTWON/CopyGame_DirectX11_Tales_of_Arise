#pragma once
#include "Player.h"

BEGIN(Client)
class CLaw final : public CPlayer
{
public:
	enum ANIM {
		BTL_ADVENT,
		BTL_ARISE_B,
		BTL_ARISE_F,
	};
private:
	CLaw();
	virtual ~CLaw();
};
END