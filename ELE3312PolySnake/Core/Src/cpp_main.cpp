/**
  * @file cpp_main.cpp
  * @date September 2025
  * @brief Jeu Snake simple pour microcontrôleur STM32
  * 
  * Ce fichier implémente un jeu Snake basique avec:
  * - Structure tile pour représenter les éléments du jeu
  * - Serpent avec tête différente du corps
  * - Deux types de fruits (pomme et cerise)
  * - Affichage sur écran ILI9341 (32x24 tuiles de 10x10 pixels)
  * 
  * @defgroup ELE3312
  * @{
  */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "main.h"
#include <cpp_main.h>
#include <NucleoImp/Display/ILI9341Display.h>

using namespace ELE3312;

// IDs des tuiles
enum TileType {
    TILE_EMPTY = 0,
    TILE_SNAKE_HEAD = 1,
    TILE_SNAKE_BODY = 2,
    TILE_APPLE = 3,
    TILE_CHERRY = 4
};

// Structure tile 
struct tile {
    int x;  // Position horizontale (0-31)
    int y;  // Position verticale (0-23) 
    TileType id; // Type de la tuile à afficher
    int mask;   // 4 bits pour les bords (TOP/RIGHT/BOTTOM/LEFT)
};

// Bits utilisés dans tile::mask pour indiquer quelles bordures tracer
const int BORDER_LEFT   = 1 << 0; // 0001
const int BORDER_BOTTOM = 1 << 1; // 0010
const int BORDER_RIGHT  = 1 << 2; // 0100
const int BORDER_TOP    = 1 << 3; // 1000

// Constantes du jeu
const int BOARD_WIDTH = 32;   // 32 tuiles de 10x10 = 320 pixels
const int BOARD_HEIGHT = 24;  // 24 tuiles de 10x10 = 240 pixels
const int TILE_SIZE = 10;     // Taille d'une tuile en pixels
const int MAX_SNAKE_LENGTH = 101;
const int MAX_FRUITS = 101;

// -------------------------
// Outils pour Scan_serpent()
// -------------------------

// Directions cardinales (utile pour naviguer dans le serpent)
enum Dir { UP=0, RIGHT=1, DOWN=2, LEFT=3 };

// Vérifie si une coordonnée est dans le plateau
inline bool in_bounds(int x, int y) {
    return (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT);
}

// Grille d’occupation du plateau (sert à marquer où sont les cases du serpent)
static bool occu[BOARD_HEIGHT][BOARD_WIDTH];

// Réinitialise la grille d’occupation - fonction écrire, mais pas particulièrement utilisé. Gardé pour plus tard dans le LAB
inline void clear_occupancy() {
    for (int j = 0; j < BOARD_HEIGHT; ++j)
        for (int i = 0; i < BOARD_WIDTH; ++i)
            occu[j][i] = false;
}

// ------------------------------
// FIN Outils pour Scan_serpent()
// ------------------------------

// Structure du jeu Snake
struct snake_game {
    tile* head;                // Pointeur vers la tête du serpent
    std::vector<tile>* body;   // Pointeur vers le vecteur contenant le corps
    tile fruits[MAX_FRUITS];
    int fruit_count;
    int score;
};

ILI9341Display display;
snake_game game;

