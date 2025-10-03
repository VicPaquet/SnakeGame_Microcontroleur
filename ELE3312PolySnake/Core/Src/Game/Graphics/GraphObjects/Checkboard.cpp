


#include <Resources/checkboardData.h>
#include <Resources/spriteData.h>
#include "Game/Sections/SnakeGame.h"
#include "Game/Graphics/GraphObjects/Checkboard.h"
#include "Game/Graphics/GraphObjects/Head.h"
#include "Game/Graphics/GraphObjects/BodyPart.h"
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
    if (!ready || !snakeGame) return;

    const uint16_t halfGridWidth  = gridWidth / 2;
    const uint16_t halfGridHeight = gridHeight / 2;

    // Dessiner la tête du serpent
    Head* headTile = snakeGame->getHead();
    if (headTile) {
        uint16_t headX = toScreenX((headTile->getRect()).getX1()) - halfGridWidth;
        uint16_t headY = toScreenY((headTile->getRect()).getY1()) - halfGridHeight;
        drawSprite(headX, headY, &snake_head);
    }

    // Dessiner chaque partie du corps
    std::vector<BodyPart*> body = snakeGame->getBody();
    if (body) {
        for (auto& part : *body) {
            uint16_t bodyX = toScreenX(part.getRect().getX1()) - halfGridWidth;
            uint16_t bodyY = toScreenY(part.getRect().getY1()) - halfGridHeight;
            drawSprite(bodyX, bodyY, &snake_body);
        }
    }
}


uint16_t Checkboard::toScreenX(uint16_t gridX) const{
	return gridX * gridWidth + rect.getX1();
}

uint16_t Checkboard::toScreenY(uint16_t gridY) const{
	return gridY * gridHeight + rect.getY1();
}

uint16_t Checkboard::numHorizontalTiles() const{
	return rect.getWidth() / gridWidth;
}

uint16_t Checkboard::numVerticalTiles() const{
	return rect.getHeight() / gridHeight;
}

void Checkboard::drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const{
	if (!disp || !sprite) return;
	
	uint16_t *data = sprite->getData();
	uint16_t width = sprite->getWidth();
	uint16_t height = sprite->getHeight();
	
	for (uint16_t py = 0; py < height; ++py) {
		for (uint16_t px = 0; px < width; ++px) {
			uint16_t pixelColor = data[py * width + px];
			if (pixelColor != 0x0000) { // Ne pas dessiner les pixels transparents
				disp->drawPixel(static_cast<Color>(pixelColor), x + px, y + py);
			}
		}
	}
}

Sprite* Checkboard::getSpriteAt(Point p) const{
	if (checkboard.empty()) return nullptr;
	
	uint16_t gridX = (p.getX() - rect.getX1()) / gridWidth;
	uint16_t gridY = (p.getY() - rect.getY1()) / gridHeight;
	
	if (gridX >= numHorizontalTiles() || gridY >= numVerticalTiles()) {
		return nullptr;
	}
	
	uint32_t index = gridY * numHorizontalTiles() + gridX;
	if (index >= checkboard.size()) return nullptr;
	
	return checkboard[index];
}

void Checkboard::clear(){
	if (!disp) return;
	disp->fillScreen(Color::BLACK);
}

void Checkboard::removeFruit(uint16_t x, uint16_t y){
	if (checkboard.empty()) return;
	
	uint32_t pos = (y * numHorizontalTiles()) + x;
	if (pos >= checkboard.size()) return;
	
	Sprite * fruitSprite = checkboard.at(pos);
	checkboard[pos] = sprites[0]; // Remplacer par le sprite de fond (bg_white)
	drawSprite(toScreenX(x), toScreenY(y), fruitSprite);
}



