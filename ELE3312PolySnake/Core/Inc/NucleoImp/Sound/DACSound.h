#ifndef DACSOUND_H_
#define DACSOUND_H_

#include "main.h"
#include "Interfaces/Sound/Sound.h"

#ifdef __cplusplus
extern "C" {
#endif
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac);
void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac);
#ifdef __cplusplus
}
#endif

class MusicPlayer;

class DACSound : public Sound {
public:
    DACSound();
    void setup(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, uint64_t base_freq);
    virtual ~DACSound() = default;

    void enableSound() override;
    void disableSound() override;
    void setFrequency(float freq) override;
    void setNote(const Note &note) override;
    void update() override;
    void setAmplitude(uint16_t value) override;
    static void setMusicPlayerInstance(MusicPlayer* player);
    uint16_t getAmplitude() const override;

    // Nouvelles méthodes pour le double buffer
    void fillNextHalfBuffer(bool firstHalf);

    volatile static uint16_t dacCpltEvent;
    volatile static uint16_t dacHalfCpltEvent;

private:
    DAC_HandleTypeDef *hdac = nullptr;
    TIM_HandleTypeDef *htim = nullptr;
    float sampleFrequency = 0.0;
    float currentFrequency = 440.0;
    uint64_t baseFreq;

    // BUFFER DE TAILLE FIXE
    constexpr static uint32_t BUFFER_SIZE = 2048; // Taille fixe du buffer DMA
    constexpr static uint32_t WAVE_TABLE_SIZE = 256; // Taille de la table d'onde
    constexpr static float pi = 3.1415926535f;

    uint32_t dmaBuffer[BUFFER_SIZE]; // Buffer circulaire pour le DMA
    uint32_t waveTable[WAVE_TABLE_SIZE]; // Table contenant UNE période

    float phaseAccumulator = 0.0f; // Accumulateur de phase
    float phaseIncrement = 0.0f;   // Incrément de phase par échantillon

    Note currentNote;

    // Génération de la table d'onde (une seule période)
    void generateWaveTable();
    static MusicPlayer* musicPlayerInstance;

    // Calcul de l'incrément de phase
    void updatePhaseIncrement();
};

#endif
