#pragma once
#include "singleton.h"

#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod.hpp"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod_codec.h"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod_common.h"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod_dsp.h"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod_dsp_effects.h"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod_errors.h"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc/fmod_output.h"

#ifdef _x64
#ifdef _UNICODE
#pragma comment( lib, "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/fmodL64_vc.lib" )
#else
#pragma comment( lib, "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/fmod64_vc.lib" )
#endif
#else
#ifdef _UNICODE
#pragma comment( lib, "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/fmodL_vc.lib" )
#else
#pragma comment( lib, "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/fmod_vc.lib" )
#endif
#endif

struct sound
{
	const SOUND_KIND kind;
	FMOD::Sound* soundPtr;
	tstring filePath;
	float volume;

	sound(const tstring& filePath, const SOUND_KIND kind, float volume)
		:filePath(filePath), soundPtr(nullptr), kind(kind), volume(volume)
	{
	}

	bool Load();

	virtual ~sound()
	{
		Release();
	}

	FMOD::Channel* Play();
	virtual void Stop() PURE;
	virtual void Resume() PURE;
	virtual void Pause() PURE;

	virtual bool IsPlay() PURE;
	virtual bool IsPaused() PURE;

	virtual void Release()
	{
		if (soundPtr)
			soundPtr->release();
	}
};

struct effectSound : public sound
{
	FMOD::ChannelGroup* group;

	effectSound(const tstring& filePath, float volume = 1.f)
		:sound(filePath, SK_EFFECT, volume), group(nullptr)
	{
	}

	~effectSound()
	{
	}

	virtual void Stop() override
	{
		if (group)
			group->stop();
	}
	virtual void Resume() override
	{
		if (group)
			group->setPaused(false);
	}
	virtual void Pause() override
	{
		if (group)
			group->setPaused(true);
	}

	virtual bool IsPlay() override
	{
		if (group)
		{
			bool result;
			group->isPlaying(&result);
			return result;
		}
		return false;
	}
	virtual bool IsPaused() override
	{
		if (group)
		{
			bool result;
			group->getPaused(&result);
			return result;
		}
		return false;	
	}

	virtual void Release() override
	{
		sound::Release();
		if (group)
			group->release();
		group = nullptr;
	}
};

struct bgmSound : public sound
{
	FMOD::Channel* channel;
	
	bgmSound(const tstring& filePath, float volume = 1.f)
		:sound(filePath, SK_BGM, volume), channel(nullptr)
	{
	}

	~bgmSound()
	{
	}

	virtual void Stop() override
	{
		if (channel)
			channel->stop();
		channel = nullptr;
	}
	virtual void Resume() override
	{
		if (channel)
			channel->setPaused(false);
	}
	virtual void Pause() override
	{
		if (channel)
			channel->setPaused(true);
	}

	virtual bool IsPlay() override
	{
		bool result = false;
		if (channel)
			channel->isPlaying(&result);
		return result;
	}
	virtual bool IsPaused() override
	{
		bool result = false;
		if (channel)
			channel->getPaused(&result);
		return result;
	}
};

class cSoundManager : public singleton<cSoundManager>
{
private:
	const int maxChannel;

	FMOD::System* soundSystem;

	FMOD::ChannelGroup* masterChannelGroup;
	FMOD::ChannelGroup* channelGroup[SK_END];
	FMOD::SoundGroup* masterSoundGroup;

	float volume[SK_END];
	float masterVolume;

	map<string, sound*> mSounds;
private:
	void Init();
	void Release();

	void FadeOut();
public:
	void Update();
	void PauseAll();
	void ResumeAll();

	bool Load(sound* soundPtr);

	bool AddSound(const string& key, sound* soundPtr);
	sound* FindSound(const string& key);

	bool DeleteSound(const string& key);
	void FailedLoad(const string& key);

	FMOD::Channel* Play(sound* soundPtr);

	void SetVolume(SOUND_KIND kind, float volume = 1.f);
	void SetMasterVolume(float volume = 1.f);

	cSoundManager();
	~cSoundManager();
};

#define SOUNDMANAGER cSoundManager::GetInstance()