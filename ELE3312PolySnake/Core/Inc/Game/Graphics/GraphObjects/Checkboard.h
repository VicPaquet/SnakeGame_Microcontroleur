#ifndef INC_GAME_GRAPHICS_GRAPHOBJECTS_CHECKBOARD_H_
#define INC_GAME_GRAPHICS_GRAPHOBJECTS_CHECKBOARD_H_

#include "Game/Graphics/GraphObjects.h"
#include "Interfaces/Display/Sprite.h"
#include <vector>
#include <memory>

enum class CollisionType{
		None, Fruits, Opponent, Outline, Unknown,
	};

struct Collision {
	CollisionType type;
	uint16_t x;
	uint16_t y;
};

class Checkboard : public GraphObject {
public:
	void setup(const Rect &rect, Display *disp); //Add PlayerManager later
	virtual ~Checkboard() = default;
	void draw() override;
	void clear() override;
	void update();

	//Collision checkPosition(uint16_t x, uint16_t y, Player *player, Player *opponent);
	void removeCoin(uint16_t x, uint16_t y);
	bool isReady();

private:
	uint16_t gridHeight; // The height in pixels
	uint16_t gridWidth;  // The width in pixels
	std::vector<Sprite*> checkboard; //Contains all the Sprites of the maze

	uint16_t numHorizontalTiles() const;
	uint16_t numVerticalTiles() const;
	uint16_t toScreenX(uint16_t gridX) const;
	uint16_t toScreenY(uint16_t gridY) const;
	void drawSprite(uint16_t x, uint16_t y, Sprite *sprit) const;

	Sprite * getSpriteAt(Point p) const;
	bool ready = false;
};
#endif /* INC_GAME_GRAPHICS_GRAPHOBJECTS_CHECKBOARD_H_ */
