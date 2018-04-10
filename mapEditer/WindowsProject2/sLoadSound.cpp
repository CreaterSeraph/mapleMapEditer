#include "stdafx.h"
#include "sLoadSound.h"


sLoadSound::sLoadSound(const string& key, const tstring& path, const SOUND_KIND& kind, float volume)
	:sLoadData(key, path, LK_SOUND), kind(kind), soundPtr(nullptr)
{
	if (kind == SK_BGM)
	{
		soundPtr = new bgmSound(path, volume);
	}
	else
	{
		soundPtr = new effectSound(path, volume);
	}
}


sLoadSound::~sLoadSound()
{
}

bool sLoadSound::Load()
{
	return soundPtr->Load();
}
