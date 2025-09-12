/**
  * @file Display.h
  * @date June 2025
  * @brief Abstract class defining the interface for the interaction with a graphical output device
  * 
  * This class defines methods to interact with an abstract graphical interface like an TFT or OLED screen.
  * @defgroup ELE3312
  * @{
  */
#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_

#include <cstdint>
#include <string>
#include "Interfaces/Geometry/Rect.h"


namespace ELE3312 {

	/** @brief enum class that represents a selection of displayable colors.
	  */
	enum class Color {
		BLACK,
		NAVY,
		DARKGREEN,
		DARKCYAN,
		MAROON,
		PURPLE,
		OLIVE,
		LIGHTGREY,
		DARKGREY,
		BLUE,
		GREEN,
		CYAN,
		RED,
		MAGENTA,
		YELLOW,
		WHITE,
		ORANGE,
		GREENYELLOW,
		PINK,
	};

	/** @brief Abstract graphics class that models an interface to an abstract graphical device.
	 * 
	 * The contained methods represent a set of functions expected to be provided from a 
	 * graphical device in order to be used as an graphics output in the game application.
	 * @todo change all coordinates for unsigned values since negative values make no sense
	 * @todo add overloaded function that accept Point objects 
	 */
	class Display {
	public:

		/** @brief Destructor for the Display interface.
		 */
		virtual ~Display() = default;

		/** @brief Clear the screen.
		  * @details The function removes all graphical objects from the screen
		  * and replaces them with a homogeneous color.
		  */
		virtual void clearScreen() = 0;

		/** @brief Fills the entire screen with the specified color.
		  * @details The function fills the screen with the specified color so that
		  * none of the previous displayed objects remain.
		  * 
		  * @param [in] The desired color.
		  */
		virtual void fillScreen(Color color) = 0;

		/** @brief Draws a colored pixel on the screen.
		  * @details The function draws a single pixel with the specified color
		  * at the x and y position on the screen.
		  * 
		  * @param [in] color The color of the pixel
		  * @param [in] x The x position of the pixel
		  * @param [in] y The y position of the pixel
		  */
		virtual void drawPixel(Color color, uint16_t x, uint16_t y) = 0;
		
		/** @brief Draws a line on the screen.
		  * @details The function draws a line between the two endpoints (x0,y0) and (x1, y1) 
		  * with the specified color.
		  * 
		  * @param [in] color The line color.
		  * @param [in] x0 The x position of the first point.
		  * @param [in] y0 The y position of the first point.
		  * @param [in] x1 The x position of the second point.
		  * @param [in] y1 The y position of the second point.
		  */
		virtual void drawLine(Color color,
				uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) = 0;

