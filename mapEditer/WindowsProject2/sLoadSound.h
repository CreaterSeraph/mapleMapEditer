#pragma once
#include "sLoadData.h"

struct sLoadSound : public sLoadData
{
	sound* soundPtr;
	const SOUND_KIND kind;
	sLoadSound(const string& key, const tstring& path, const SOUND_KIND& kind, float volume = 1.0f);
	~sLoadSound();

	virtual bool Load() override;
};

