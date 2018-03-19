#pragma once
#include "singleton.h"

class cRandManager : public singleton<cRandManager>
{
private:

public:
	cRandManager();
	~cRandManager();
};

#define RAND cRandManager::GetInstance()