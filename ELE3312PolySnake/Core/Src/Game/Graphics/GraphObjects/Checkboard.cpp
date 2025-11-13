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
	
	// Debug: vérifier les dimensions
	uint16_t hTiles = numHorizontalTiles();
	uint16_t vTiles = numVerticalTiles();
	uint16_t totalTiles = hTiles * vTiles;
	
	// Vérifier que les dimensions correspondent à checkboardData (768 éléments = 32x24)
	if (totalTiles != 768) {
		// Forcer les dimensions à 32x24 si elles ne correspondent pas
		hTiles = 32;
		vTiles = 24;
		totalTiles = 768;
	}

	if(!checkboard.empty()){
		checkboard.clear();
	}
	uint32_t idx = 0; // Index into the maze data array
	uint16_t maxTiles = totalTiles; // Utiliser les dimensions calculées
	for (uint16_t gridY = 0; gridY < vTiles; ++gridY) {
		for (uint16_t gridX = 0; gridX < hTiles; ++gridX) {
			// Protection contre le dépassement d'index
			if (idx >= maxTiles) {
				// Utiliser un sprite par défaut (bg_white = 0)
				checkboard.push_back(sprites[0]);
				continue;
			}
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
	// Utiliser les dimensions fixes 32x24 pour correspondre à checkboardData
	uint16_t hTiles = 32;
	uint16_t vTiles = 24;
	
	// Debug: vérifier l'état
	if (!disp) return;
	if (checkboard.empty()) return;
	
	for (uint16_t gridY = 0; gridY < vTiles; ++gridY) {
		for (uint16_t gridX = 0; gridX < hTiles; ++gridX) {
			uint32_t index = (gridY * hTiles) + gridX;
			if (index < checkboard.size()) {
				auto sprite = checkboard.at(index);
				if (sprite) {
					uint16_t pixelX =toScreenX(gridX);
					uint16_t pixelY =toScreenY(gridY);
					drawSprite(pixelX, pixelY, sprite);
				}
			}
		}
	}

}

void Checkboard::update(){
    if (!ready || !snakeGame) return;

    // Dessiner le serpent complet avec MySnake::draw()
    MySnake* snake = snakeGame->getMySnake();
    if (snake) {
        snake->draw(); // Dessine automatiquement la tête et le corps avec les sprites
    }
    MySnake* snakeOpponent = snakeGame->getSnakeOpponent();
    if (snakeOpponent) {
    	snakeOpponent->drawOpponent(); // Dessine automatiquement la tête et le corps avec les sprites
    }

    
    // Dessiner les fruits avec leur méthode draw()
    const auto& fruits = snakeGame->getFruits();
    for (const auto& fruit : fruits) {
        if (fruit->isActive()) {
            fruit->draw(); // Dessine automatiquement avec le bon sprite selon le type
        }
    }
    // Redessinner à l'ancienne position
    uint16_t X1 = snake->getOldTail().getX1();
    uint16_t Y1 = snake->getOldTail().getY1();
    Sprite* sprite = this->getSpriteAt(Point(X1, Y1));

    this->drawSprite(X1, Y1, sprite);
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
	if (!data) return;
	
	uint16_t width = sprite->getWidth();
	uint16_t height = sprite->getHeight();
	
	for (uint16_t py = 0; py < height; ++py) {
		for (uint16_t px = 0; px < width; ++px) {
			uint16_t pixelColor = data[py * width + px]; // METTRE COLOR à la place de uint16_t
			if (pixelColor != 20) { // Ne pas dessiner les pixels transparents

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
