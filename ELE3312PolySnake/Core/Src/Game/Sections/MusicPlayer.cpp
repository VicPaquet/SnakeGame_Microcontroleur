#include "Game/Sections/MusicPlayer.h"
#include "main.h"

// Initialiser le pointeur statique
MusicPlayer* MusicPlayer::instance = nullptr;

MusicPlayer::MusicPlayer() :
    sound(nullptr),
    currentMelody(nullptr),
    melodyLength(0),
    currentNoteIndex(0),
    noteStartTime(0),
    playing(false) {
    instance = this; // Enregistrer cette instance
}

void MusicPlayer::setup(DACSound* sound) {
    this->sound = sound;
}

void MusicPlayer::playMelody(const MusicNote* melody, uint16_t length) {
    if (!sound) return;

    currentMelody = melody;
    melodyLength = length;
    currentNoteIndex = 0;
    playing = true;

    playNextNote();
}

void MusicPlayer::playNextNote() {
    if (!sound || !currentMelody || currentNoteIndex >= melodyLength) {
        stop();
        return;
    }

    const MusicNote& note = currentMelody[currentNoteIndex];
    Note musicNote(note.name, note.octave);
    sound->setNote(musicNote);

    if (!sound->isEnabled()) {
        sound->enableSound();
    }

    noteStartTime = HAL_GetTick();
}

// *** CETTE FONCTION EST APPELÉE PAR L'INTERRUPTION TIMER ***
void MusicPlayer::timerCallback() {
    if (!playing || !sound) return;

    // Mettre à jour le son (remplir les buffers DMA)
    // C'est CRITIQUE que cela soit fait régulièrement !
    sound->update();
}

// *** CETTE FONCTION EST APPELÉE PAR LA BOUCLE PRINCIPALE ***
void MusicPlayer::update() {
    if (!playing || !sound) return;

    // Vérifier si la note actuelle est terminée
    uint32_t currentTime = HAL_GetTick();
    const MusicNote& note = currentMelody[currentNoteIndex];

    if ((currentTime - noteStartTime) >= note.duration_ms) {
        currentNoteIndex++;

        if (currentNoteIndex < melodyLength) {
            playNextNote();
        } else {
            currentNoteIndex = 0; // Boucler
            playNextNote();
        }
    }
}

void MusicPlayer::stop() {
    playing = false;
    if (sound && sound->isEnabled()) {
        sound->disableSound();
    }
}
