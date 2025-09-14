/**
  * @file Player.cpp
  * @date June 2025
  * @brief This file implements the Player class that encapsulates player meta data. 
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/Player/Player.h"

using namespace ELE3312;

/** @brief Constructor for the Player class.
  * @param [in] type The type of the player (i.e. Local, Remote, Unknown).
  * @param [in] character The player's game character (i.e. PacMan, Ghost, Unknown).
  */
Player::Player(PlayerType type, PlayerCharacter character) : type{type}, character{character}{
	reset(); // Reset to initial values
}

// Setters
/** @brief Sets the type of the player.
  * @param [in] newType The type of the player (i.e. Local, Remote, Unknown).
  */
void Player::setType(PlayerType newType){
	type = newType;	
}

/** @brief Sets the player's game character (PacMan, Ghost, etc.).
  * @param [in] newCharacter The player's game character.
  */
void Player::setCharacter(PlayerCharacter newCharacter){
	character = newCharacter;
}


/** @brief Sets the start position of the player.
  * @param [in] x The x start position.
  * @param [in] y The y start position.
  */
void Player::setStartPosition(uint16_t x, uint16_t y){
	startPosition.setX(x);
	startPosition.setY(y);
}

/** @brief Sets the sprite of the player's game character.
  * @param [in] newSprite A pointer to the sprite object that represents the player's character
  * in the game.
  */
void Player::setSprite(Sprite *newSprite) {
	sprite = newSprite;
}

/** @brief Sets the current position of the player's game character.
  * @param [in] newX The new x position.
  * @param [in] newY The new y position.
  */
void Player::setPosition(uint16_t newX, uint16_t newY) {
	currentPosition.setX(newX);
	currentPosition.setY(newY);
}

/** @brief Updates the current position of the player's game character
  * and stores it's previous position.
  * @param [in] newX The new x position.
  * @param [in] newY The new y position.
  */
void Player::updatePosition(uint16_t newX, uint16_t newY) {
	oldPosition = currentPosition;
	currentPosition = {newX, newY};
}

/** @brief Sets the x position of the player's game character.
  * @param [in] newX The new x position.
  */
void Player::setX(uint16_t newX){
	currentPosition.setX(newX);
}

/** @brief Sets the y position of the player's game character.
  * @param [in] newY The new y position.
  */
void Player::setY(uint16_t newY){
	currentPosition.setY(newY);
}

/** @brief Sets the old position value of the player's game character.
  * @param [in] x The x position.
  * @param [in] y The y position.
  */
void Player::setOldPosition(uint16_t x, uint16_t y){
	oldPosition = {x, y};
}

/** @brief Sets whether or not the player has won the combat.
  * @param [in] win Boolean value indicating whether or not the player has won.
  */
void Player::setWinner(bool win){
	winner = win;
}

// Getters
/** @brief Returns the type of the player.
  * @return The player's type (i.e. Local, Remote or Unknown).
  */
PlayerType Player::getType() const{
	return type;
}

/** @brief Returns the player's game character.
  * @return The players game character (i.e. PacMan, Ghost, or Unknown).
  */
PlayerCharacter Player::getCharacter() const{
	return character;
}


/** @brief Returns the player's x start position. 
  * @return The player's x start position.
  */
uint16_t Player::getStartPositionX() const{
	return startPosition.getX();
}

/** @brief Returns the player's y start position.
  * @return The player's y start position.
  */
uint16_t Player::getStartPositionY() const{
	return startPosition.getY();
}

/** @brief Returns a pointer to the player's game character's sprite.
  * @return A pointer to the player's game character's sprite.
  */
Sprite * Player::getSprite() {
	return sprite;
}

/** @brief Returns the player's x position.
  * @return The player's x position.
  */
uint16_t Player::getX() const{
	return currentPosition.getX();
}

/** @brief Returns the player's y position.
  * @return The player's y position.
  */
uint16_t Player::getY() const{
	return currentPosition.getY();
}

/** @brief Returns the player's old x position.
  * @return The player's old x position.
  */
uint16_t Player::getOldX() const{
	return oldPosition.getX();
}

/** @brief Returns the player's old y position.
  * @return The player's old y position.
  */
uint16_t Player::getOldY() const{
	return oldPosition.getY();
}

/** @brief Returns whether or not the player has won the combat.
  * @retval true The player has won the combat.
  * @retval false The player has not won the combat.
  */
bool Player::isWinner() {
	return winner;
}

/** @brief Adds a coin to the player.
  */
void Player::addCoin() {
	++coins;
}

/** @brief Returns the number of coins the player has collected.
  * @return The number of collected coins.
  */
uint16_t Player::getCoins() const {
	return coins;
}

/** @brief Returns the number of lives the player has.
  * @return Number of lives.
  */
uint16_t Player::getNumberLives() const {
	return numberLives;
}

/** @brief Decrements the number of lives the player has.
  */
void Player::removeLive() {
	numberLives -= 1;
}

/** @brief Sets the number of lives the player has.
 * @param num the number of lives.
 */
void Player::setNumberLives(uint16_t num){
	numberLives = num;
}
/** @brief Resets the player.
 */
void Player::reset(){
	numberLives = initialNumberLives;
	coins = 0;
	type = PlayerType::Unknown;
	character = PlayerCharacter::Unknown;
	winner = false;
	sprite = nullptr;
}
/**
  * @}
  */ // End of documentation group ELE3312
