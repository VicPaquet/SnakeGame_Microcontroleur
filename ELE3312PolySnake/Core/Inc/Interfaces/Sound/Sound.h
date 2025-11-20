#ifndef SOUND_H_
#define SOUND_H_

#include "Interfaces/Sound/Note.h"

/** @brief Defines different wave forms for the sound creation.
  */
enum class Waveform {
	Sinus, Square
};
/** @brief The Sound class defines an abstract interface for the creation of sound.
  */
class Sound {
public:
	virtual ~Sound() = default;
	virtual void enableSound() = 0;
	virtual void disableSound() = 0;
	virtual void setFrequency(float freq) = 0;
	virtual void setNote(const Note &note) = 0;
	virtual void update() = 0;
	virtual void setWaveform(Waveform newWaveform) { waveform = newWaveform; }
	virtual bool isEnabled() const { return soundEnabled; }
	virtual void setAmplitude(uint16_t value) = 0;
	virtual uint16_t getAmplitude() const = 0;
protected:
	Waveform waveform; //!< The waveform of the sound.
	bool soundEnabled; //!< Defines if the sound is enabled or not.
	uint16_t amplitude = 100; //!< Amplitude in percent.
};


#endif /* SOUND_H_ */
