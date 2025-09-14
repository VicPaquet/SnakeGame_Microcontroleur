/*
 * Note.cpp
 *
 *  Created on: May 13, 2025
 *      Author: joerg
 */

#include "Interfaces/Sound/Note.h"

namespace ELE3312 {
/** Table with note frequencies starting at C2
  */
const float Note::noteFrequencies[]= {65.4064,
		69.2957, 73.4162, 77.7817, 82.4069, 87.3071, 92.4986, 97.9989, 103.8262, 110.0, 116.5409,
		123.4708, 130.8128, 138.5913, 146.8324, 155.5635, 164.8138, 174.6141, 184.9972, 195.9977, 207.6523,
		220.0, 233.0819, 246.9417, 261.6256, 277.1826, 293.6648, 311.127, 329.6276, 349.2282, 369.9944,
		391.9954, 415.3047, 440.0, 466.1638, 493.8833, 523.2511, 554.3653, 587.3295, 622.254, 659.2551,
		698.4565, 739.9888, 783.9909, 830.6094, 880.0, 932.3275, 987.7666, 1046.5023, 1108.7305, 1174.6591,
		1244.5079, 1318.5102, 1396.9129, 1479.9777, 1567.9817, 1661.2188, 1760.0, 1864.655, 1975.5332};

const NoteName Note::baseName = NoteName::C; //!< The base note of the frequency table.
const uint16_t Note::baseOctave = 2; //!< The base octave of the frequency table.
const uint16_t Note::maxOctave = 6; //!< The highest octave for which we have frequencies.

/** @brief Constructor for the Note class.
  * @param [in] name The note's name (i.e. NoteName::C, NoteName::E, NoteName::G, etc.).
  * @param [in] octave The octave of the note (i.e. 2, 3, 4, etc.).
  */
Note::Note(NoteName name, uint16_t octave) : name{name}, octave{octave}{
	updateFrequency();
}


/** @brief Default constructor for the Note class.
  */
Note::Note() {
	name = NoteName::C;
	octave = 3;
}

/** @brief The copy constructor for the Note class.
  * @param [in] other A const reference to another Note object.
  */
Note::Note(const Note &other) {
	name = other.name;
	octave = other.octave;
	frequency = other.frequency;
}

/** @brief The assignment operator for the Note class.
  * @param [in] other A const reference to another Note object from which the content is being copied.
  * @return A reference to the current object.
  */
Note & Note::operator= (const Note &other){
	name = other.name;
	octave = other.octave;
	frequency = other.frequency;
	return *this;
}

/** @brief Sets the musical identity of the note
  * @param [in] name The name of the note (i.e. NoteName::C, Notename::E, NoteName::G, etc.).
  * @param [in] octave The octave of the note (i.e. 2,3,4, etc.).
  */
void Note::setNote(NoteName name, uint16_t octave){
	this->name = name;
	this->octave = octave;
	updateFrequency();
}

/** @brief Returns the name of the note.
  * @return The name of the note as a NoteName enum.
  */
NoteName Note::getNoteName() const {
	return name;
}

/** @brief Returns the octave of the note.
  * @return The octave value of the note.
  */
uint16_t Note::getOctave() const {
	return octave;
}

/** @brief updates the stored frequency value of the note.
  */
void Note::updateFrequency(){
	uint16_t noteIndex = (uint16_t)name + octave*12-24; // Is this OK ?
	if (noteIndex > NOTELUT_SIZE) {
		noteIndex = 0;
	}
	frequency = noteFrequencies[noteIndex];
}

/** @brief Returns the frequency value of the note.
  * @return The frequency value of the note in Hz.
  */
float Note::getFrequency() const {
	//return frequency;
	
	uint16_t noteIndex = (uint16_t)name + octave*12-24; // Is this OK ?
	if (noteIndex > NOTELUT_SIZE) {
		noteIndex = 0;
	}
	return noteFrequencies[noteIndex];
}

/** @brief Calculates the distance between two notes.
  * @param [in] other Const reference to another note.
  * @return The number of semitones between the two notes.
  */
uint16_t Note::getDistance(const Note &other) const {
	float highNoteFreq = 0.0f;
	uint16_t count = 0;
	uint16_t pos = 0;
	if (frequency < other.getFrequency()) {
		highNoteFreq = other.getFrequency();
		pos = (uint16_t)name + octave*12-24; 
	} else {
		highNoteFreq = other.getFrequency();
		pos = (uint16_t)other.getNoteName() + other.getOctave()*12-24; 
	}
	for (; pos < NOTELUT_SIZE; ++pos){
		count += 1;
		if (noteFrequencies[pos] == highNoteFreq) {
			break;
		}
	}
	return count;
}

/** @brief Increments the note value by the specified number of semitones
  * and returns a new Note with that note value.
  * @param [in] semitones The number of semitones.
  * @return A Note object that is the number of semitones higher then  the original note.
  */
Note Note::goUp(uint16_t semitones) const{
	if (semitones == 0 ) return Note{name, octave};
	uint16_t noteIndex = (uint16_t)name + octave*12-24; // Is this OK ?
	if ((noteIndex + semitones) >= NOTELUT_SIZE){
		noteIndex = NOTELUT_SIZE - 1;
	}

	uint16_t noteValue = (uint16_t)name;
	uint16_t newOctave = octave + ((noteValue + semitones) / 12);
	if (newOctave > Note::maxOctave){
		newOctave = Note::baseOctave;
	}
	NoteName newname = (NoteName)((noteValue + (semitones % 12)) % 12);
	return Note{newname, newOctave};
}

/** @brief Returns a string with the note name and octave of the note.
  * @retval String containing the note name and octave.
  */
std::string Note::toString() const {
	char str[10] = {0};
	switch(name){
		case NoteName::C:
			sprintf(str,"C%d", octave);
			break;
		case NoteName::CS:
			sprintf(str,"CS%d", octave);
			break;
		case NoteName::D:
			sprintf(str,"D%d", octave);
			break;
		case NoteName::DS:
			sprintf(str,"DS%d", octave);
			break;
		case NoteName::E:
			sprintf(str,"E%d", octave);
			break;
		case NoteName::F:
			sprintf(str,"F%d", octave);
			break;
		case NoteName::FS:
			sprintf(str,"FS%d", octave);
			break;
		case NoteName::G:
			sprintf(str,"G%d", octave);
			break;
		case NoteName::GS:
			sprintf(str,"GS%d", octave);
			break;
		case NoteName::A:
			sprintf(str,"A%d", octave);
			break;
		case NoteName::AS:
			sprintf(str,"AS%d", octave);
			break;
		case NoteName::B:
			sprintf(str,"B%d", octave);
			break;
	}
	return str;
}
			


} /* namespace ELE3312 */

/**
  * @}
  */ // End of documentation group ELE3312