// Analyse la continuité du serpent (si k <-> k+1) et calcule les masks -> Permet l'ajout de bordures
void Scan_serpent() {
    // Étape 0 : vider la grille d’occupation (occu[][] = false)
	clear_occupancy();

	// Étape 1 : marquer les cases occupées par le serpent
    if (game.head && in_bounds(game.head->x, game.head->y)) {
        occu[game.head->y][game.head->x] = true;
    }
    size_t n = (game.body ? game.body->size() : 0);
    if (game.body) {
        for (const auto &bp : *game.body) {
            if (in_bounds(bp.x, bp.y)) {
                occu[bp.y][bp.x] = true;
            }
        }
    }

    // Fonction : calcule la case voisine dans une direction donnée
    auto nb = [](int x, int y, int dir, int &nx, int &ny) {
        nx = x; ny = y;
        switch (dir) {
            case 0: ny = y - 1; break; // UP
            case 1: nx = x + 1; break; // RIGHT
            case 2: ny = y + 1; break; // DOWN
            case 3: nx = x - 1; break; // LEFT
        }
    };

    // Fonction : compare deux coordonnées
    auto eq = [](int ax, int ay, int bx, int by) { return (ax == bx) && (ay == by); };

    // Étape 2 : traiter la tête (body[0])
    if (game.head) {
        int x = game.head->x, y = game.head->y;
        int m = 0;
        int nx, ny;

        // Coordonnées du voisin attendu
        int cx = -1, cy = -1;  // -1 = valeur sentinelle -> aucun voisin
        if (n > 0) { cx = (*game.body)[0].x; cy = (*game.body)[0].y; }

     // Vérifie chaque direction : si pas en continuité -> active la bordure
        // TOP
        nb(x, y, 0, nx, ny);
        if (!(in_bounds(nx, ny) && eq(nx, ny, cx, cy))) m |= BORDER_TOP;
        // RIGHT
        nb(x, y, 1, nx, ny);
        if (!(in_bounds(nx, ny) && eq(nx, ny, cx, cy))) m |= BORDER_RIGHT;
        // BOTTOM
        nb(x, y, 2, nx, ny);
        if (!(in_bounds(nx, ny) && eq(nx, ny, cx, cy))) m |= BORDER_BOTTOM;
        // LEFT
        nb(x, y, 3, nx, ny);
        if (!(in_bounds(nx, ny) && eq(nx, ny, cx, cy))) m |= BORDER_LEFT;

        game.head->mask = m; // sauvegarde du résultat
    }

    // Étape 3 : traiter chaque élément du corps. Chaque case est connectée à son prédécesseur et son successeur
    // (sauf le premier, relié à la tête, et le dernier, relié uniquement au précédent).
    if (game.body) {
        for (size_t i = 0; i < n; ++i) {
            tile &t = (*game.body)[i];
            int x = t.x, y = t.y;
            int m = 0;
            int nx, ny;

            // Coordonnées attendues des voisins (sinon = sentinelle -1,-1)
            int px = -1, py = -1; // précédent
            int qx = -1, qy = -1; // suivant

            // précédent : tête si i==0, sinon body[i-1]
            if (i == 0 && game.head) { px = game.head->x; py = game.head->y; }
            else if (i > 0)          { px = (*game.body)[i-1].x; py = (*game.body)[i-1].y; }

            // suivant : body[i+1] si existe
            if (i + 1 < n) { qx = (*game.body)[i+1].x; qy = (*game.body)[i+1].y; }

            // Fonction : vrai si (nx,ny) correspond à un voisin attendu
            auto is_consecutive_neighbor = [&](int nx_, int ny_) {
                // dernier élément : seulement le précédent compte
                if (i == n - 1) {
                    return eq(nx_, ny_, px, py);
                }
                return (eq(nx_, ny_, px, py) || eq(nx_, ny_, qx, qy));
            };

         // Vérifie chaque direction : si pas en continuité -> active la bordure
            // TOP
            nb(x, y, 0, nx, ny);
            if (!(in_bounds(nx, ny) && is_consecutive_neighbor(nx, ny))) m |= BORDER_TOP;
            // RIGHT
            nb(x, y, 1, nx, ny);
            if (!(in_bounds(nx, ny) && is_consecutive_neighbor(nx, ny))) m |= BORDER_RIGHT;
            // BOTTOM
            nb(x, y, 2, nx, ny);
            if (!(in_bounds(nx, ny) && is_consecutive_neighbor(nx, ny))) m |= BORDER_BOTTOM;
            // LEFT
            nb(x, y, 3, nx, ny);
            if (!(in_bounds(nx, ny) && is_consecutive_neighbor(nx, ny))) m |= BORDER_LEFT;

            t.mask = m; // sauvegarde du résultat
        }
    }
}

// fonctions pour tracer les bordures de 1px
inline void draw_edge_top   (Color c, int px, int py) { display.fillRect(c, px,                 py,                 TILE_SIZE, 1); }
inline void draw_edge_right (Color c, int px, int py) { display.fillRect(c, px + TILE_SIZE - 1, py,                 1,         TILE_SIZE); }
inline void draw_edge_bottom(Color c, int px, int py) { display.fillRect(c, px,                 py + TILE_SIZE - 1, TILE_SIZE, 1); }
inline void draw_edge_left  (Color c, int px, int py) { display.fillRect(c, px,                 py,                 1,         TILE_SIZE); }


