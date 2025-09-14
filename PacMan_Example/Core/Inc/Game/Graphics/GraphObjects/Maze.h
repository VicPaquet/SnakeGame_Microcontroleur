/**
  * @file Maze.h
  * @date June 2025
  * @brief Declaration of the Maze class that is used to draw the maze
  * on the screen and handle all interactions between the players and
  * the objects in the maze.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef MAZE_HEADER_GUARD
#define MAZE_HEADER_GUARD


#include "Game/Graphics/GraphObject.h"
#include "Interfaces/Display/Sprite.h"
#include "Game/Player/PlayerManager.h"
#include <vector>
#include <memory>

namespace ELE3312 {
	/** @brief Represents the different collision types in the maze.
	  */
	enum class CollisionType{
		None, Wall, Opponent, Block, Unknown, TunnelLeft, TunnelRight, TunnelTop, TunnelButtom, Coin
	};

struct Collision {
	CollisionType type;
	uint16_t x;
	uint16_t y;
};

/** @brief The Maze class is a graphical object that draws the maze on the 
  * screen and handles all interactions between the players and other objects
  * in the maze.
  */
class Maze : public GraphObject {
public:
	void setup(const Rect &rect, Display *disp,
			PlayerManager *players) ;
	virtual ~Maze() = default;
	void draw() override;
	void clear() override;
	void update();
	void setNumberLives(uint16_t numLives);
	Collision checkPosition(uint16_t x, uint16_t y, Player *player, Player *opponent);
	void removeCoin(uint16_t x, uint16_t y);
	bool isReady();

private:
	uint16_t gridHeight; //!< The height of a grid rectangle in pixels
	uint16_t gridWidth;  //!< The width of a grid rectangle in pixels
	uint16_t numberLives = 0; //!< The number of hearts shown in the top-right corner of the screen.
	std::vector<Sprite*> maze; //!< Contains all tiles of the maze and it's corresponding sprite objects.
	PlayerManager *players; //!< Pointer to the PlayerManager that contains the meta data of the players

	uint16_t numHorizontalTiles() const;
	uint16_t numVerticalTiles() const;
	uint16_t toScreenX(uint16_t gridX) const;
	uint16_t toScreenY(uint16_t gridY) const;
	void drawSprite(uint16_t x, uint16_t y, Sprite *sprit) const;
	void updateLives();
	Sprite * getSpriteAt(Point p) const;
	bool ready = false;

};
} // End of namespace ELE3312

/**
  * @}
  */ // End of documentation group ELE3312

#endif
