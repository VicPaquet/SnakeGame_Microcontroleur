/**
  * @file Communication.h
  * @date June 2025
  * @brief Declares the communication interface that is used to exchange 
  * messages with a second player.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef COMMUNICATION_INCLUDE_GUARD
#define COMMUNICATION_INCLUDE_GUARD

#include "Interfaces/Communication/Message.h"
#include <memory>

namespace ELE3312 {
	/** @brief Interface for the communication with another player.
	  */
	class Communication {
		public:
			virtual void send(Message *msg) = 0;
	};
} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312
#endif
