#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_MYSNAKE_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_MYSNAKE_H_

#include "Game/Graphics/GraphObjects.h"
#include "Game/Graphics/GraphObjects/Head.h"
#include "Game/Graphics/GraphObjects/BodyPart.h"
#include "Resources/Direction.h"
#include "Resources/spriteData.h"
#include "Interfaces/Display/Sprite.h"
#include "Interfaces/Display/Point.h"
#include <cstdint>
#include <vector>
#include <memory>
#include "NucleoImp/Keypad/GPIOKeypad.h"



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
    void moveBodyOnly();
    
    // Méthodes de dessin
    void draw() override;
    void clear() override;
    
    // Getters pour les GraphObjects (système uniforme)
    Head* getHead() const { return head.get(); }
    const std::vector<BodyPart>& getBody() const { return body; }
    uint16_t getLength() const { return 1 + body.size(); } // 1 pour la tête + taille du corps
    Direction getCurrentDirection() const { return currentDirection; }
    void setDirection(Direction direction) ;

    
    // Getters pour les positions en grille
    uint16_t getHeadX() const;
    uint16_t getHeadY() const;
    uint16_t getTailX() const;
    uint16_t getTailY() const;
    
    void setHeadPosition(uint16_t x, uint16_t y); // Nouvelle méthode pour positionner la tête
    
    Rect getOldTail() const { return oldTailPosition; }
    void setOldTail(Rect rectangle) {oldTailPosition = rectangle; }
    
    // Gestion des couleurs
    void setColor(uint16_t primaryColor, uint16_t secondaryColor);
    uint16_t getPrimaryColor() const { return primaryColor_; }
    uint16_t getSecondaryColor() const { return secondaryColor_; }

    // Méthodes utilitaires
    bool isPositionOccupied(uint16_t x, uint16_t y) const;
    bool checkCollision(uint16_t x, uint16_t y) const; // Vérifier collision avec le corps
    void reset(); // Remettre à zéro le serpent
    void addBodyPart();

private:
    Direction currentDirection; // Direction courante
    uint16_t primaryColor_ = COLOR_GREEN;   // Couleur principale (remplace COLOR_GREEN)
    uint16_t secondaryColor_ = COLOR_DARKGREEN; // Couleur secondaire (remplace COLOR_DARKGREEN)
    
    // GraphObjects uniformes
    std::unique_ptr<Head> head;        // Objet tête
    std::vector<BodyPart> body;        // Objets corps
    Rect oldTailPosition;
    
    // Méthodes privées
    bool isValidPosition(uint16_t x, uint16_t y) const; // Vérifier si une position est valide
    void drawSprite(uint16_t x, uint16_t y, Sprite *sprite) const; // Dessiner un sprite
    void moveHead(Direction direction); // Déplacer la tête dans une direction
    void moveBody(); // Déplacer le corps pour suivre la tête
     // Ajouter une nouvelle partie du corps
};

#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_MYSNAKE_H_ */
