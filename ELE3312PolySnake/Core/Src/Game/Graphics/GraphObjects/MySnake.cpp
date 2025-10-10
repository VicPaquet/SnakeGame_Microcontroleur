/*
 * MySnake.cpp
 *
 *  Created on: Sep 29, 2025
 *      Author: vicpa
 * 
 * Implémentation uniforme de la classe MySnake avec GraphObjects uniquement
 * selon les spécifications du laboratoire ELE3312.
 */

#include "Game/Graphics/GraphObjects/MySnake.h"
#include "Interfaces/Display/Rect.h"
#include "Resources/spriteData.h"
#include "Interfaces/Keypad/Keypad.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"


MySnake::MySnake() : 
    currentDirection(Direction::EAST) {
    // Initialiser l'objet tête
    head = std::make_unique<Head>();
}

void MySnake::setup(const Rect &rect, Display *disp) {
    GraphObject::setup(rect, disp);
}

void MySnake::initializeSnake(uint16_t startX, uint16_t startY, Direction startDirection, uint16_t initialLength) {
    // Réinitialiser le serpent
    currentDirection = startDirection;
    
    // Initialiser la tête
    if (head && disp) {
        uint16_t pixelX = startX * 10 + rect.getX1();
        uint16_t pixelY = startY * 10 + rect.getY1();
        Rect headRect(pixelX, pixelY, pixelX + 10, pixelY + 10);
        head->setup(headRect, disp);
    }
    
    // Créer le corps initial
    body.clear();
    uint16_t currentX = startX;
    uint16_t currentY = startY;
    
    // Créer le corps en allant dans la direction opposée
    Direction oppositeDir = static_cast<Direction>((static_cast<int>(currentDirection) + 2) % 4);
    
    for (uint16_t i = 1; i < initialLength; i++) {
        // Déplacer dans la direction opposée pour créer le corps
        switch (oppositeDir) {
            case Direction::NORTH:
                currentY--;
                break;
            case Direction::EAST:
                currentX++;
                break;
            case Direction::SOUTH:
                currentY++;
                break;
            case Direction::WEST:
                currentX--;
                break;
        }
        
        // Créer une nouvelle partie du corps
        if (isValidPosition(currentX, currentY)) {
            uint16_t pixelX = currentX * 10 + rect.getX1();
            uint16_t pixelY = currentY * 10 + rect.getY1();
            Rect bodyRect(pixelX, pixelY, pixelX + 10, pixelY + 10);
            BodyPart bodyPart(disp, bodyRect);
            body.push_back(bodyPart);
        }
    }
}

void MySnake::move(int eat) {
    if (!head || !disp) return;
    // Déplacer la tête dans la direction courante
    moveHead(currentDirection);
    
    // Si le serpent mange (eat != 0), ajouter une partie du corps
    if (eat != 0) {
        addBodyPart();
    } else {
        // Déplacer le corps pour suivre la tête
        moveBody();
    }
}

void MySnake::turn(int direction) {
    if (direction == 0) {
        // Tourner à gauche (anti-horaire)
        currentDirection = static_cast<Direction>((static_cast<int>(currentDirection) + 3) % 4);
    } else if (direction == 1) {
        // Tourner à droite (horaire)
        currentDirection = static_cast<Direction>((static_cast<int>(currentDirection) + 1) % 4);
    }
}

void MySnake::draw() {
    if (!disp) return;
    
    // Dessiner la tête avec le sprite snake_head
    if (head) {
        uint16_t pixelX = getHeadX() * 10 + rect.getX1();
        uint16_t pixelY = getHeadY() * 10 + rect.getY1();
        drawSprite(pixelX, pixelY, &snake_head);
    }
    
    // Dessiner le corps avec le sprite snake_body
    for (const auto& bodyPart : body) {
        uint16_t pixelX = (bodyPart.getRect().getX1() - rect.getX1()) / 10 * 10 + rect.getX1();
        uint16_t pixelY = (bodyPart.getRect().getY1() - rect.getY1()) / 10 * 10 + rect.getY1();
        drawSprite(pixelX, pixelY, &snake_body);
    }
}

void MySnake::clear() {
    if (!disp) return;
    
    // Effacer la tête
    if (head) {
        head->clear();
    }
    
    // Effacer le corps
    for (auto& bodyPart : body) {
        bodyPart.clear();
    }
}

uint16_t MySnake::getHeadX() const {
    if (!head) return 0;
    return (head->getRect().getX1() - rect.getX1()) / 10;
}

uint16_t MySnake::getHeadY() const {
    if (!head) return 0;
    return (head->getRect().getY1() - rect.getY1()) / 10;
}

uint16_t MySnake::getTailX() const {
    if (body.empty()) return getHeadX();
    return (body.back().getRect().getX1() - rect.getX1()) / 10;
}

