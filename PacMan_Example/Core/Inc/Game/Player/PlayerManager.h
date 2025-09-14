#ifndef PLAYERMANAGER_HEADER_GUARD
#define PLAYERMANAGER_HEADER_GUARD

/**
  * @file PlayerManager.h
  * @date June 2025
  * @brief Contains the declaration of the PlayerManager class that holds the 
  * meta data information of all players in the game.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/Player/Player.h"

namespace ELE3312 {
	/** @brief The PlayerManager contains the meta data of all players in the game.
	  */
	class PlayerManager {
		public:
			PlayerManager();
			Player & getPlayer();
			Player & getOpponent();
		private:
			Player player; //!< Meta data of the local player.
			Player opponent; //!< Meta data of the opponent (remote player).
	};

} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312

#endif
