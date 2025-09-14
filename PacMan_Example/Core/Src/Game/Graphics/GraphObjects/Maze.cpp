/**
  * @file Maze.cpp
  * @date June 2025
  * @brief This file implements the Maze class that is used to draw the maze on the screen. 
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/Graphics/GraphObjects/Maze.h"
#include "Interfaces/Display/Point.h"
#include "Resources/mazeData.h"
#include "Resources/spriteData.h"

using namespace ELE3312;

/** @brief Setup routine for the Maze graphical object class.
  * @param [in] rect A rectangle defining the maze's size and position on screen.
  * @param [in] disp A Display object that is used to draw the maze on the screen.
  * @param [in] players A PlayerManager object that contains player meta data.
  */
void Maze::setup(const Rect &rect, Display *disp,
		PlayerManager *players) {
	this->rect = rect;
	this->disp = disp;
	this->players = players;
	// Default configuration, can be made accessible outside of the object later
	gridWidth  = 10; // 10 pixels grid width
	gridHeight = 10; // 10 pixels grid height
	// Reset maze if it is not empty
	if(!maze.empty()){
		maze.clear();
	}
	uint32_t idx = 0; // Index into the maze data array
	for (uint16_t gridY = 0; gridY < numVerticalTiles(); ++gridY) {
		for (uint16_t gridX = 0; gridX < numHorizontalTiles(); ++gridX) {
			uint16_t spriteID = mazeData[idx];
			idx += 1;
			Sprite * sprite = sprites[spriteID];	
			if(sprite->getType() == SpriteType::Pacman ){ 
				uint16_t screenX = toScreenX(gridX) + gridWidth / 2;
				uint16_t screenY = toScreenY(gridY) + gridHeight / 2;
				if(this->players->getPlayer().getCharacter() == PlayerCharacter::Pacman) {
					this->players->getPlayer().setSprite(sprite);
					this->players->getPlayer().setStartPosition(screenX, screenY);
					this->players->getPlayer().setPosition(screenX, screenY);
					this->players->getPlayer().setOldPosition(screenX, screenY);
				} else {
					this->players->getOpponent().setSprite(sprite);
					this->players->getOpponent().setStartPosition(screenX, screenY);
					this->players->getOpponent().setPosition(screenX, screenY);
					this->players->getOpponent().setOldPosition(screenX, screenY);
				}
				sprite = nullptr; // Don't put the player on the board
			}
			if(sprite->getType() == SpriteType::Ghost) { 
				uint16_t screenX = toScreenX(gridX) + gridWidth / 2;
				uint16_t screenY = toScreenY(gridY) + gridHeight / 2;
				if(this->players->getPlayer().getCharacter() == PlayerCharacter::Ghost) {
					this->players->getPlayer().setSprite(sprite);
					this->players->getPlayer().setStartPosition(screenX, screenY);
					this->players->getPlayer().setPosition(screenX, screenY);
					this->players->getPlayer().setOldPosition(screenX, screenY);
				} else {
					this->players->getOpponent().setSprite(sprite);
					this->players->getOpponent().setStartPosition(screenX, screenY);
					this->players->getOpponent().setPosition(screenX, screenY);
					this->players->getOpponent().setOldPosition(screenX, screenY);
				}
				sprite = nullptr; // Don't put the player on the board
			}
			
			maze.push_back(sprite);
		}
	}
	ready = true;
}

/** @brief Returns the state of the Maze object.
  * @retval true The object is setup.
  * @retval false The object is not setup.
  */
bool Maze::isReady() {
	return ready;
}

/** @brief Draws the maze on the screen.
  */
void Maze::draw(){
	for (uint16_t gridY = 0; gridY < numVerticalTiles(); ++gridY) {
		for (uint16_t gridX = 0; gridX < numHorizontalTiles(); ++gridX) {
			auto sprite = maze.at((gridY*numHorizontalTiles()) + gridX);
			drawSprite(toScreenX(gridX), toScreenY(gridY), sprite);	
		}
	}
	updateLives(); // Displays the number of lives.
	// Draw the active characters (players)
	update();
}

/** @brief Updates the maze on the screen. Particularly, it redraws the characters to make them 
  * appear to move.
  */
void Maze::update(){
	Player * player = &players->getPlayer();
	Sprite * sprite = player->getSprite();
	const uint16_t halfGridWidth = gridWidth/2;
	const uint16_t halfGridHeight = gridHeight/2;
	uint16_t x = player->getOldX() - halfGridWidth;
	uint16_t y = player->getOldY() - halfGridHeight;
	// Draw local player
	disp->fillRect(disp->getBackgroundColor(), {{x, y}, sprite->getWidth(), sprite->getHeight()});	
	drawSprite(player->getX() - halfGridWidth, player->getY() - halfGridHeight, sprite);
	// Draw remote player (opponent)
	player = &players->getOpponent();
	sprite = player->getSprite();
	x = player->getOldX() - halfGridWidth;
	y = player->getOldY() - halfGridHeight;
	disp->fillRect(disp->getBackgroundColor(), {{x, y}, sprite->getWidth(), sprite->getHeight()});	
	drawSprite(player->getX() - halfGridWidth, player->getY() - halfGridHeight, sprite);
}

/** @brief Returns the Sprite object for the specified tile position.
  * @param [in] p A point that identifies a tile of the screen.
  * @return A pointer to the Sprite object displayed at the specified position.
  */
