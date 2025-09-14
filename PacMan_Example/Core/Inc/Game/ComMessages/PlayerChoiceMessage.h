/**
  * @file PlayerChoiceMessage.h 
  * @date June 2025
  * @brief Declares the PlayerChoiceMessage that is exchanged in the menu phase of the game
  * where the player can chose it's game character.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef PLAYERCHOICEMESSAGE_INCLUDE_GUARD
#define PLAYERCHOICEMESSAGE_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"
#include "Game/Player/Player.h"

namespace ELE3312 {
/** @brief Message type exchanged in the menu phase of the game where the player chooses 
  * his game character (i.e. PacMan or Ghost)
  */
class PlayerChoiceMessage : public Message {
	public:
		PlayerChoiceMessage(PlayerCharacter playerCharacter);
		PlayerChoiceMessage();
		// Setters
		void setType(MessageType newType);
		virtual void setData(uint8_t *newData, size_t length) override;
		// Getters
		MessageType getType();
		virtual size_t getSize() const override;
		virtual const uint8_t *getData() const override;
		PlayerCharacter getPlayerChoice() const;

		virtual std::string toString() const override;
	private:
		PlayerCharacter playerCharacter;
};

} // End namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif

