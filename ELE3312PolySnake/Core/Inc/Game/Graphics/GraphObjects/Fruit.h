#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_FRUIT_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_FRUIT_H_

#include "Game/Graphics/GraphObjects.h"
#include "Interfaces/Display/Point.h"
#include "Interfaces/Display/Sprite.h"
#include <cstdint>

enum class FruitType {
    APPLE = 0,
    BANANA = 1,
};

class Fruit : public GraphObject {
public:
    // Constructeur
    Fruit();
    Fruit(FruitType type, uint16_t gridX, uint16_t gridY);
    virtual ~Fruit() = default;
    
    // Méthodes d'initialisation
    void setup(const Rect &rect, Display *disp) override;
    void initializeFruit(FruitType type, uint16_t gridX, uint16_t gridY);
    
    // Méthodes de dessin
    void draw() override;
    void clear() override;
    
    // Getters et setters
    FruitType getType() const { return type; }
    void setType(FruitType newType) { type = newType; }
    
    uint16_t getGridX() const { return gridX; }
    uint16_t getGridY() const { return gridY; }
    void setGridPosition(uint16_t x, uint16_t y);
    
    bool isActive() const { return active; }
    void setActive(bool newActive) { active = newActive; }
    
    // Méthodes utilitaires
    bool isAtPosition(uint16_t x, uint16_t y) const;
    void consume(); // Marquer le fruit comme consommé
    void respawn(uint16_t newX, uint16_t newY); // Faire réapparaître le fruit à une nouvelle position
    
    // Méthodes statiques pour les couleurs et sprites
    static Color getFruitColor(FruitType type);
    static const char* getFruitName(FruitType type);
    static Sprite* getFruitSprite(FruitType type);

    void updatePixelPosition(); // Mettre à jour la position en pixels basée sur gridX/gridY
    void drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const; // Dessiner un sprite

private:
    FruitType type;
    uint16_t gridX;  // Position en grille (pas en pixels)
    uint16_t gridY;  // Position en grille (pas en pixels)
    bool active;     // Si le fruit est actif et visible
};

#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_FRUIT_H_ */
