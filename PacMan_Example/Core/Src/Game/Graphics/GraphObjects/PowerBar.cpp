/**
  * @file PowerBar.cpp
  * @date June 2025
  * @brief This file implements the PowerBar graphical object that draws the progress/power bar
  * in the combat phase on the screen.
  * 
  * @defgroup ELE3312
  * @{
  */
#include "Game/Graphics/GraphObjects/PowerBar.h"


namespace ELE3312 {

/** @brief Setup routine for the PowerBar graphical object class.
  * @param [in] rect A rectangle defining the size and position of the PowerBar.
  * @param [in] disp A Display object that is used to draw the PowerBar on the screen.
  * @param [in] orientation An Orientation enum that decides the location of the PowerBar (Left or Right).
  * @param [in] color The color of the PowerBar. It is intended to match the PowerBar to the corresponding 
  * player character's main color.
  */
void PowerBar::setup(const Rect & rect, Display *disp, Orientation orientation, Color color) {
	this->rect = rect;
	this->disp = disp;
	this->orientation = orientation;
	this->color = color;
	targetPosition = 100;
	playerPosition = 0;
	value = 0;
	numberNotes = 20;
	noteCount = 0;
	width = 10; // Default width for the bar 
	touchDetected = false;
	noteComplete = false;
	inputEnabled = true;
	startNote.setNote(NoteName::C, 4);
	stopNote.setNote(NoteName::B,4); // Two octaves
	noteRange = startNote.getDistance(stopNote);	
	highlightColor = Color::MAGENTA;
	// Calculate the indicator height based on the number of different notes playable on the bar
	indicatorHeight = disp->getScreenHeight() / noteRange;
	// Calculate the number of notes to play to complete the bar

	
}

/** @brief Default constructor for the PowerBar class.
  */
PowerBar::PowerBar() {
}
/** @brief Draws the target that has to be matched in order to increment the value of the PowerBar
  * and ultimately win the combat.
  * @param [in] position The y position of the target token.
  * @param [in] color The color of the target token.
  */
void PowerBar::drawTarget(uint16_t position, Color color) const {
	// Remove old target indicator from screen
	uint16_t x = rect.getX1() + width + separator;
	disp->fillRect(disp->getBackgroundColor(), Rect{{x, targetPosition}, width, indicatorHeight});
	// Update targetPosition and draw new target on screen
	disp->fillRect(color, Rect{{x, position}, width, indicatorHeight});
}

/** @brief Draws the player's token that has to be matched with the target token 
  * in order to generate a tone and increment the value of the PowerBar.
  * @param [in] position The y position of the token.
  * @param [in] color The color of the token.
  */
void PowerBar::drawPlayer(uint16_t position, Color color) const {
	// Remove old target indicator from screen
	uint16_t x = rect.getX1() + separator;
	if (orientation == Orientation::LEFT) {
		x += width + width;
	}
	disp->drawRect(disp->getBackgroundColor(), Rect{{x, playerPosition}, width, indicatorHeight});
	// Update targetPosition and draw new target on screen
	disp->drawRect(color, Rect{{x, position}, width, indicatorHeight});
}

/** @brief Helper function that is used to determine if the player's token ant the target token 
  * vertically coincide.
  * @param [in] targetPosition The y position of the target.
  * @param [in] playerPosition The y psoition of the player's token.
  * @retval true The two tokens coincide.
  * @retval false The two tokens don't coincide.
  */
bool PowerBar::isTouching(uint16_t targetPosition_, uint16_t playerPosition_) {
	bool cond1 = targetPosition_ < (playerPosition_ + indicatorHeight) && targetPosition_ > playerPosition_;
	bool cond2 = (targetPosition_ + indicatorHeight) > playerPosition_ && targetPosition_ < playerPosition_;
	bool cond3 = targetPosition_ == playerPosition_;
	touchDetected = cond1 || cond2 || cond3;
	return touchDetected;
}

/** @brief Returns whether the player's token and the target token currently coincide.
  * @retval true The two tokens coincide.
  * @retval false The two tokens don't coincide.
  */
bool PowerBar::isMatched() const {
	return touchDetected;
}

/** @brief Returns whether or not the current note was finished.
  * @retval true The current note is complete.
  * @retval false The current note is not complete.
  */
bool PowerBar::isNoteComplete() const {
	return noteComplete;
}

/** @brief Returns the number of notes completed by the player.
  * @return The number of finished notes.
  */
uint16_t PowerBar::getNumberNotesPlayed() const{
	return noteCount; 
}

/** @brief Returns whether or not the player has played all notes.
  * @retval true The player successfully has played all notes.
  * @retval false The player has not completed all notes.
  */
bool PowerBar::isDone() const {
	return noteCount == numberNotes;
}

/** @brief The method updates the position of the target token.
  * @pram [in] newPos The new y position of the target token.
  */
void PowerBar::updateTargetPosition(uint16_t newPos) {
	uint16_t lowerYPos = newPos + indicatorHeight;
	if (lowerYPos > disp->getScreenHeight()){
		// The new position exceeds the screen height -> saturate
		newPos = disp->getScreenHeight() - indicatorHeight;		
	}
	if ( isTouching(newPos, playerPosition) ) {
		drawTarget(newPos, highlightColor);
		drawPlayer(playerPosition, highlightColor);
	} else {
		drawTarget(newPos, color);
		drawPlayer(playerPosition, color);
	}
	targetPosition = newPos;
}

/** @brief The method updates the position of the player's token.
  * @param [in] newPos The new y position of the player's token.
  */
void PowerBar::updatePlayerPosition(uint16_t newPos) {
	uint16_t lowerYPos = newPos + indicatorHeight;
	if (lowerYPos > disp->getScreenHeight()){
		// The new position exceeds the screen height -> saturate
		newPos = disp->getScreenHeight() - indicatorHeight;		
	}
	if ( isTouching(targetPosition, newPos) ) {
		drawPlayer(newPos, highlightColor);
		drawTarget(targetPosition, highlightColor);
	} else {
		drawPlayer(newPos, color);
		drawTarget(targetPosition, color);
	}
	playerPosition = newPos;
}

/** @brief This method increments the value of the PowerBar. It stores the new value
  * and updates the screen to indicate the new PowerBar value.
  * @param [in] incValue The value by which the PowerBar will be incremented.
  */
void PowerBar::incrementValue(uint16_t incValue) {
	// Saturate the bar if the increment exceeds the screen height
	if (value + incValue <= disp->getScreenHeight()) {
		value += incValue;
	} else {
		value = disp->getScreenHeight();
	}
	uint16_t barY = disp->getScreenHeight() - value;
	uint16_t barX = rect.getX1();
	if (orientation == Orientation::RIGHT) {
		 barX += rect.getWidth() - width;
	} 
	disp->fillRect(color, {{barX, barY}, width, incValue} );
	noteComplete = value % (disp->getScreenHeight()/numberNotes)  == 0;
	if (noteComplete) {
		noteCount += 1;
	}
}

/** @brief This method changes the value of the PowerBar and updates the 
  * the screen. The method is mainly used to update the PowerBar of a remote
  * player.
  * @param [in] newValue The new value of the PowerBar.
  */
void PowerBar::updateValue(uint16_t newValue) {
	if (newValue == value){
		return;
	}
	// Saturate the bar if the increment exceeds the screen height
	if (newValue <= disp->getScreenHeight()) {
		value = newValue;
	} else {
		value = disp->getScreenHeight();
	}
	uint16_t barY = disp->getScreenHeight() - value;
	uint16_t barX = rect.getX1();
	if (orientation == Orientation::RIGHT) {
		 barX += rect.getWidth() - width;
	} 
	disp->fillRect(color, {{barX, barY}, width, value} );
	if (value == disp->getScreenHeight()) {
		noteCount = numberNotes;
	}
}

/** @brief Decrements the value of the PowerBar by the specified value.
  * @param [in] decValue The value by which the PowerBar is decremented. 
  */
void PowerBar::decrementValue(uint16_t decValue) {
	// Prevent the bar from becomming negative
	if (value - decValue < 0) {
		decValue = value;
	}
	uint16_t barY = disp->getScreenHeight() - value;
	value -= decValue;
	uint16_t barX = rect.getX1();
	if (orientation == Orientation::RIGHT) {
		 barX += rect.getWidth() - width;
	} 
	disp->fillRect(disp->getBackgroundColor(), {{barX, barY}, width, decValue} );
}

/** @brief Sets the width of the PowerBar
  * @param [in] newWidth The new width of the PowerBar.
  * @todo Change of width is not implemented. Maybe not even needed.
  */
void PowerBar::setWidth(uint16_t newWidth) {
	width = newWidth;
}

/** @brief Enables/Disabled the input for the PowerBar.
  * @param [in] enabled Boolean value that determines whether the input for the PowerBar is 
  * enabled (true) or not (false).
  */
void PowerBar::setInputEnabled(bool enabled) {
	inputEnabled = enabled;
}

/**  @brief Draws the PowerBar on the screen.
  */
void PowerBar::draw() {
	uint16_t barY = disp->getScreenHeight() - value;
	uint16_t barX = rect.getX1();
	if (orientation == Orientation::RIGHT) {
		 barX += rect.getWidth() - width;
	} 
	disp->fillRect(color, {{barX, barY}, width, value} );
}

/** @brief Removes the PowerBar from the screen by over-painting it with the 
  * background color.
  */
void PowerBar::clear() {
	disp->fillRect(disp->getBackgroundColor(), rect);
}

/** @brief Sets the value of the PowerBar.
  * @param [in] newValue The new value of the PowerBar.
  */
void PowerBar::setValue(uint16_t newValue) {
	value = newValue;
}

/** Returns the current value of the PowerBar.
  * @return The current value of the PowerBar.
  */
uint16_t PowerBar::getValue() const {
	return value;
}

/** @brief Returns the current note of the target token.
  * @return The current note of the target token.
  */
Note PowerBar::getCurrentNote() const {
	return currentNote;
}

/** @brief Picks a new Note to play. 
  */
void PowerBar::pickNewNote(){
	// Randomly pick a new note to play
	currentNote = startNote.goUp(rand() % noteRange);
	// Update target
	updateTargetPosition(startNote.getDistance(currentNote)* indicatorHeight);
}

}
/**
  * @}
  */ // End of documentation group ELE3312
