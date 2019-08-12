#include "AudioManager.hpp"

void AudioManager::Init(){
    VirtualFileInit();
    oslInitAudio();
}

void AudioManager::Update()
{
    oslAudioVSync();
}

void AudioManager::PlaySound(OSL_SOUND * sound, SoundChannel channel)
{
    oslPlaySound(sound, channel);
}

AudioManager g_AudioManager = AudioManager();
