#ifndef MELODIES_H_
#define MELODIES_H_

#include "Game/Sections/MusicPlayer.h"

// Mélodie "Frère Jacques" (32 notes)
const MusicNote FRERE_JACQUES[] = {
    {NoteName::C, 4, 500}, // Do
    {NoteName::D, 4, 500}, // Ré
    {NoteName::E, 4, 500}, // Mi
    {NoteName::C, 4, 500}, // Do

    {NoteName::C, 4, 500}, // Do
    {NoteName::D, 4, 500}, // Ré
    {NoteName::E, 4, 500}, // Mi
    {NoteName::C, 4, 500}, // Do

    {NoteName::E, 4, 500}, // Mi
    {NoteName::F, 4, 500}, // Fa
    {NoteName::G, 4, 1000}, // Sol (longue)

    {NoteName::E, 4, 500}, // Mi
    {NoteName::F, 4, 500}, // Fa
    {NoteName::G, 4, 1000}, // Sol (longue)

    {NoteName::G, 4, 250}, // Sol
    {NoteName::A, 4, 250}, // La
    {NoteName::G, 4, 250}, // Sol
    {NoteName::F, 4, 250}, // Fa
    {NoteName::E, 4, 500}, // Mi
    {NoteName::C, 4, 500}, // Do

    {NoteName::G, 4, 250}, // Sol
    {NoteName::A, 4, 250}, // La
    {NoteName::G, 4, 250}, // Sol
    {NoteName::F, 4, 250}, // Fa
    {NoteName::E, 4, 500}, // Mi
    {NoteName::C, 4, 500}, // Do

    {NoteName::C, 4, 500}, // Do
    {NoteName::G, 3, 500}, // Sol (octave inférieure)
    {NoteName::C, 4, 1000}, // Do (longue)

    {NoteName::C, 4, 500}, // Do
    {NoteName::G, 3, 500}, // Sol (octave inférieure)
    {NoteName::C, 4, 1000}  // Do (longue - fin)
};

const uint16_t FRERE_JACQUES_LENGTH = sizeof(FRERE_JACQUES) / sizeof(MusicNote);

#endif
