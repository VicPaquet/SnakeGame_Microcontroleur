#ifndef SPRITE_HEADER_GUARD
#define SPRITE_HEADER_GUARD
/**
  * @file Sprite.h
  * @date June 2025
  * @brief This file contains the declaration of the Sprite class
  *
  * A sprite represents a pixel image of an graphical element in the game.
  * Examples of sprites are elements of the labyrinth or the characters present
  * in the game.
  * @defgroup ELE3312
  * @{
  */

#include <cstdint>

namespace ELE3312 {

	/** @brief The type of a Sprite object
	  * @details Sprite objects can have different types according to
	  * their usage. Example of types are Wall, Ghost or Pacman.
	  */
	enum class SpriteType {
		Wall, Ghost, Pacman, Object, None, Heart, Coin
	};

	/** @brief A class representing a Sprite in the game.
	  * @details A sprite represents a pixel image of a graphical object 
	  * in the game. Such pixel graphics have a specific height and width, as 
	  * well as a type and the associated pixel data stored in an array of 
	  * integer values. 
	  */
	class Sprite {
		public:
			Sprite(uint16_t width, uint16_t height, SpriteType type, uint16_t *data);
			Sprite(const Sprite& other);
			Sprite & operator=(const Sprite& other);
			virtual ~Sprite();
			// Setters
			void setWidth(uint16_t newWidth);
			void setHeight(uint16_t newHeight);
			void setType(SpriteType newType);
			void setData(uint16_t *newData);
			// Getters
			uint16_t getWidth() const;
			uint16_t getHeight() const;
			uint16_t *getData() const;
			SpriteType getType() const;
			
		private:
			uint16_t width; //!< The width of the sprite in pixels.
			uint16_t height;//!< The height of the sprite in pixels.
			SpriteType type;//!< The type of the sprite.
			uint16_t *data; //!< Pointer to the actual sprite pixel data.

	};

} // End of namespace ELE3312

/**
  * @}
  */
#endif
