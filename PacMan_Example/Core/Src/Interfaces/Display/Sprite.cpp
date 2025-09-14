/**
 * @file Sprite.cpp
 * @date June 2025
 * @brief This file contains the implementation of the Sprite class
 *
 * A sprite represents a pixel image of an graphical element in the game.
 * Examples of sprites are elements of the labyrinth or the characters present
 * in the game.
 * @defgroup ELE3312
 * @{
 */
#include "Interfaces/Display/Sprite.h"
using namespace ELE3312;

/** @brief Sprite constructor
 * @details The constructor creates a sprite object that has dimensions, a type
 * (indicating whether it is part of the environment or one of the game characters)
 * and a pointer to the actual pixel data.
 * @param [in] width The width of the sprite in pixels.
 * @param [in] height The height of the sprite in pixels.
 * @param [in] type The type of the sprite (wall, character, etc.)
 * @param [in] data A pointer to the pixel data that represents the sprite.
 */
Sprite::Sprite(uint16_t width, uint16_t height, SpriteType type, uint16_t *data) :
	width{width}, height{height}, type{type}, data{data} {
	}
/** @brief Copy constructor
 * @details The copy constructor creates a new Sprite object based on the 
 * the provided reference to another Sprite object.
 * @param [in] other A const reference to another Sprite object.
 */
Sprite::Sprite(const Sprite& other):
	width{other.width}, height{other.height}, type{other.type}, data{other.data} {
	}

/** @brief Equal operator for Sprite object assignment.
 * @details The equals operator assigns all values of the specified Sprite object
 * to the object on the left side to the equal sign.
 * @param other A const reference to another Sprite object.
 */
Sprite & Sprite::operator=(const Sprite& other){
	if (this != &other) {
		width = other.getWidth();
		height = other.getHeight();
		type = other.getType();
		data = other.getData();
	}
	return *this;
}

/** @brief Destructor of the Sprite class.
*/
Sprite::~Sprite() {
}
// Setters

/** @brief Sets the width of the Sprite object.
 * @param [in] newWidth The new width value of the Sprite object.
 */
void Sprite::setWidth(uint16_t newWidth){
	width = newWidth;
}

/** @brief Sets the height of the Sprite object.
 * @param [in] newHeight The new height of the Sprite object.
 */
void Sprite::setHeight(uint16_t newHeight){
	height = newHeight;
}

/** @brief Sets the type of the Sprite.
 * @param [in] newType The new type of the Sprite object.
 */
void Sprite::setType(SpriteType newType){
	type = newType;
}

/** @brief Sets the pixel data of the Sprite object.
 * @param [in] newData The new pointer to the pixel data of the  Sprite object.
 */
void Sprite::setData(uint16_t *newData){
	// Be aware of memory leaks 
	data = newData;
}

// Getters

/** @brief Returns the width of the Sprite object.
 * @retval The width of the Sprite object in pixels.
 */
uint16_t Sprite::getWidth() const{
	return width;
}

/** @brief Returns the height of the Sprite object.
 * @retval The height of the Sprite object in pixels.
 */
uint16_t Sprite::getHeight() const{
	return height;
}

/** @brief Returns the pointer to the pixel data of the Sprite object.
 * @retval A pointer to the pixel data of the Sprite object.
 */
uint16_t *Sprite::getData() const{
	return data;
}

/** @brief Returns the type of the Sprite object.
 * @retval The type of the Sprite object (wall, character, etc.).
 */
SpriteType Sprite::getType() const{
	return type;
}

/** 
 * @}
 */
