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
#include "Game/Graphics/GraphObjects/Checkboard.h"

	struct SnakeGameMessageData {
		uint16_t x; //!< The x position on the screen in pixels
		uint16_t y; //!< The y position on the screen in pixels
		uint8_t direction; //!< The direction of the snake (0=NORTH, 1=EAST, 2=SOUTH, 3=WEST)
		CollisionType collision; //!< Indicates that a collision with the opponent was detected
	};

/** @brief The LabyrinthMessage class encapsulates information in the 
  * labyrinth phase of the game.
  */
class SnakeGameMessage : public Message {
	public:
		SnakeGameMessage(uint16_t x, uint16_t y, uint8_t direction = 0, CollisionType collisionType = CollisionType::None);
		SnakeGameMessage();
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
		uint8_t getDirection() const;
		CollisionType getCollisionType() const;

		void setX(uint16_t newX);
		void setY(uint16_t newY);
		void setDirection(uint8_t direction);
		void setCollisionType(CollisionType type);
	private:
		SnakeGameMessageData data;
};

#endif
