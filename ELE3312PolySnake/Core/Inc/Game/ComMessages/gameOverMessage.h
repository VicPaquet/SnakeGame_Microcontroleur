/**
  * @file handShakeMessage.h 
  * @date November 2025
  * @brief Declaration of the handShakeMessage class that is used to exchange 
  * initial handshake messages between players.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef HANDSHAKEMESSAGE_INCLUDE_GUARD
#define HANDSHAKEMESSAGE_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"


	struct gameOverMessageData {
		bool gameOver;
	};


class gameOverMessage : public Message {
    public:
		gameOverMessage(uint32_t seed);
		gameOverMessage();
        
        // Setters
        void setType(MessageType newType);
        virtual void setData(uint8_t *newData, size_t length) override;
        void setGameOver(uint32_t newGameOver);

        // Getters
        MessageType getType() const;
        virtual size_t getSize() const override;
        virtual const uint8_t *getData() const override;
        std::string toString() const override {
            return "Game Over Message " + std::to_string(data.gameOver);
        }

        uint32_t getGameOver() const {
            return data.gameOver;
        }

    private:
        gameOverMessageData data;
};

#endif // HANDSHAKEMESSAGE_INCLUDE_GUARD
/** @} */
