/**
  * @file PlayerManager.cpp
  * @date June 2025
  * @brief This file implements the PlayerManager class the is used to manage the 
  * meta data of the games players.
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/Player/PlayerManager.h"

using namespace ELE3312;

/** @brief Default constructor for the PlayerManager class.
  */
PlayerManager::PlayerManager() : player {PlayerType::Local, PlayerCharacter::Unknown},
	opponent {PlayerType::Remote, PlayerCharacter::Unknown} { }

/** @brief Returns the local player.
  * @return A reference to the local player.
  */
Player & PlayerManager::getPlayer() {
	return player;
}

/** @brief Returns the opponent.
  * @return A reference to the opponent Player object.
  */
Player & PlayerManager::getOpponent() {
	return opponent;
}
