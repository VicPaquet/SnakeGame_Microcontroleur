/*
 * music_timer_callback.cpp
 *
 *  Created on: Nov 19, 2025
 *      Author: vicpa
 */


#include "Game/Sections/MusicPlayer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fonction C appelée par l'interruption TIM7
 * Cette fonction fait le pont entre le code C (interruption) et C++ (MusicPlayer)
 */
void MusicPlayer_TimerCallback(void) {
    MusicPlayer* player = MusicPlayer::getInstance();
    if (player) {
        player->timerCallback();
    }
}

#ifdef __cplusplus
}
#endif

