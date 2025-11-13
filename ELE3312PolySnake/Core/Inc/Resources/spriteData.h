#ifndef SPRITE_DATA_GUARD
#define SPRITE_DATA_GUARD

#include "Interfaces/Display/Sprite.h"
#include "Interfaces/Display/Display.h"

#include <memory>
#include <vector>

// === Couleurs en RGB565 ===
#define COLOR_TRANSPARENT 20  // noir (ou transparent)
#define COLOR_BLUE		  9
#define COLOR_WHITE       15
#define COLOR_GRAY        7
#define COLOR_GREEN       10
#define COLOR_DARKGREEN   2
#define COLOR_RED         12
#define COLOR_YELLOW      14
#define COLOR_BROWN       4

// ==========================
// Déclarations externes des sprites
// ==========================

// Données des sprites
extern uint16_t sprite_snake_head[];
extern uint16_t sprite_snake_body[];
extern uint16_t sprite_snake_tail[];
extern uint16_t sprite_bg_white[];
extern uint16_t sprite_bg_gray[];
extern uint16_t sprite_apple[];
extern uint16_t sprite_banana[];

// Sprites
extern Sprite snake_head;
extern Sprite snake_body;
extern Sprite snake_tail;
extern Sprite bg_white;
extern Sprite bg_gray;
extern Sprite apple;
extern Sprite banana;

// Vecteur des sprites
extern std::vector<Sprite*> sprites;

#endif