uint16_t MySnake::getTailY() const {
    if (body.empty()) return getHeadY();
    return (body.back().getRect().getY1() - rect.getY1()) / 10;
}

bool MySnake::isPositionOccupied(uint16_t x, uint16_t y) const {
    // Vérifier la tête
    if (head && getHeadX() == x && getHeadY() == y) {
        return true;
    }
    
    // Vérifier le corps
    for (const auto& bodyPart : body) {
        uint16_t bodyX = (bodyPart.getRect().getX1() - rect.getX1()) / 10;
        uint16_t bodyY = (bodyPart.getRect().getY1() - rect.getY1()) / 10;
        if (bodyX == x && bodyY == y) {
            return true;
        }
    }
    
    return false;
}

bool MySnake::checkCollision(uint16_t x, uint16_t y) const {
    // Vérifier collision avec le corps (pas la tête)
    for (const auto& bodyPart : body) {
        uint16_t bodyX = (bodyPart.getRect().getX1() - rect.getX1()) / 10;
        uint16_t bodyY = (bodyPart.getRect().getY1() - rect.getY1()) / 10;
        if (bodyX == x && bodyY == y) {
            return true;
        }
    }
    return false;
}

void MySnake::reset() {
    currentDirection = Direction::EAST;
    body.clear();
    
    if (head) {
        head->clear();
    }
}

void MySnake::drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const {
    if (!disp || !sprite) return;
    
    uint16_t *data = sprite->getData();
    if (!data) return;
    
    uint16_t width = sprite->getWidth();
    uint16_t height = sprite->getHeight();
    
    for (uint16_t py = 0; py < height; ++py) {
        for (uint16_t px = 0; px < width; ++px) {
            uint16_t pixelColor = data[py * width + px];
            if (pixelColor != 20) { // Ne pas dessiner les pixels transparents
                disp->drawPixel(static_cast<Color>(pixelColor), x + px, y + py);
            }
        }
    }
}

void MySnake::moveHead(Direction direction) {
    if (!head || !disp) return;
    
    uint16_t currentX = getHeadX();
    uint16_t currentY = getHeadY();
    head->setOldHead(currentX, currentY);
    uint16_t newX = currentX;
    uint16_t newY = currentY;
    
    // Calculer la nouvelle position selon la direction
    switch (direction) {
        case Direction::NORTH:
            newY--;
            break;
        case Direction::EAST:
            newX++;
            break;
        case Direction::SOUTH:
            newY++;
            break;
        case Direction::WEST:
            newX--;
            break;
    }
    
    // Vérifier si la nouvelle position est valide
    if (isValidPosition(newX, newY)) {
        // Mettre à jour la position de la tête
        uint16_t pixelX = newX * 10 + rect.getX1();
        uint16_t pixelY = newY * 10 + rect.getY1();
        head->setPosition(pixelX, pixelY);
    }
}

void MySnake::moveBody() {
    if (body.empty() || !head) return;

    
    // Déplacer chaque partie du corps vers la position de la partie précédente

    Rect oldTail = body[body.size() - 1].getRect();
    this->setOldTail(oldTail);
    for (int i = body.size() - 1; i > 0; i--) {
        uint16_t prevX = (body[i-1].getRect().getX1() - rect.getX1()) / 10;
        uint16_t prevY = (body[i-1].getRect().getY1() - rect.getY1()) / 10;
        
        uint16_t pixelX = prevX * 10 + rect.getX1();
        uint16_t pixelY = prevY * 10 + rect.getY1();
        body[i].setPosition(pixelX, pixelY);
    }
    
    // Déplacer la première partie du corps vers l'ancienne position de la tête
    if (!body.empty()) {
    	uint16_t pixelX = head->getOldHead().getX1() * 10 + rect.getX1();
    	uint16_t pixelY = head->getOldHead().getY1() * 10 + rect.getY1();
        body[0].setPosition(pixelX, pixelY);
    }
}

void MySnake::addBodyPart() {
    if (!disp) return;
    
    // Ajouter une nouvelle partie du corps à la position de la queue
    uint16_t tailX = getTailX();
    uint16_t tailY = getTailY();
    
    uint16_t pixelX = tailX * 10 + rect.getX1();
    uint16_t pixelY = tailY * 10 + rect.getY1();
    Rect bodyRect(pixelX, pixelY, pixelX + 10, pixelY + 10);
    BodyPart bodyPart(disp, bodyRect);
    body.push_back(bodyPart);
}

bool MySnake::isValidPosition(uint16_t x, uint16_t y) const {
    // Vérifier les limites de l'écran (32x24 tuiles)
    return (x < 32 && y < 24);
}



// Définition (unique) du global
//Keypad* g_keypad = nullptr;
//static GPIOKeypad s_keypadImpl; // instance concrète

void MySnake::setDirection(Direction direction) {
	currentDirection = direction;

}