// Dessine le serpent (tête + corps) à l’écran en utilisant les masks de bordure.
void Visualisation_serpent() {
	// Fonction : dessine une tuile du serpent avec sa couleur de remplissage
    // px, py = position en pixels (coin haut-gauche de la tuile)
	auto draw_snake_tile = [&](const tile& t, Color fill, Color edge) {
        const int px = t.x * TILE_SIZE;
        const int py = t.y * TILE_SIZE;

        // Étape 1 : remplir entièrement la tuile
        display.fillRect(fill, px, py, TILE_SIZE, TILE_SIZE);

        // Étape 2 : tracer les bordures manquantes (si mask l’indique)
        if (t.mask & BORDER_TOP)    draw_edge_top  (edge, px, py);
        if (t.mask & BORDER_RIGHT)  draw_edge_right(edge, px, py);
        if (t.mask & BORDER_BOTTOM) draw_edge_bottom(edge, px, py);
        if (t.mask & BORDER_LEFT)   draw_edge_left (edge, px, py);
    };

    // Étape 3 : dessiner la tête selon le mask calculé dans Scan_serpent
    if (game.head) {
        // Bordures via mask (pour bien distinguer la tête)
        draw_snake_tile(*game.head, Color::BLUE, Color::MAGENTA);
    }

    	// Étape 4 : dessiner le corps - Même logique que la tête, mais avec d’autres couleurs
    if (game.body) {
        for (const auto& bp : *game.body) {
            draw_snake_tile(bp, Color::DARKGREEN, Color::BLACK);
        }
    }
}

/**
 * @brief Génère un nombre aléatoire entre min et max (inclus)
 */
int random_range(int min, int max) {
    return min + (rand() % (max - min + 1));
}

/**
 * @brief Vérifie si une position est libre (pas occupée par le serpent ou les fruits)
 */
bool is_position_free(int x, int y) {
    // Vérifier la tête du serpent
    if (game.head != nullptr && game.head->x == x && game.head->y == y) {
        return false;
    }
    
    // Vérifier le corps du serpent
    if (game.body != nullptr) {
        for (const auto& body_part : *game.body) {
            if (body_part.x == x && body_part.y == y) {
                return false;
            }
        }
    }
    
    // Vérifier les fruits
    for (int i = 0; i < game.fruit_count; i++) {
        if (game.fruits[i].x == x && game.fruits[i].y == y) {
            return false;
        }
    }
    
    return true;
}


/**
 * @brief Initialise le serpent avec une position aléatoire
 */
void init_snake() {
    // Allouer la mémoire pour la tête
    if (game.head == nullptr) {
        game.head = new tile();
    }
    
    // Allouer la mémoire pour le corps si nécessaire
    if (game.body == nullptr) {
        game.body = new std::vector<tile>();
    }
    
    // Longueur du corps entre 1 et 100 (sans compter la tête)
    int body_length = random_range(1, 101);
    
    // Position de départ aléatoire mais valide
    int start_x = random_range(5, BOARD_WIDTH - 5);
    int start_y = random_range(5, BOARD_HEIGHT - 5);
    
    // Initialiser la tête
    game.head->x = start_x;
    game.head->y = start_y;
    game.head->id = TILE_SNAKE_HEAD;
    game.head->mask = 0; // initialisation du mask à 0000
    
    // Vider le corps précédent
    game.body->clear();
    
    // Créer le corps avec des directions aléatoires
    int current_x = start_x;
    int current_y = start_y;
    
    for (int i = 1; i <= body_length; i++) {
        tile body_part;
        
        // Générer une direction aléatoire (1-4)
        int direction = random_range(1, 4);
        bool position_found = false;
        int attempts = 0;
        const int max_attempts = 10;
        
        // Essayer de trouver une position valide
        while (!position_found && attempts < max_attempts) {
            int new_x = current_x;
            int new_y = current_y;
            
            // Appliquer la direction
            switch (direction) {
                case 1: // Haut
                    new_y = current_y - 1;
                    break;
                case 2: // Droite
                    new_x = current_x + 1;
                    break;
                case 3: // Bas
                    new_y = current_y + 1;
                    break;
                case 4: // Gauche
                    new_x = current_x - 1;
                    break;
            }
            
            // Vérifier si la position est libre en utilisant la fonction existante
            if (is_position_free(new_x, new_y)) {
                body_part.x = new_x;
                body_part.y = new_y;
                body_part.id = TILE_SNAKE_BODY;
                body_part.mask = 0; // initialisation du mask à 0000
                game.body->push_back(body_part);
                
                current_x = new_x;
                current_y = new_y;
                position_found = true;
            }
            
            // Si position non valide, essayer une autre direction
            if (!position_found) {
                direction = (direction % 4) + 1; // Cycle à travers les directions
                attempts++;
            }
        }
        
        // Si on n'a pas trouvé de position valide après max_attempts,
        // placer la partie du corps à côté de la précédente de manière sécurisée
        if (!position_found) {
            body_part.x = current_x;
            body_part.y = current_y;
            body_part.id = TILE_SNAKE_BODY;
            body_part.mask = 0; // initialisation du mask à 0000
            game.body->push_back(body_part);
        }
    }
}

