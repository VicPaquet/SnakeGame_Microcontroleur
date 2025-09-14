#ifndef PLAYER_HEADER_GUARD
#define PLAYER_HEADER_GUARD 

/**
  * @file Player.h
  * @date June 2025
  * @brief Contains the declaration of the Player class that encapsulates player meta data.
  * 
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>
#include "Interfaces/Display/Sprite.h"
#include "Interfaces/Display/Point.h"

namespace ELE3312 {
	/** @brief Represents the player type 
	  */
	enum class PlayerType {
		Local, Remote, Unknown
	};
	
	/** @brief Represents the player's game character.
	  */
	enum class PlayerCharacter {
		Pacman, Ghost, Unknown
	};
	
	/** @brief The Player class encapsulates player meta data.
	  */
	class Player {
		public:
			Player(PlayerType type, PlayerCharacter character);
			virtual ~Player() = default;
			// Setters
			void setType(PlayerType type);
			void setCharacter(PlayerCharacter character);
			void setGridPosition(uint16_t x, uint16_t y);
			void setStartPosition(uint16_t x, uint16_t y);
			void setSprite(Sprite *newSprite);
			void setPosition(uint16_t newX, uint16_t newY); 
			void updatePosition(uint16_t newX, uint16_t newY); 
			void setOldPosition(uint16_t x, uint16_t y);
			void setX(uint16_t x);
			void setY(uint16_t y);
			void setWinner(bool win);
			void addCoin();
			void removeLive();
			void setNumberLives(uint16_t num);
			void reset();
			// Getters
			PlayerType getType() const;
			PlayerCharacter getCharacter() const;
			uint16_t getX() const;
			uint16_t getY() const;
			uint16_t getStartPositionX() const;
			uint16_t getStartPositionY() const;
			uint16_t getOldX() const;
			uint16_t getOldY() const;
			uint16_t getCoins() const;
			uint16_t getNumberLives() const;
			Sprite * getSprite();
			bool isWinner();

		private:
			PlayerType type; //!< The player's type.
			PlayerCharacter character; //!< The player's game character type.
			Point startPosition;
			Point currentPosition;
			Point oldPosition;
			Sprite *sprite;//!< A pointer to the sprite object that represents the player's game character.
			bool winner; //!< Indicates whether or not the player has won.
			uint16_t coins = 0;
			const uint16_t initialNumberLives = 3;
			uint16_t numberLives;
	};
} // End of the namespace ELE3312

/**
  * @}
  */ // End of the documentation group ELE3312

#endif
