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


	struct handShakeMessageData {
		uint32_t seed; //!< The x position on the screen in pixels
	};


class handShakeMessage : public Message {
    public:
        handShakeMessage(uint32_t seed);
        handShakeMessage();
        
        // Setters
        void setType(MessageType newType);
        virtual void setData(uint8_t *newData, size_t length) override;
        void setSeed(uint32_t newSeed);

        // Getters
        MessageType getType() const;
        virtual size_t getSize() const override;
        virtual const uint8_t *getData() const override;
        std::string toString() const override {
            return "HandShake Message - Seed: " + std::to_string(data.seed);
        }

        uint32_t getSeed() const {
            return data.seed;
        }

    private:
        handShakeMessageData data;
};

#endif // HANDSHAKEMESSAGE_INCLUDE_GUARD
/** @} */
