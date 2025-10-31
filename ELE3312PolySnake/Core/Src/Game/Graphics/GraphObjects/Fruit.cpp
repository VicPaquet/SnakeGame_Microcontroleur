#include "Game/Graphics/GraphObjects/Fruit.h"
#include "Game/Graphics/GraphObjects/Checkboard.h"
#include "Interfaces/Display/Rect.h"
#include <Resources/spriteData.h>
#include "Game/Sections/SnakeGame.h"

Fruit::Fruit() : 
    type(FruitType::APPLE),
    gridX(0),
    gridY(0),
    active(false) {
}

Fruit::Fruit(FruitType type, uint16_t gridX, uint16_t gridY) :
    type(type),
    gridX(gridX),
    gridY(gridY),
    active(true) {
}

void Fruit::setup(const Rect &rect, Display *disp) {
    GraphObject::setup(rect, disp);
    updatePixelPosition();
}

void Fruit::initializeFruit(FruitType type, uint16_t gridX, uint16_t gridY) {
    this->type = type;
    this->gridX = gridX;
    this->gridY = gridY;
    this->active = true;
    updatePixelPosition();
}

void Fruit::draw() {
    if (!disp || !active) return;
    
    // Utiliser les sprites
    uint16_t pixelX = rect.getX1();
    uint16_t pixelY = rect.getY1();
    
    Sprite* fruitSprite = getFruitSprite(type);
    if (fruitSprite) {
        drawSprite(pixelX, pixelY, fruitSprite);
    }
}

void Fruit::clear() {
    if (!disp) return;

    Sprite* sprite = getFruitSprite(type);
    if (!sprite) return;

    uint16_t width = sprite->getWidth();
    uint16_t height = sprite->getHeight();

    Rect fruitRect(rect.getX1(), rect.getY1(),
                   rect.getX1() + width, rect.getY1() + height);

    if ((gridX + gridY) % 2 == 0)
        disp->fillRect(Color::WHITE, fruitRect);
    else
        disp->fillRect(Color::LIGHTGREY, fruitRect);
}

void Fruit::setGridPosition(uint16_t x, uint16_t y) {
    gridX = x;
    gridY = y;
    updatePixelPosition();
}

bool Fruit::isAtPosition(uint16_t x, uint16_t y) const {
    return (gridX == x && gridY == y);
}

void Fruit::consume() {
    active = false;
    clear(); // Effacer visuellement le fruit
}

void Fruit::respawn(uint16_t newX, uint16_t newY) {
    gridX = newX;
    gridY = newY;
    active = true;
    updatePixelPosition();
}

Color Fruit::getFruitColor(FruitType type) {
    switch (type) {
        case FruitType::APPLE:
            return Color::RED;
        case FruitType::BANANA:
            return Color::BLUE;
        default:
            return Color::WHITE;
    }
}

const char* Fruit::getFruitName(FruitType type) {
    switch (type) {
        case FruitType::APPLE:
            return "Apple";
        case FruitType::BANANA:
            return "Banana";
        default:
            return "Unknown";
    }
}

Sprite* Fruit::getFruitSprite(FruitType type) {
    switch (type) {
        case FruitType::APPLE:
            return &apple;  // Utiliser le sprite apple de spriteData.h
        case FruitType::BANANA:
            return &banana; // Utiliser le sprite banana de spriteData.h
        default:
            return &apple;  // Par défaut, utiliser la pomme
    }
}

void Fruit::updatePixelPosition() {
    // Convertir les coordonnées de grille en pixels
    uint16_t pixelX = gridX * 10+rect.getX1();
    uint16_t pixelY = gridY * 10+rect.getY1();
    
    // Mettre à jour le rectangle de position
    rect.setX1(pixelX);
    rect.setY1(pixelY);
    rect.setX2(pixelX + 10);
    rect.setY2(pixelY + 10);
}

void Fruit::drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const {
    if (!disp || !sprite) return;
    
    uint16_t *data = sprite->getData();
    if (!data) return;
    
    uint16_t width = sprite->getWidth();
    uint16_t height = sprite->getHeight();
    
    for (uint16_t py = 0; py < height; ++py) {
        for (uint16_t px = 0; px < width; ++px) {
            uint16_t pixelColor = data[py * width + px];
            if (pixelColor != 20) { // Ne pas dessiner les pixels transparents (c'est comme un pixel transparent
                disp->drawPixel(static_cast<Color>(pixelColor), x + px, y + py);
            }
        }
    }
}
