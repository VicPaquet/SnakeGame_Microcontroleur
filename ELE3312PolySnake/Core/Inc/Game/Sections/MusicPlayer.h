#ifndef MUSICPLAYER_H_
#define MUSICPLAYER_H_

#include "Interfaces/Sound/Note.h"
#include "NucleoImp/Sound/DACSound.h"
#include <cstdint>

struct MusicNote {
    NoteName name;
    uint16_t octave;
    uint32_t duration_ms;
};

class MusicPlayer {
public:
    MusicPlayer();
    void setup(DACSound* sound);
    void playMelody(const MusicNote* melody, uint16_t length);
    void update();
    void stop();
    bool isPlaying() const { return playing; }

    // Nouvelle méthode pour l'interruption
    void timerCallback();

    // Singleton pour accès depuis C
    static MusicPlayer* getInstance() { return instance; }

private:
    DACSound* sound;
    const MusicNote* currentMelody;
    uint16_t melodyLength;
    uint16_t currentNoteIndex;
    uint32_t noteStartTime;
    bool playing;

    static MusicPlayer* instance;

    void playNextNote();
};

#endif