		/** @brief Draws a rectangle on the screen.
		  * @details The function draws a colored rectangle with the specified
		  * width and height. The lower left corner of the rectangle is specified 
		  * by the point parameter.
		  * @param [in] color The color of the rectangle.
		  * @param [in] x The x coordinate of the lower left corner.
		  * @param [in] y The y coordinate of the lower left corner.
		  * @param [in] width The width of the rectangle in pixels.
		  * @param [in] height The height of the rectangle in pixels.
		  */
		virtual void drawRect(Color color,
		    uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;
		
		/** @brief Draws a rectangle on the screen.
		  * @details The function draws a rectangle with the specified 
		  * color, dimensions, and position on the screen.
		  * @param [in] color The color of the rectangle specified as an color enumeration.
		  * @param [in] rect A reference to a Rect object specifying the dimensions
		  * and position of the rectangle.
		  */
		virtual void drawRect(Color color, const Rect<uint16_t> &rect) = 0;

		/** @brief Draws a filled rectangle on the screen.
		  * @details The function draws a rectangle on the screen that is 
		  * filled with the specified color. The rectangle has the specified 
		  * width and height and it's lower left corner is positioned at the 
		  * given x and y coordinates.
		  * @param [in] x The x position of the lower left corner.
		  * @param [in] y The y position of the lower left corner.
		  * @param [in] width The width of the rectangle in pixels.
		  * @param [in] height The height of the rectangle in pixels.
		  */
		virtual void fillRect(Color color,
		    uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;

		/** @brief Draws a filled rectangle on the screen.
		  * @details The function draws a rectangle on the screen that is
		  * filled with the specified color. The Rectangle has the position
		  * and dimension of the Rect object specified as a const reference 
		  * parameter.
		  * @param [in] color The color of the filled rectangle.
		  * @param [in] rect A const reference to a Rect object that specifies the 
		  * dimension and position of the rectangle.
		  */
		virtual void fillRect(Color color, const Rect<uint16_t> &rect) = 0;
		
		/** @brief Draws a circle on the screen.
		  * @details The function draws a colored circle on the screen. The center of the 
		  * circle is positioned at the specified x and y coordinates. The size of the 
		  * circle is defined by the specified radius.
		  * @param [in] color The color of the circle.
		  * @param [in] x The x position of the circle's center.
		  * @param [in] y The y position of the circle's center.
		  * @param [in] radius The radius of the circle in pixels.
		  */ 
		virtual void drawCircle(Color color,
		    uint16_t x, uint16_t y, uint16_t radius) = 0;

		/** @brief Draws a filled circle on the screen.
		  * @details The function draws a colored filled circle on the screen.
		  * The circle is positioned at the specified x and y coordinates and 
		  * it's size is determined by the specified radius.
		  * @param [in] color The color of the filled circle.
		  * @param [in] x The x position of the center of the circle.
		  * @param [in] y The y position of the center of the circle.
		  * @param [in] radius The radius of the circle in pixels.
		  */
		virtual void fillCircle(Color color,
		    uint16_t x, uint16_t y, uint16_t r) = 0;

		/** @brief Draws a single character on the screen.
		  * @details The function draws a single white ASCII character at the 
		  * specified coordinates on the screen.
		  * @param [in] x The x position of the lower left corner of the character.
		  * @param [in] y The y position of the lower left corner of the character.
		  * @param [in] character The ASCII character to be drawn on the screen.
		  */
		virtual void drawChar(uint16_t x, uint16_t y, char character) = 0;

		/** @brief Draws a text string on the Screen.
		  * @details The function renders a text string on the screen.
		  * The text has the specified color and in its lower left corner is positioned 
		  * at the specified x and y coordinate.
		  * @param [in] x The x position of the lower left corner of the text string.
		  * @param [in] y The y position of the lower left corner of the text string.
		  * @param [in] text The text string that is displayed on the screen.
		  * @param [in] color The color of the rendered text.
		  */
		virtual void drawString(uint16_t x, uint16_t y, const std::string text, Color color= Color::WHITE) = 0;

		/** @brief Draws a filled segment of a circle on the screen.
		  * @details The function draws a segment of a circle at the specified x and y position.
		  * The segment is filled with the specified color.
		  * @param [in] x The x position of the center of the underlying circle.
		  * @param [in] y The y position of the center of the underlying circle.
		  * @param [in] r1 The inner radius of the underlying circle.
		  * @param [in] r2 The outer radius of the underlying circle.
		  * @param [in] start The start of the segment in radians. 
		  * @param [in] end  The end of the segment in radians.
		  * @param [in] color The color of the filled segment of a circle.
		  */
		virtual void fillArc( uint16_t x, uint16_t y, uint16_t r1, uint16_t r2, float start, float end, Color color) = 0;

		/** @brief Draws a bitmap graphic on the screen.
		  * @details The function takes the specified bitmap pixel data and displays 
		  * it on the screen. The lower left corner of the bitmap (specified by it's x and y coordinates)
		  * is used to position the bitmap on the screen. The width and height information are used
		  * correctly draw the bitmap on the screen. It is crucial that their values correspond 
		  * to the provided array data. 
		  * @param [in] data A one dimensional array of 16 bit pixel color values.
		  * @param [in] x The x position of the lower left corner of the rectangular bitmap.
		  * @param [in] y The y position of the lower left corner of the rectangular bitmap.
		  * @param [in] width The width of the bitmap.
		  * @param [in] height The height of the bitmap.
		  */
		virtual void drawBitmap(uint16_t *data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;
		
		/** @brief Returns the width of the screen in pixels
		  * @retval The width of the screen in pixels;
		  */
		virtual uint16_t getScreenWidth() const = 0;

		/** @brief Returns the height of the screen in pixels.
		  * @retval The height of the screen in pixels.
		  */
		virtual uint16_t getScreenHeight() const = 0;

		/** @brief Returns the current background color.
		  * @retval The current background color.
		  */
		virtual Color getBackgroundColor() const = 0;
		
		/** @brief Sets the background color.
		  * @details The background color is used by functions that need to 
		  * erase graphical objects from the screen while maintaining a uniform 
		  * appearance of the displayed graphics.
		  * @param color The new background color.
		  */
		virtual void setBackgroundColor(Color color) = 0;

		/**
		 * @brief Function that reads the graphics memory and stores it in the provided array.
		 *        It is important to know that each pixel is represented as 3 bytes (red [7:3], green [7:2], and
		 *        blue [7:3]). Thus, the data array has to be 3 times the number of bytes in the rectangular
		 *        area defined by the two corners (top-left and bottom-right).
		 * @param x0 The x position of the top-left corner.
		 * @param y0 The y position of the top-left corner.
		 * @param x1 The x position of the bottom-right corner.
		 * @param y1 The y position of the bottom-right corner.
		 * @param data An array in which the pixel data will be stored.
		 */
		virtual void readMemory(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t data[]) = 0;
};
} // End of namespace ELE3312
/** 
  *@}
  */ // End of documentation group ELE3312

#endif /* INC_GRAPHICS_H_ */
