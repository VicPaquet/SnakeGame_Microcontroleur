#ifndef DACSOUND_H_
#define DACSOUND_H_
/**
  * @file DACSound.h
  * @date June 2025
  * @brief This file contains the declaration of the DACSound class that implements the Sound interface
  * that is used to generate sound through the micro-controllers digital to analog converter (DAC).
  * 
  * @defgroup ELE3312
  * @{
  */

#include "main.h"
#include "Interfaces/Sound/Sound.h"



#ifdef __cplusplus
extern "C" {
#endif
 void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac);
#ifdef __cplusplus
}
#endif

namespace ELE3312 {

class DACSound : public Sound{
public:
	DACSound();
	void setup(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, uint64_t base_freq);
	virtual ~DACSound() = default;
	void enableSound() override;
	void disableSound() override;
	void setFrequency(float freq) override;
	void setNote(const Note &note) override;
	void update() override;
	volatile static uint16_t dacCpltEvent;
	virtual void setAmplitude(uint16_t value) override;
	virtual uint16_t getAmplitude() const  override;
private:
	DAC_HandleTypeDef *hdac;
	TIM_HandleTypeDef *htim;
	float sampleFrequency = 0.0;
	float currentFrequency = 440.0; // this is an arbitrary starting frequency
	uint64_t baseFreq;
	constexpr static uint32_t sampleBufferSize{2058};
	constexpr static float pi{3.1415926535f};
	uint32_t sampleBuffer1[sampleBufferSize] = {0};
	uint32_t sampleBuffer2[sampleBufferSize] = {0};
	uint32_t *currentBuffer {sampleBuffer1};
	uint32_t sampleBuffer1Length{0};
	uint32_t sampleBuffer2Length{0};
	uint32_t *sampleBufferLength{&sampleBuffer1Length};
	bool needUpdate{false};
	Note currentNote;

	int generateWaveLUT(uint32_t *buffer, uint32_t *bufferLength);
};

} /* namespace ELE3312 */

#endif /* SOUND_H_ */
