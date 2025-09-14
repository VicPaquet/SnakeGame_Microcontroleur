#ifndef COMBATMESSAGE_INCLUDE_GUARD
#define COMBATMESSAGE_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"

namespace ELE3312 {

class CombatMessage : public Message {
	public:
		CombatMessage();
		// Setters
		void setType(MessageType newType);
		virtual void setData(uint8_t *newData, size_t length) override;
		// Getters
		MessageType getType();
		virtual size_t getSize() const override;
		virtual const uint8_t *getData() const override;

		virtual std::string toString() const override;
		void setBarValue(uint16_t value);
		uint16_t getBarValue() const;
	private:
		uint16_t barValue;
};

} // End namespace ELE3312


#endif
