#ifndef NOTE_H_
#define NOTE_H_

/**
  * @file Note.h
  * @date June 2025
  * @brief Contains declaration of the Note class that represents a musical note.
  * 
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>
#include <cmath>
#include <string>
#include <array>

#define NOTELUT_SIZE 60

namespace ELE3312 {


/** @brief Represent the name of a musical note.
  */
enum class NoteName {
	C, CS, D, DS, E, F, FS, G, GS, A, AS, B
};

/** @brief The Note class represents a musical note
  */
class Note {
public:
	Note(NoteName name, uint16_t octave);
	Note(const Note &other);
	Note & operator= (const Note &other);
	Note();
	virtual ~Note() = default;
	float getFrequency() const;
	void setNote(NoteName name, uint16_t octave);
	uint16_t getDistance(const Note &other) const;
	NoteName getNoteName() const;
	uint16_t getOctave() const;
	Note goUp(uint16_t semitones) const;
	std::string toString() const;
private:
	NoteName name; //!< The name of the note (e.g. A, B, C, ...).
	uint16_t octave; //!< The octave value of the note (e.g. 2,3,4, ...).
	uint16_t freqIndex; //!< The index into the frequency table. 
	float frequency; //!< The frequency in Hz of the note.
	static const float noteFrequencies[]; //!< Frequency table containing the frequencies of a range of notes
	static const NoteName baseName; //!< The name of the lowest note in the frequency table.
	static const uint16_t baseOctave; //!< The octave of the lowest note in the frequency table.
	static const uint16_t maxOctave; //!< The highest octave for which we have frequencies.
	void updateFrequency();
	
};

} /* namespace ELE3312 */

/**
  * @}
  */
#endif /* NOTE_H_ */
