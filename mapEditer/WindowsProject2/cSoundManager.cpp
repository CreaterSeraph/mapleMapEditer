#include "stdafx.h"
#include "cSoundManager.h"

bool sound::Load()
{
	return SOUNDMANAGER->Load(this);
}

FMOD::Channel* sound::Play()
{
	return SOUNDMANAGER->Play(this);
}

cSoundManager::cSoundManager()
	:soundSystem(nullptr), masterChannelGroup(nullptr), masterSoundGroup(nullptr), maxChannel(100)
{
	channelGroup[SK_BGM] = nullptr;
	channelGroup[SK_EFFECT] = nullptr;
	Init();
}

cSoundManager::~cSoundManager()
{
	Release();
}

void cSoundManager::Init()
{
	FMOD::System_Create(&soundSystem);
	soundSystem->init(maxChannel, FMOD_INIT_NORMAL, 0);
	soundSystem->getMasterChannelGroup(&masterChannelGroup);
	soundSystem->getMasterSoundGroup(&masterSoundGroup);
	
	soundSystem->createChannelGroup("bgm", &channelGroup[SK_BGM]);
	soundSystem->createChannelGroup("effect", &channelGroup[SK_EFFECT]);

	masterChannelGroup->addGroup(channelGroup[SK_BGM]);
	masterChannelGroup->addGroup(channelGroup[SK_EFFECT]);

	masterVolume = 1.f;
	masterChannelGroup->setVolume(masterVolume);
	volume[SK_EFFECT] = 1.f;
	volume[SK_BGM] = 1.f;
}

void cSoundManager::Release()
{
	thread fadeOut([&]() {FadeOut(); });
	fadeOut.join();

	for (auto& iter : mSounds)
		SAFE_DELETE(iter.second);

	channelGroup[SK_EFFECT]->release();
	channelGroup[SK_BGM]->release();
	masterChannelGroup->release();

	masterSoundGroup->release();

	soundSystem->close();
	soundSystem->release();
}

void cSoundManager::FadeOut()
{
}

void cSoundManager::Update()
{
	soundSystem->update();
}

void cSoundManager::PauseAll()
{
	masterChannelGroup->setPaused(true);
}

void cSoundManager::ResumeAll()
{
	masterChannelGroup->setPaused(false);
}

bool cSoundManager::Load(sound* soundPtr)
{
	FMOD::Sound* temp;
	FMOD_RESULT result;

	string filePath;
	filePath.assign(soundPtr->filePath.begin(), soundPtr->filePath.end());
	if (soundPtr->kind == SK_BGM)
	{
		result = soundSystem->createStream(filePath.c_str(), FMOD_LOOP_NORMAL, 0, &temp);

		if (result != FMOD_OK)//사운드 로딩 실패
		{
			DEBUG_LOG("Sound Load Failed");
			return false;
		}
	}
	else
	{
		result = soundSystem->createSound(filePath.c_str(), FMOD_DEFAULT, 0, &temp);

		if (result != FMOD_OK)//사운드 로딩 실패
		{
			DEBUG_LOG("Sound Load Failed");
			return false;
		}

		FMOD::ChannelGroup* group;
		soundSystem->createChannelGroup(filePath.c_str(), &group);
		channelGroup[SK_EFFECT]->addGroup(group);

		((effectSound*)soundPtr)->group = group;
	}

	temp->setSoundGroup(masterSoundGroup);
	soundPtr->soundPtr = temp;
	return true;
}

bool cSoundManager::AddSound(const string& key, sound* soundPtr)
{
	auto find = mSounds.find(key);
	if (find == mSounds.end())
	{
		mSounds.insert(make_pair(key, soundPtr));
		return true;
	}
#ifdef _DEBUG
	else if (find->second->filePath != soundPtr->filePath)//이미 있는 키값인데 파일 경로가 다를경우
	{
		DEBUG_LOG("key :" << key);
		DEBUG_LINE_W(L"\tpath1 :" << find->second->filePath << L"\tpath2 :" << soundPtr->filePath);
	}
#endif
	return false;
}

sound* cSoundManager::FindSound(const string& key)
{
	auto find = mSounds.find(key);
	if (find == mSounds.end()) return nullptr;
	return find->second;
}

bool cSoundManager::DeleteSound(const string& key)
{
	auto find = mSounds.find(key);
	if (find == mSounds.end()) return false;

	find->second->Release();

	return true;
}

void cSoundManager::FailedLoad(const string& key)
{
	auto find = mSounds.find(key);
	if (find == mSounds.end()) return;

	assert(find->second->soundPtr);

	mSounds.erase(find);
}

FMOD::Channel* cSoundManager::Play(sound* soundPtr)
{
	if (!soundPtr->soundPtr) return nullptr;
	FMOD::Channel* channel;

	if (soundPtr->kind == SK_EFFECT)
	{
		soundSystem->playSound(soundPtr->soundPtr, ((effectSound*)soundPtr)->group, false, &channel);
	}
	else
	{
		soundSystem->playSound(soundPtr->soundPtr, channelGroup[SK_BGM], false, &channel);
		((bgmSound*)soundPtr)->channel = channel;
	}
	channel->setVolume(soundPtr->volume);

	return channel;
}

void cSoundManager::SetVolume(SOUND_KIND kind, float volume)
{
	channelGroup[kind]->setVolume(volume);
}

void cSoundManager::SetMasterVolume(float volume)
{
	masterChannelGroup->setVolume(volume);
}
