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
};

// Constantes du jeu
const int BOARD_WIDTH = 32;   // 32 tuiles de 10x10 = 320 pixels
const int BOARD_HEIGHT = 24;  // 24 tuiles de 10x10 = 240 pixels
const int TILE_SIZE = 10;     // Taille d'une tuile en pixels
const int MAX_SNAKE_LENGTH = 101;
const int MAX_FRUITS = 101;

// Structure du jeu Snake
struct snake_game {
    tile* head;                    // Pointeur vers la tête du serpent
    std::vector<tile>* body;       // Pointeur vers le vecteur contenant le corps
    tile fruits[MAX_FRUITS];
    int fruit_count;
    int score;
};

ILI9341Display display;
snake_game game;

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
            color = Color::RED;
            break;
        case TILE_CHERRY:
            color = Color::MAGENTA;
            break;
        default:
            color = Color::WHITE;
            break;
    }
    
    // Dessiner la tuile avec un petit contour
    display.fillRect(color, pixel_x + 1, pixel_y + 1, TILE_SIZE - 2, TILE_SIZE - 2);
    
    // Ajouter un contour pour la tête du serpent
    if (tile_id == TILE_SNAKE_HEAD) {
        display.drawRect(Color::YELLOW, pixel_x, pixel_y, TILE_SIZE, TILE_SIZE);
    }
}

/**
 * @brief Affiche tout le plateau de jeu
 */
void draw_game_board() {
    // Effacer l'écran
    display.fillScreen(Color::BLACK);
    
    // Dessiner la tête du serpent
    if (game.head != nullptr) {
        draw_tile(game.head->x, game.head->y, game.head->id);
    }
    
    // Dessiner le corps du serpent
    if (game.body != nullptr) {
        for (const auto& body_part : *game.body) {
            draw_tile(body_part.x, body_part.y, body_part.id);
        }
    }
    
    // Dessiner les fruits
    for (int i = 0; i < game.fruit_count; i++) {
        draw_tile(game.fruits[i].x, game.fruits[i].y, game.fruits[i].id);
    }
    
    // Afficher le score
    char score_text[50];
    sprintf(score_text, "Score: %d", game.score);
    display.drawString(10, 10, score_text, Color::WHITE);
    
    // Afficher les informations du jeu
    int total_snake_length = (game.body != nullptr) ? 1 + game.body->size() : 0;
    char info_text[50];
    sprintf(info_text, "Serpent: %d, Fruits: %d", total_snake_length, game.fruit_count);
    display.drawString(10, 25, info_text, Color::CYAN);
}

/**
 * @brief Initialise le jeu Snake
 */
void init_game() {
    game.score = 0;
    
    // Initialiser les pointeurs
    game.head = nullptr;
    game.body = nullptr;
    
    // Initialiser le générateur de nombres aléatoires
    srand(HAL_GetTick());
    
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
    
    // Initialiser le jeu
    init_game();
    
    // Afficher le plateau de jeu initial
    draw_game_board();
    
    // Boucle principale de démonstration
    uint32_t last_update = 0;
    int demo_step = 0;
    
    while(1) {
        uint32_t current_time = HAL_GetTick();
        
        // Mise à jour toutes les 3 secondes pour la démonstration
        if (current_time - last_update > 3000) {
            last_update = current_time;
            
            // Effacer et regénérer le jeu pour la démonstration
            init_game();
            draw_game_board();
            
            demo_step++;
            
            // Afficher un message de démonstration
            char demo_text[50];
            sprintf(demo_text, "Demo - Etape %d", demo_step);
            display.drawString(10, 40, demo_text, Color::YELLOW);
            
            // Afficher les types de fruits
            display.drawString(10, 55, "Pommes (rouge), Cerises (magenta)", Color::WHITE);
        }
        
            HAL_Delay(100);
    }
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