Sprite * Maze::getSpriteAt(Point p) const{
	return maze.at((p.getY()*numHorizontalTiles()) + p.getX());
}

/** @brief Method that checks if a position is valid for the character to move to.
  * @param [in] x The x coordinate of the position.
  * @param [in] y The y coordinate of the position.
  * @param [in] self A pointer to the player that wants to go to the specified position.
  * @param [in] opponent A pointer to the player object that represents the opponent. (used to detect 
  * an encounter between the player and the opponent)
  * @return An indication if the position would trigger a collision and if so, it's type.
  */
Collision Maze::checkPosition(uint16_t x, uint16_t y, Player *self, Player *opponent){
	// Check for tunnels
	if (x < gridWidth){
		return {CollisionType::TunnelLeft, x, y};
	}
	if (x > disp->getScreenWidth() - gridWidth){
		return {CollisionType::TunnelRight, x, y};
	}
	if (y - gridHeight < rect.getY1() ) {
		return {CollisionType::TunnelTop, x, y};
	}
	if (y > disp->getScreenHeight() - gridHeight){
		return {CollisionType::TunnelButtom, x, y};
	}
	// Get the direction of movement
	bool x_pos = x > self->getX();
	bool x_neg = x < self->getX();
	bool y_pos = y > self->getY();
	bool y_neg = y < self->getY();
	int16_t x_la = x_pos ? gridWidth/2 - 0: -(gridWidth/2) + 0;
	CollisionType collType = CollisionType::None;

	if ((!x_pos && !x_neg) || x % 5 == 0){
		x_la = 0;
	}
	int16_t y_la = y_pos ? gridHeight/2 - 0 : -(gridHeight/2) + 0;
	if ((!y_pos && !y_neg) || y % 5 == 0) {
		y_la = 0;
	}

	uint16_t x_tile = ( x + x_la - rect.getX1() )/gridWidth;
	uint16_t y_tile = ( y + y_la - rect.getY1() )/gridHeight;

	Sprite *sprite = getSpriteAt({x_tile, y_tile});
	if (sprite->getType() == SpriteType::Wall) {
		collType =  CollisionType::Wall;
	}
	
	if (sprite->getType() == SpriteType::Coin) {
		collType =  CollisionType::Coin;
	}
	// Check if there is a collision with the opponent

	uint16_t opponent_x_tile = (opponent->getX() - rect.getX1())/ gridWidth;
	uint16_t opponent_y_tile = (opponent->getY() - rect.getY1())/ gridHeight;
	if(x_tile == opponent_x_tile && y_tile == opponent_y_tile) {
		collType = CollisionType::Opponent;
	}

	return {collType, x_tile, y_tile};
}


/**
  * @brief Removes a coin sprite from the maze.
  * @param x The x screen position of the coin.
  * @param y The y screen position of the coin.
  */
void Maze::removeCoin(uint16_t x, uint16_t y){
	uint32_t pos = (y*numHorizontalTiles()) + x;
	Sprite * coinSprite = maze.at(pos);
	maze[pos] = sprites.back();
	drawSprite(toScreenX(x),toScreenY(y), coinSprite);
}

/** @brief Conversion from a tile position to a screen position.
  * @param [in] gridX The x position of the tile.
  * @return The corresponding tile position.
  */
uint16_t Maze::toScreenX(uint16_t gridX) const{
	return gridX * gridWidth + rect.getX1();
}

/** @brief Conversion from a tile position to a screen position.
  * @param [in] gridY The y position of the tile.
  * @return The corresponding y position.
  */
uint16_t Maze::toScreenY(uint16_t gridY) const{
	return gridY * gridHeight + rect.getY1();
}

/** @brief Draws the specified Sprite object on the screen.
  * @param [in] x The x position.
  * @param [in] y The y position.
  * @param [in] sprite A pointer to a Sprite object that contains the pixel data of the sprite.
  */
void Maze::drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const {
	if (sprite != nullptr) {
		disp->drawBitmap(sprite->getData(), x, y, sprite->getWidth(), sprite->getHeight());
	}
}

/** @brief Clears the maze.
  * @todo Implement the clear method or remove it if it is not necessary.
  */
void Maze::clear() {

}

/** @brief Returns the number of horizontal tiles.
  * @return The number of horizontal tiles.
  */
uint16_t Maze::numHorizontalTiles() const{
	return rect.getWidth() / gridWidth;
}

/** @brief Returns the number of vertical tiles.
  * @return Number of vertical tiles.
  */
uint16_t Maze::numVerticalTiles() const{
	return rect.getHeight() / gridHeight;
}


/** @brief Updates the display of the remaining lives of the player.
  */
void Maze::updateLives(){
	// Get heart sprite
	Sprite *heart = sprites[17];
	// Clear the display area
	disp->fillRect(disp->getBackgroundColor(), 
			{(uint16_t)(disp->getScreenWidth()/2),
			0, disp->getScreenWidth(), 10});
	for (int i=1; i<=numberLives; ++i){
		drawSprite(disp->getScreenWidth() - 15*i, 5, heart);
	}
}

/** @brief Sets the number of lives the player has. The value is used
  * to show the corresponding number of hearts in the top-right corner of the 
  * display.
  * @param numLives The number of lives of the player.
  */
void Maze::setNumberLives(uint16_t numLives){
	numberLives = numLives;
}
/**
  * @}
  */ // End of documentation group ELE3312