/**
 * @brief Génère des fruits aléatoirement sur le plateau
 */
void generate_fruits() {
    game.fruit_count = random_range(1, 101); // Entre 1 et 101 fruits
    
    for (int i = 0; i < game.fruit_count; i++) {
        int x, y;
        do {
            x = random_range(0, BOARD_WIDTH - 1);
            y = random_range(0, BOARD_HEIGHT - 1);
        } while (!is_position_free(x, y));
        
        game.fruits[i].x = x;
        game.fruits[i].y = y;
        // Alterner entre pomme et cerise
        game.fruits[i].id = (i % 2 == 0) ? TILE_APPLE : TILE_CHERRY;
        game.fruits[i].mask = 0; // initialisation du mask à 0000
    }
}

/**
 * @brief Dessine une tuile à la position donnée
 */
void draw_tile(int x, int y, TileType tile_id) {
    int pixel_x = x * TILE_SIZE;
    int pixel_y = y * TILE_SIZE;
    
    Color color;
    switch (tile_id) {
        case TILE_EMPTY:
            color = Color::BLACK;
            break;
        case TILE_SNAKE_HEAD:
            color = Color::GREEN;
            break;
        case TILE_SNAKE_BODY:
            color = Color::DARKGREEN;
            break;
        case TILE_APPLE:
        	display.drawApple(pixel_x + 1,  pixel_y + 1);
            return;
        case TILE_CHERRY:
            display.drawCherry(pixel_x + 1,  pixel_y + 1);
            return;
        default:
            return;
    }
    
    // Dessiner la tuile avec un petit contour
    display.fillRect(color, pixel_x + 1, pixel_y + 1, TILE_SIZE - 2, TILE_SIZE - 2);
    
    // Ajouter un contour pour la tête du serpent
    if (tile_id == TILE_SNAKE_HEAD) {
        display.drawRect(Color::YELLOW, pixel_x, pixel_y, TILE_SIZE, TILE_SIZE);
    }
}

/**
 * @brief Affiche fond + fruits
 */
void draw_game_board_static() {
    // Effacer l'écran
    display.drawCheckerboard(TILE_SIZE);
    // Dessiner les fruits
    for (int i = 0; i < game.fruit_count; i++) {
        draw_tile(game.fruits[i].x, game.fruits[i].y, game.fruits[i].id);
    }
}

/**
 * @brief Initialise le jeu Snake
 */
void init_game() {
    game.score = 0;
    
    // Initialiser les pointeurs
    game.head = nullptr;
    game.body = nullptr;
    
    // Initialiser le générateur de nombres aléatoires - *9 est pour avoir une situation réaliste de jeu (autrement une situation atypique arrive pour les bordures)
    srand(9*HAL_GetTick());
    
    // Générer le serpent et les fruits
    init_snake();
    generate_fruits();
}

/**
 * @brief Fonction principale du jeu Snake
  */
void cpp_main(peripheral_handles *handles) {
    // Initialiser l'affichage TFT
    display.setup(handles->hspi_tft);
    display.clearScreen();
    
    // Afficher le titre du jeu
    display.drawString(80, 100, "PolySnake - Initialisation", Color::WHITE);
    HAL_Delay(2000);
    
    init_game(); // Initialisation du jeu
    
    // 2) Rendu initial (fond + fruits, puis serpent via mask)
    draw_game_board_static(); // damier + fruits seulement
    Scan_serpent();           // calcule les mask du serpent (tête + corps)
    Visualisation_serpent();  // dessine le serpent selon ces mask
}

/**
 * @brief Nettoie la mémoire allouée pour le jeu
 */
void cleanup_game() {
    if (game.head != nullptr) {
        delete game.head;
        game.head = nullptr;
    }
    
    if (game.body != nullptr) {
        delete game.body;
        game.body = nullptr;
    }
}
