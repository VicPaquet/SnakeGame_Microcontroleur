/**
  * @file PacMan.h
  * @date June 2025
  * @brief Contains declaration of the PacMan graphical object class that draws an image of
  * the PacMan game character on the screen.
  * 
  * @defgroup ELE3312
  * @{
  */

#ifndef PACKMAN_H_
#define PACKMAN_H_

#include "Game/Graphics/GraphObject.h"

namespace ELE3312 {
/** @brief The PacMan graphical object class draws an image of the PacMan game character on
  * the screen.
  */
class PacMan : public GraphObject{
public:
	virtual ~PacMan() = default;
	void draw() override;
	void clear() override;
};

} /* namespace ELE3312 */

/**
  * @}
  */ // End of documentation group ELE3312

#endif /* PACKMAN_H_ */
