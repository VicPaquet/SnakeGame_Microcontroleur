


#include <Resources/checkboardData.h>
#include <Resources/spriteData.h>
#include "Game/Sections/SnakeGame.h"
#include "Game/Graphics/GraphObjects/Checkboard.h"
#include "Game/Graphics/GraphObjects/Head.h"
#include "Interfaces/Display/Point.h"

void Checkboard::setup(const Rect &rect, Display *disp, SnakeGame* game) { //Add playerManager as a parameter
	this->rect = rect;
	this->disp = disp;
	this->snakeGame = game;
	//this->players = players;

	gridWidth  = 10; // 10 pixels grid width
	gridHeight = 10; // 10 pixels grid height

	if(!checkboard.empty()){
		checkboard.clear();
	}
	uint32_t idx = 0; // Index into the maze data array
	for (uint16_t gridY = 0; gridY < numVerticalTiles(); ++gridY) {
		for (uint16_t gridX = 0; gridX < numHorizontalTiles(); ++gridX) {
			uint16_t spriteID = checkboardData[idx];
			idx += 1;
			Sprite * sprite = sprites[spriteID];
			if(sprite->getType() == SpriteType::SnakeHead ){
				//uint16_t screenX = toScreenX(gridX) + gridWidth / 2;
				//uint16_t screenY = toScreenY(gridY) + gridHeight / 2;

				// Stocker la position du snake initiale etc
//				if(this->players->getPlayer().getCharacter() == PlayerCharacter::Pacman) {
//					this->players->getPlayer().setSprite(sprite);
//					this->players->getPlayer().setStartPosition(screenX, screenY);
//					this->players->getPlayer().setPosition(screenX, screenY);
//					this->players->getPlayer().setOldPosition(screenX, screenY);
//				} else {
//					this->players->getOpponent().setSprite(sprite);
//					this->players->getOpponent().setStartPosition(screenX, screenY);
//					this->players->getOpponent().setPosition(screenX, screenY);
//					this->players->getOpponent().setOldPosition(screenX, screenY);
//				}
//				sprite = nullptr; // Don't put the player on the board
			}

			checkboard.push_back(sprite);
		}
	}
	ready = true;
}

bool Checkboard::isReady(){
	return ready;
}

void Checkboard::draw(){
	for (uint16_t gridY = 0; gridY < numVerticalTiles(); ++gridY) {
			for (uint16_t gridX = 0; gridX < numHorizontalTiles(); ++gridX) {
				auto sprite = checkboard.at((gridY*numHorizontalTiles()) + gridX);
				drawSprite(toScreenX(gridX), toScreenY(gridY), sprite);
			}
		}
	// Add update number of Lives later

	update();
}

void Checkboard::update(){
    if (!ready) return;

    const uint16_t halfGridWidth  = gridWidth / 2;
    const uint16_t halfGridHeight = gridHeight / 2;

    // Dessiner la tête du serpent
    Head* headTile = snakeGame->get;
    uint16_t headX = toScreenX((headTile->getRect()).getX1()) - halfGridWidth;
    uint16_t headY = toScreenX((headTile->getRect()).getY1()) - halfGridWidth;
    drawSprite(headX, headY, SnakeHead);

    // Dessiner chaque partie du corps
    for (auto& part : *(SnakeGame->body)) {
        uint16_t bodyX = toScreenX(part.x) - halfGridWidth;
        uint16_t bodyY = toScreenY(part.y) - halfGridHeight;
        drawSprite(bodyX, bodyY, snakeBodySprite);
    }
}


uint16_t Checkboard::toScreenX(uint16_t gridX) const{
	return gridX * gridWidth + rect.getX1();
}



