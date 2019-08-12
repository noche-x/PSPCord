#pragma once
#include "sound_utils/audio.h"
#include "sound_utils/VirtualFile.h"
#include "sound_utils/pgeWav.h"

//WE HAVE 8 CHANNELS TO WORK WITH
enum SoundChannel {
	    AUDIO_CHANNEL_MUSIC1 = 0,
	    AUDIO_CHANNEL_MUSIC2 = 1,
	    AUDIO_CHANNEL_GUI1 = 2,
	    AUDIO_CHANNEL_GUI2 = 3,
	    AUDIO_CHANNEL_GUI3 = 4,
	    AUDIO_CHANNEL_GUI4 = 5,
	    AUDIO_CHANNEL_MISC1 = 6,
	    AUDIO_CHANNEL_MISC2 = 7
};

class AudioManager {
    public:
	void Init();
    void Update();
	void PlaySound(OSL_SOUND* sound, SoundChannel channel);
    void PlayMusic(OSL_SOUND* music);
};

extern AudioManager g_AudioManager;