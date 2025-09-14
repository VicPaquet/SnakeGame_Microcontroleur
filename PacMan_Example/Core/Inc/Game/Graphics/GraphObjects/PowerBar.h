/**
  * @file PowerBar.h 
  * @date June 2025
  * @brief Contains the declaration of the PowerBar class that is used to display and manage
  * the power/progress bar of the combat phase of the game.
  * 
  * @defgroup ELE3312
  * @{
  */
#ifndef POWERBAR_H
#define POWERBAR_H

#include "Interfaces/Display/Display.h"
#include "Game/Graphics/GraphObject.h"
#include "Interfaces/Sound/Note.h"


namespace ELE3312 {
/** Defines the position/orientation of the PowerBar.
  */
enum class Orientation {
	LEFT,
	RIGHT
};
/** @brief The PowerBar class represents and manages the power/progress bar of the 
  * combat phase of the game.
  */
class PowerBar : public GraphObject {
public:
	PowerBar();
	void setup(const Rect & rect, Display *disp, Orientation orientation, Color color);

	void updateTargetPosition(uint16_t newPos);
	void updatePlayerPosition(uint16_t playerPos);
	void incrementValue(uint16_t incValue);
	void decrementValue(uint16_t decValue);
	void updateValue(uint16_t newValue); 
	void setValue(uint16_t newValue);
	void setWidth(uint16_t newWidth);
	void setInputEnabled(bool enabled);

	virtual void draw() override;
	virtual void clear() override;

	
	uint16_t getValue() const;
	uint16_t getNumberNotesPlayed() const;
	bool isDone() const;
	bool isMatched() const;
	bool isNoteComplete() const;
	Note getCurrentNote() const;
	void pickNewNote();

private:
	uint16_t targetPosition; //!< The y position of the target token
	uint16_t playerPosition; //!< The y position of the player token
	Orientation orientation; //!< Indicates whether the bar is on the left or the right site of the screen
	uint16_t value; //!< The value of the progress/power bar
	uint16_t indicatorHeight; //!< The height of the target and player token indicators
	uint16_t width; //!< The width of the bar, comprising the actual bar and the two indicator tokens
	uint16_t numberNotes; //!< The number of notes to play
	uint16_t noteCount; //!< Counts the number of played notes
	uint16_t noteRange; //!< The number of different notes on the bar
	Note startNote; //!< The lowest musical note
	Note stopNote; //!< The highest musical note
	Note currentNote; //!< The current musical note that the player has to reach in order to increment the bar.
	const uint16_t separator = 2; //!< Number pixels that separate the bar, target, and player indicator
	Color color; //!< The color of the bar.
	Color highlightColor; //!< The color of the tokens when they intersect.
	bool  touchDetected; //!< Indicates that the two tokens intersect.
	bool  noteComplete; //!< Indicates that the player has finished a note.
	bool  inputEnabled; //!< Indicates that the input is enabled.

	void drawTarget(uint16_t position, Color color) const;
	void drawPlayer(uint16_t position, Color color) const;
	bool isTouching(uint16_t targetPosition_, uint16_t playerPosition_);
};

} // End of namespace ELE3312

/**
  * @}
  */

#endif
