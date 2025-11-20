#include "NucleoImp/Sound/DACSound.h"
#include <cstdint>
#include <cmath>
#include "Game/Sections/MusicPlayer.h"

#ifdef __cplusplus
extern "C" {
#endif

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac) {
    DACSound::dacCpltEvent = 1;
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac) {
    DACSound::dacHalfCpltEvent = 1;
}

#ifdef __cplusplus
}
#endif

volatile uint16_t DACSound::dacCpltEvent = 0;
volatile uint16_t DACSound::dacHalfCpltEvent = 0;

DACSound::DACSound() {
	baseFreq = 0;
    soundEnabled = false;
}

MusicPlayer* DACSound::musicPlayerInstance = nullptr;

void DACSound::setMusicPlayerInstance(MusicPlayer* player) {
    musicPlayerInstance = player;
}

void DACSound::setup(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, uint64_t baseFreq) {
    this->hdac = hdac;
    this->htim = htim;
    this->baseFreq = baseFreq;

    // Calculer la fréquence d'échantillonnage
    sampleFrequency = (baseFreq / (htim->Instance->PSC + 1)) / htim->Instance->ARR;

    waveform = Waveform::Sinus;

    // Générer la table d'onde (une seule période)
    generateWaveTable();

    // Calculer l'incrément de phase initial
    updatePhaseIncrement();

    // Remplir le buffer initial
    fillNextHalfBuffer(true);  // Première moitié
    fillNextHalfBuffer(false); // Deuxième moitié

    HAL_TIM_Base_Start(htim);
}

void DACSound::generateWaveTable() {
    for (uint32_t i = 0; i < WAVE_TABLE_SIZE; i++) {
        float phase = (2.0f * pi * i) / WAVE_TABLE_SIZE;

        switch (waveform) {
            case Waveform::Sinus: {
                float sinValue = sinf(phase); // [-1, 1] - utilisez sinf() pour float
                float normalized = (sinValue + 1.0f) / 2.0f; // [0, 1]
                waveTable[i] = (uint32_t)(normalized * 4095.0f * amplitude / 100);

                // S'assurer qu'on ne dépasse pas 4095
                if (waveTable[i] > 4095) waveTable[i] = 4095;
                break;
            }  // ← AJOUTER }

            case Waveform::Square: {
                waveTable[i] = (i < WAVE_TABLE_SIZE / 2) ? (4095 * amplitude / 100) : 0;
                break;
            }

            default:
                waveTable[i] = 2048;
                break;
        }
    }
}
void DACSound::updatePhaseIncrement() {
    // Incrément de phase = (fréquence_note / fréquence_échantillonnage) * taille_table
    phaseIncrement = (currentFrequency / sampleFrequency) * WAVE_TABLE_SIZE;
}

void DACSound::fillNextHalfBuffer(bool firstHalf) {
    uint32_t startIndex = firstHalf ? 0 : (BUFFER_SIZE / 2);
    uint32_t endIndex = startIndex + (BUFFER_SIZE / 2);

    float localPhase = phaseAccumulator;

    for (uint32_t i = startIndex; i < endIndex; i++) {
        // Convertir directement en index (localPhase est toujours < 256)
        uint32_t tableIndex = (uint32_t)localPhase;
        dmaBuffer[i] = waveTable[tableIndex];

        // Incrémenter la phase
        localPhase += phaseIncrement;

        // Garder dans [0, 256) avec soustraction (plus rapide que modulo)
        if (localPhase >= (float)WAVE_TABLE_SIZE) {
            localPhase -= (float)WAVE_TABLE_SIZE;
        }
    }

    phaseAccumulator = localPhase;
}

void DACSound::enableSound() {
    HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, dmaBuffer, BUFFER_SIZE, DAC_ALIGN_12B_R);
    soundEnabled = true;
}

void DACSound::disableSound() {
    HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1);
    soundEnabled = false;
}

void DACSound::setFrequency(float freq) {
    currentFrequency = freq;
    updatePhaseIncrement();
}

void DACSound::setNote(const Note &note) {
    // Option : Baisser brièvement le volume pour éviter les clics
    uint16_t oldAmplitude = amplitude;
    setAmplitude(0); // Fade out rapide

    currentNote = note;
    setFrequency(currentNote.getFrequency());

    for (volatile int i = 0; i < 1000; i++); // Délai très court

    setAmplitude(oldAmplitude); // Fade in
}

void DACSound::update() {
    // Désactiver brièvement les interruptions pendant la vérification
    __disable_irq();
    bool halfEvent = (dacHalfCpltEvent == 1);
    bool fullEvent = (dacCpltEvent == 1);

    if (halfEvent) dacHalfCpltEvent = 0;
    if (fullEvent) dacCpltEvent = 0;
    __enable_irq();

    // Traiter les événements APRÈS avoir réactivé les interruptions
    if (halfEvent) {
        fillNextHalfBuffer(true); // Remplir la première moitié
    }

    if (fullEvent) {
        fillNextHalfBuffer(false); // Remplir la deuxième moitié
    }
}

void DACSound::setAmplitude(uint16_t value) {
    amplitude = value <= 100 ? value : 100;
    generateWaveTable(); // Regénérer la table avec la nouvelle amplitude
}

uint16_t DACSound::getAmplitude() const {
    return amplitude;
}
