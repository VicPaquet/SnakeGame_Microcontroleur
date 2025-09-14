/**
  * @file Ghost.cpp
  * @date June 2025
  * @brief This file implements the Ghost class that draws an image of a ghost
  * on the screen.
  * 
  * @defgroup ELE3312
  * @{
  */

#include "Game/Graphics/GraphObjects/Ghost.h"
using namespace ELE3312;

Ghost::Ghost() {}

/** @brief Setup routine for the Ghost graphical object.
  * @param [in] rect A rectangle defining the size and position of the ghost.
  * @param [in] graph A Graphics object that is used to draw the ghost on the screen.
  * @param [in] color The color of the ghost's body.
  */
void Ghost::setup(const Rect &rect, Display *disp, Color color){
	this->rect = rect;
	this->disp = disp;
	this->color = color;
}

/** @brief The method draws the ghost on the screen.
  */
void Ghost::draw(){
	// The body of the ghost.
	disp->fillCircle(color,rect.getX1() + rect.getWidth()*0.5,
		   rect.getY1()+ rect.getWidth()*0.4,rect.getWidth()*0.4);
	disp->fillRect(color, rect.getX1() + rect.getWidth()*0.1,
		 rect.getY1() + rect.getWidth() * 0.4,
		 rect.getWidth() * 0.8,
		 rect.getWidth() * 0.4); //40, 80, 80, 40
	uint16_t skirtRadius = (rect.getWidth()*0.8) / 6;
	uint16_t startX = rect.getX1() + rect.getWidth()*0.1;
	disp->fillCircle(color, startX + skirtRadius,
			rect.getY1() + rect.getWidth() * 0.8, skirtRadius);
	disp->fillCircle(color, startX + 3*skirtRadius,
				rect.getY1() + rect.getWidth() * 0.8, skirtRadius);
	disp->fillCircle(color, startX + 5*skirtRadius,
					rect.getY1() + rect.getWidth() * 0.8, skirtRadius);

	// Eyes
	disp->fillCircle(Color::WHITE,rect.getX1() + rect.getWidth() *0.3 , 
		   rect.getY1() + rect.getWidth() * 0.4,
		   rect.getWidth() * 0.1);

	disp->fillCircle(Color::WHITE,rect.getX1() + rect.getWidth() *0.6 , 
		   rect.getY1() + rect.getWidth() * 0.4,
		   rect.getWidth() * 0.1);
	// Pupil
	uint16_t pupilRadius = rect.getWidth() * 0.03;
	if (pupilRadius == 0 ) {
		pupilRadius = 1;
	}
	disp->fillCircle(Color::BLACK,rect.getX1() + rect.getWidth() *0.3 , 
		   rect.getY1() + rect.getWidth() * 0.4,
		   pupilRadius);

	disp->fillCircle(Color::BLACK,rect.getX1() + rect.getWidth() *0.6 , 
		   rect.getY1() + rect.getWidth() * 0.4,
		   pupilRadius);

}

/** @brief This method removes the ghost from the screen by redrawing the ghost
  * in the background color as it is defined in the graph object.
  */
void Ghost::clear(){
	// Body
	Color clear_color = Color::BLACK;
	disp->fillCircle(clear_color,rect.getX1()+49,rect.getY1()-40,50);
	disp->fillRect(clear_color, rect.getX1(), rect.getY1()-40, 100, 50); //40, 80, 80, 40
	disp->fillCircle(clear_color,rect.getX1()+20,rect.getY1()+5,20); //53,120,13
	disp->fillCircle(clear_color,rect.getX1()+50,rect.getY1()+5,20);//79,120,13
	disp->fillCircle(clear_color,rect.getX1()+79,rect.getY1()+5,20); //105,120,13
	// Eyes
	disp->fillCircle(clear_color,rect.getX1()+25,rect.getY1()-40,8);
	disp->fillCircle(clear_color,rect.getX1()+65,rect.getY1()-40,8);
	// Pupils
	disp->fillCircle(clear_color,rect.getX1()+27,rect.getY1()-38,2);//67,y-2,2
	disp->fillCircle(clear_color,rect.getX1()+63,rect.getY1()-42,2);//93,y+2,2

}

/**
  * @}
  */
