/**
  * @file LabyrinthMessage.h 
  * @date June 2025
  * @brief Declaration of the LabyrinthMessage class that is used to exchange 
  * messages in the labyrinth phase of the game.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef LABYRINTHMESSAGE_INCLUDE_GUARD
#define LABYRINTHMESSAGE_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"
#include "Game/Graphics/GraphObjects/Maze.h"


namespace ELE3312 {
	/** @brief Structure that encapsulates the position data for
	  * a player on the screen.
	  */
	struct LabyrinthMessageData {
		uint16_t x; //!< The x position on the screen in pixels
		uint16_t y; //!< The y position on the screen in pixels
		CollisionType collision; //!< Indicates that a collision with the opponent was detected
	};

/** @brief The LabyrinthMessage class encapsulates information in the 
  * labyrinth phase of the game.
  */
class LabyrinthMessage : public Message {
	public:
		LabyrinthMessage(uint16_t x, uint16_t y, CollisionType collisionType = CollisionType::None);
		LabyrinthMessage();
		// Setters
		void setType(MessageType newType);
		virtual void setData(uint8_t *newData, size_t length) override;
		// Getters
		MessageType getType();
		virtual size_t getSize() const override;
		virtual const uint8_t *getData() const override;

		virtual std::string toString() const override;

		uint16_t getX() const;
		uint16_t getY() const;
		CollisionType getCollisionType() const;

		void setX(uint16_t newX);
		void setY(uint16_t newY);
		void setCollisionType(CollisionType type);
	private:
		LabyrinthMessageData data;
};

} // End namespace ELE3312

/**
  * @}
  */ // End of the documentation group ELE3312
#endif
