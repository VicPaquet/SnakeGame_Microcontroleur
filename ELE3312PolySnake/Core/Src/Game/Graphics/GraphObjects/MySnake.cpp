/*
 * MySnake.cpp
 *
 *  Created on: Sep 29, 2025
 *      Author: vicpa
 * 
 * Implémentation uniforme de la classe MySnake avec GraphObjects uniquement
 * selon les spécifications du laboratoire ELE3312.
 */


#include "Game/Sections/SnakeGame.h"
#include "Game/Graphics/GraphObjects/MySnake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"
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
            default:
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

    // 1. Sauvegarder l'ancienne position de la queue (pour l'effacer après)
    Rect oldTail;
    if (!body.empty()) {
        oldTail = body.back().getRect();
        this->setOldTail(oldTail);
    } else {
        // Si pas de corps, l'ancienne queue est la tête actuelle
        oldTail = head->getRect();
        this->setOldTail(oldTail);
    }

    // 2. Sauvegarder l'ancienne position de la tête
    Rect oldHead = head->getRect();

    // 3. Déplacer la tête
    moveHead(currentDirection);

    // 4. Déplacer chaque body part (chacun prend la position du précédent)
    if (!body.empty()) {
        // Déplacer de la queue vers la tête
        for (int i = body.size() - 1; i > 0; i--) {
            Rect prevRect = body[i-1].getRect();
            body[i].setPosition(prevRect.getX1(), prevRect.getY1());
        }

        // Le premier body part prend la position de l'ancienne tête
        body[0].setPosition(oldHead.getX1(), oldHead.getY1());
    }

    // 5. Si on a mangé, ajouter un body part à l'ancienne position de la queue
    if (eat != 0) {
        Rect bodyRect(oldTail.getX1(), oldTail.getY1(),
                      oldTail.getX1() + 10, oldTail.getY1() + 10);
        BodyPart bodyPart(disp, bodyRect);
        body.push_back(bodyPart);
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

void MySnake::moveBodyOnly() {
    if (body.empty() || !head) return;

    // Obtenir l'ANCIENNE position de la tête (avant qu'elle bouge)
    uint16_t oldHeadX = head->getOldHead().getX1();
    uint16_t oldHeadY = head->getOldHead().getY1();

    // Sauvegarder l'ancienne queue avant de déplacer
    Rect oldTail = body[body.size() - 1].getRect();
    this->setOldTail(oldTail);

    // Déplacer chaque partie du corps vers la position de la partie précédente
    // EN PARTANT DE LA FIN (queue vers tête)
    for (int i = body.size() - 1; i > 0; i--) {
        // Copier la position de la partie précédente (plus proche de la tête)
        Rect prevRect = body[i - 1].getRect();
        body[i].setPosition(prevRect.getX1(), prevRect.getY1());
    }

    // Déplacer la première partie du corps vers l'ANCIENNE position de la tête
    if (!body.empty()) {
        uint16_t pixelX = oldHeadX * 10 + rect.getX1();
        uint16_t pixelY = oldHeadY * 10 + rect.getY1();
        body[0].setPosition(pixelX, pixelY);
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
                // Remplacer les couleurs vertes par les couleurs personnalisées
                if (pixelColor == COLOR_GREEN) {
                    pixelColor = primaryColor_;
                } else if (pixelColor == COLOR_DARKGREEN) {
                    pixelColor = secondaryColor_;
                }
                disp->drawPixel(static_cast<Color>(pixelColor), x + px, y + py);
            }
        }
    }
}

void MySnake::setColor(uint16_t primaryColor, uint16_t secondaryColor) {
    primaryColor_ = primaryColor;
    secondaryColor_ = secondaryColor;
}

void MySnake::moveHead(Direction direction) {
    if (!head || !disp) return;
    
    // Position actuelle en grille
    uint16_t currentX = getHeadX();
    uint16_t currentY = getHeadY();

    uint16_t newX = currentX;
    uint16_t newY = currentY;
    
    // Calculer la nouvelle position selon la direction
    switch (direction) {
        case Direction::NORTH: newY--; break;
        case Direction::EAST:  newX++; break;
        case Direction::SOUTH: newY++; break;
        case Direction::WEST:  newX--; break;
        default: break;
    }
    
    // Vérifier si la nouvelle position est valide
    if (isValidPosition(newX, newY)) {
        uint16_t pixelX = newX * 10 + rect.getX1();
        uint16_t pixelY = newY * 10 + rect.getY1();
        head->setPosition(pixelX, pixelY);
    }
}

void MySnake::moveBody() {
    if (body.empty() || !head) return;

    // Déplacer chaque partie du corps vers la position de la partie précédente
    for (int i = body.size() - 1; i > 0; i--) {
        Rect prevRect = body[i-1].getRect();
        body[i].setPosition(prevRect.getX1(), prevRect.getY1());
    }
    
    // ✅ Déplacer la première partie vers l'ancienne tête (PAS de multiplication!)
    if (!body.empty()) {
        uint16_t pixelX = head->getOldHead().getX1();  // Déjà en pixels
        uint16_t pixelY = head->getOldHead().getY1();
        body[0].setPosition(pixelX, pixelY);
    }
}
void MySnake::addBodyPart() {
    if (!disp || !head) return;
    
    // ✅ Utiliser l'ancienne position de la TÊTE (pas la queue!)
    uint16_t pixelX = head->getOldHead().getX1();
    uint16_t pixelY = head->getOldHead().getY1();
    
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
void MySnake::setHeadPosition(uint16_t x, uint16_t y) {
    if (!head || !disp) return;
    
    // **IMPORTANT: Sauvegarder l'ANCIENNE position EN PIXELS**
    uint16_t oldPixelX = head->getRect().getX1();
    uint16_t oldPixelY = head->getRect().getY1();
    head->setOldHead(oldPixelX, oldPixelY);

    // Conversion des coordonnées de grille en pixels
    uint16_t pixelX = x * 10 + rect.getX1();
    uint16_t pixelY = y * 10 + rect.getY1();
    
    // Mettre à jour la position de la tête
    head->setPosition(pixelX, pixelY);
}
