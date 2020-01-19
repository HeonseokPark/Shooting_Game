#include "stdafx.h"
#include "SoundManager.h"


SoundManager::SoundManager()
	:m_system(nullptr)
	, m_channel(nullptr)
	, m_sound(nullptr)
{
	//사운드 시스템 생성
	System_Create(&m_system);

	//사운드 채널수 생성
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 동적할당
	m_sound = new Sound *[TOTALSOUNDBUFFER];
	m_channel = new Channel *[TOTALSOUNDBUFFER];

	memset(m_sound, 0, sizeof(Sound*)* (TOTALSOUNDBUFFER));
	memset(m_channel, 0, sizeof(Channel*)* (TOTALSOUNDBUFFER));
}


SoundManager::~SoundManager()
{
	if (m_channel != nullptr || m_sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_channel != nullptr)
			{
				if (m_channel[i]) m_channel[i]->stop();
			}

			if (m_sound != nullptr)
			{
				if (m_sound[i]) m_sound[i]->release();
			}
		}
	}

	SAFE_DELETE(m_channel);
	SAFE_DELETE(m_sound);

	//시스템 닫기
	if (m_system != nullptr)
	{
		m_system->release();
		m_system->close();
	}
	//맵 비우기
	m_totalSound.clear();
}

void SoundManager::addSound(string keyName, string soundName, bool loop)
{

	if (loop)
	{
		m_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSound.size()]);

	}
	else
	{
		m_system->createStream(soundName.c_str(), FMOD_DEFAULT, nullptr, &m_sound[m_totalSound.size()]);
	}

	//맵에 사운드를 키값과 함께 담아준다.
	m_totalSound.insert(make_pair(keyName, &m_sound[m_totalSound.size()]));
}

void SoundManager::play(string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이
			m_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_channel[count]);
			//볼륨셋팅
			m_channel[count]->setVolume(volume);
		}
	}
}

void SoundManager::stop(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::isPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

bool SoundManager::isPauseSound(string keyName)
{
	return false; bool isPause;
	int count = 0;
	arrSoundIter iter = m_totalSound.begin();
	for (iter; iter != m_totalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}

void SoundManager::Update()
{
	/*
	사운드 시스템을 계속 업데이트 해줘야 볼륨이 바뀌거나
	재생이 끝난 사운드를 채널에서 빼내는 등 다양한 작업을 자동으로 해준다.
	*/
	//메인게임에서 사운드 매니져를 업데이트 해준다.
	m_system->update();
}
