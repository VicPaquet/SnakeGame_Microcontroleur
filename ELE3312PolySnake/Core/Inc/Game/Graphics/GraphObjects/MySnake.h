#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_MYSNAKE_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_MYSNAKE_H_

#include "Game/Graphics/GraphObjects.h"
#include "Game/Graphics/GraphObjects/Head.h"
#include "Game/Graphics/GraphObjects/BodyPart.h"
#include "Interfaces/Display/Sprite.h"
#include "Interfaces/Display/Point.h"
#include <cstdint>
#include <vector>
#include <memory>
#include "NucleoImp/Keypad/GPIOKeypad.h"


// Directions possibles pour le serpent
enum class Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

class MySnake : public GraphObject {
public:
    // Constructeur
    MySnake();
    virtual ~MySnake() = default;
    
    // Méthodes d'initialisation
    void setup(const Rect &rect, Display *disp) override;
    void initializeSnake(uint16_t startX, uint16_t startY, Direction startDirection, uint16_t initialLength);
    
    // Méthodes de mouvement selon les spécifications du labo
    void move(int eat);  // Faire avancer le serpent d'une position
    void turn(int direction);  // Changer la direction (0=gauche, 1=droite)
    
    // Méthodes de dessin
    void draw() override;
    void clear() override;
    
    // Getters pour les GraphObjects (système uniforme)
    Head* getHead() const { return head.get(); }
    const std::vector<BodyPart>& getBody() const { return body; }
    uint16_t getLength() const { return 1 + body.size(); } // 1 pour la tête + taille du corps
    Direction getCurrentDirection() const { return currentDirection; }
    Direction setDirection() ;

    
    // Getters pour les positions en grille
    uint16_t getHeadX() const;
    uint16_t getHeadY() const;
    uint16_t getTailX() const;
    uint16_t getTailY() const;
    
    // Méthodes utilitaires
    bool isPositionOccupied(uint16_t x, uint16_t y) const;
    bool checkCollision(uint16_t x, uint16_t y) const; // Vérifier collision avec le corps
    void reset(); // Remettre à zéro le serpent

private:
    Direction currentDirection; // Direction courante
    
    // GraphObjects uniformes
    std::unique_ptr<Head> head;        // Objet tête
    std::vector<BodyPart> body;        // Objets corps
    
    // Méthodes privées
    bool isValidPosition(uint16_t x, uint16_t y) const; // Vérifier si une position est valide
    void drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const; // Dessiner un sprite
    void moveHead(Direction direction); // Déplacer la tête dans une direction
    void moveBody(); // Déplacer le corps pour suivre la tête
    void addBodyPart(); // Ajouter une nouvelle partie du corps
};

#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_MYSNAKE_H_ */
