/**
 * @file ILI9341Graphics.cpp
 * @date June 2025
 * @brief The class declared in this file implments the generic Graphics interface 
 * for the ILI9341 chip used on the Adafruit TFT board.

 * @defgroup ELE3312
 * @{
 */

#include "NucleoImp/Display/ILI9341Display.h"
#include <cmath>
using namespace ELE3312;

/** @brief Default constructor for the ILI9341Display class.
  */
ILI9341Display::ILI9341Display() : ptext_attr_{nullptr}{

}

/** @brief Destructor for the ILI9341Display class.
  */
ILI9341Display::~ILI9341Display(){
	if(ptext_attr_ != nullptr){
		delete ptext_attr_;
	}
}

/** @brief Setup routine for the ILI9341Display class.
  * @param [in] hspi Handle to the SPI interface connected to the TFT display.
  */
void  ILI9341Display::setup(SPI_HandleTypeDef *hspi){
		this->lcd_ = 	ili9341_new(
				  hspi,
				  Void_Display_Reset_GPIO_Port, Void_Display_Reset_Pin,
				  TFT_CS_GPIO_Port, TFT_CS_Pin,
				  TFT_DC_GPIO_Port, TFT_DC_Pin,
				  isoLandscapeFlip,
				  NULL, 0,
				  NULL, 0,
				  itsNotSupported,
				  itnNormalized);
}

/** @brief Clear the screen.
 * @details The function removes all graphical objects from the screen
 * and replaces them with a homogeneous color.
 */
void ILI9341Display::clearScreen() {
	ili9341_fill_screen(lcd_, ILI9341_BLACK);
}

/** @brief Fills the entire screen with the specified color.
 * @details The function fills the screen with the specified color so that
 * none of the previous displayed objects remain.
 *
 * @param [in] The desired color.
 */
void ILI9341Display::fillScreen(Color color) {
	ili9341_fill_screen(lcd_, ELE3312::colorToILI9341Color(color));
}

/** @brief Draws a colored pixel on the screen.
 * @details The function draws a single pixel with the specified color
 * at the x and y position on the screen.
 *
 * @param [in] color The color of the pixel
 * @param [in] x The x position of the pixel
 * @param [in] y The y position of the pixel
 */
void ILI9341Display::drawPixel(Color color, uint16_t x, uint16_t y) {
	ili9341_draw_pixel(lcd_, ELE3312::colorToILI9341Color(color), x, y);
}

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
void ILI9341Display::drawLine(Color color, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	ili9341_draw_line(lcd_, ELE3312::colorToILI9341Color(color),  x0, y0, x1, y1);
}

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
void ILI9341Display::drawRect(Color color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	ili9341_draw_rect(lcd_, ELE3312::colorToILI9341Color(color), x, y, w, h);
}

/** @brief Draws a rectangle on the screen.
 * @details The function draws a rectangle with the specified
 * color, dimensions, and position on the screen.
 * @param [in] color The color of the rectangle specified as an color enumeration.
 * @param [in] rect A reference to a Rect object specifying the dimensions
 * and position of the rectangle.
 */
void ILI9341Display::drawRect(Color color, const Rect<uint16_t> &rect) {
	ili9341_draw_rect(lcd_, ELE3312::colorToILI9341Color(color), rect.getX1(), rect.getY1(), 
		   rect.getWidth(), rect.getHeight());
}

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
void ILI9341Display::fillRect(Color color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	ili9341_fill_rect(lcd_, ELE3312::colorToILI9341Color(color), x, y, w, h);
}

/** @brief Draws a filled rectangle on the screen.
 * @details The function draws a rectangle on the screen that is
 * filled with the specified color. The Rectangle has the position
 * and dimension of the Rect object specified as a const reference
 * parameter.
 * @param [in] color The color of the filled rectangle.
 * @param [in] rect A const reference to a Rect object that specifies the
 * dimension and position of the rectangle.
 */
void ILI9341Display::fillRect(Color color, const Rect<uint16_t> &rect){
	ili9341_fill_rect(lcd_, ELE3312::colorToILI9341Color(color),  rect.getX1(), rect.getY1(), 
		   rect.getWidth(), rect.getHeight());
}

/** @brief Draws a circle on the screen.
 * @details The function draws a colored circle on the screen. The center of the
 * circle is positioned at the specified x and y coordinates. The size of the
 * circle is defined by the specified radius.
 * @param [in] color The color of the circle.
 * @param [in] x The x position of the circle's center.
 * @param [in] y The y position of the circle's center.
 * @param [in] radius The radius of the circle in pixels.
 */
void ILI9341Display::drawCircle(Color color, uint16_t x, uint16_t y, uint16_t r) {
	ili9341_draw_circle(lcd_, ELE3312::colorToILI9341Color(color), x, y, r);
}

/** @brief Draws a filled circle on the screen.
 * @details The function draws a colored filled circle on the screen.
 * The circle is positioned at the specified x and y coordinates and
 * it's size is determined by the specified radius.
 * @param [in] color The color of the filled circle.
 * @param [in] x The x position of the center of the circle.
 * @param [in] y The y position of the center of the circle.
 * @param [in] radius The radius of the circle in pixels.
 */
void ILI9341Display::fillCircle(Color color, uint16_t x, uint16_t y, uint16_t r) {
	ili9341_fill_circle(lcd_, ELE3312::colorToILI9341Color(color), x, y, r);
}

/** @brief Draws a single character on the screen.
 * @details The function draws a single white ASCII character at the
 * specified coordinates on the screen.
 * @param [in] x The x position of the lower left corner of the character.
 * @param [in] y The y position of the lower left corner of the character.
 * @param [in] character The ASCII character to be drawn on the screen.
 */
void ILI9341Display::drawChar(uint16_t x, uint16_t y, char ch) {
	if (ptext_attr_ == nullptr){
		ptext_attr_ = new ili9341_text_attr_t{&ili9341_font_11x18, ILI9341_WHITE , ILI9341_BLACK,x,y};
	} else {
		ptext_attr_->origin_x = x;
		ptext_attr_->origin_y = y;
	}
	ili9341_draw_char(lcd_, *ptext_attr_, ch);
}

/** @brief Draws a text string on the Screen.
 * @details The function renders a text string on the screen.
 * The text has the specified color and in its lower left corner is positioned
 * at the specified x and y coordinate.
 * @param [in] x The x position of the lower left corner of the text string.
 * @param [in] y The y position of the lower left corner of the text string.
 * @param [in] text The text string that is displayed on the screen.
 * @param [in] color The color of the rendered text.
 */
void ILI9341Display::drawString(uint16_t x, uint16_t y, const std::string text, Color color) {
	if (ptext_attr_ == nullptr){
			ptext_attr_ = new ili9341_text_attr_t{&ili9341_font_11x18,  ELE3312::colorToILI9341Color(color), ILI9341_BLACK,x,y};
	} else {
		ptext_attr_->origin_x = x;
		ptext_attr_->origin_y = y;
		ptext_attr_->fg_color = ELE3312::colorToILI9341Color(color);
	}
	ili9341_draw_string(lcd_, *ptext_attr_, const_cast<char*>(text.c_str()));
}

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
void ILI9341Display::drawBitmap(uint16_t *data, uint16_t x, uint16_t y, uint16_t width, uint16_t height){
	  uint16_t size = +width*height;
	  // Set address region
	  ili9341_spi_tft_set_address_rect(lcd_, x, y, x + width-1, y + height);
	  ili9341_spi_tft_select(lcd_);
	  HAL_GPIO_WritePin(lcd_->data_command_port, lcd_->data_command_pin, __GPIO_PIN_SET__);
	  ili9341_transmit_color(lcd_, size*2, data, ibTrue);
	  ili9341_spi_tft_release(lcd_);
}

/** @brief Helper function used for the FillArc function
 * @param [in] cx
 * @param [in] cy
 * @param [in] oradius
 * @param [in] iradius
 * @param [in] start
 * @param [in] end
 * @param [in] color
 */
void ILI9341Display::writeFillArcHelper(int16_t cx, uint16_t cy, uint16_t oradius, uint16_t iradius, float start, float end, uint16_t color)
{
	const float DEGTORAD = 0.017453292519943295769236907684886F;
  if ((start == 90.0) || (start == 180.0) || (start == 270.0) || (start == 360.0))
  {
    start -= 0.1;
  }

  if ((end == 90.0) || (end == 180.0) || (end == 270.0) || (end == 360.0))
  {
    end -= 0.1;
  }

  float s_cos = (cos(start * DEGTORAD));
  float e_cos = (cos(end * DEGTORAD));
  float sslope = s_cos / (sin(start * DEGTORAD));
  float eslope = e_cos / (sin(end * DEGTORAD));
  float swidth = 0.5 / s_cos;
  float ewidth = -0.5 / e_cos;
  --iradius;
  int32_t ir2 = iradius * iradius + iradius;
  int32_t or2 = oradius * oradius + oradius;

  uint16_t start180 = !(start < 180.0);
  uint16_t end180 = end < 180.0;
  uint16_t reversed = start + 180.0 < end || (end < start && start < end + 180.0);

  int32_t xs = -oradius;
  int32_t y = -oradius;
  int32_t ye = oradius;
  int32_t xe = oradius + 1;
  if (!reversed)
  {
    if ((end >= 270 || end < 90) && (start >= 270 || start < 90))
    {
      xs = 0;
    }
    else if (end < 270 && end >= 90 && start < 270 && start >= 90)
    {
      xe = 1;
    }
    if (end >= 180 && start >= 180)
    {
      ye = 0;
    }
    else if (end < 180 && start < 180)
    {
      y = 0;
    }
  }
  do
  {
    int32_t y2 = y * y;
    int32_t x = xs;
    if (x < 0)
    {
      while (x * x + y2 >= or2)
      {
        ++x;
      }
      if (xe != 1)
      {
        xe = 1 - x;
      }
    }
    float ysslope = (y + swidth) * sslope;
    float yeslope = (y + ewidth) * eslope;
    int32_t len = 0;
    do
    {
      uint16_t flg1 = start180 != (x <= ysslope);
      uint16_t flg2 = end180 != (x <= yeslope);
      int32_t distance = x * x + y2;
      if (distance >= ir2 && ((flg1 && flg2) || (reversed && (flg1 || flg2))) && x != xe && distance < or2)
      {
        ++len;
      }
      else
      {
        if (len)
        {
          //writeFastHLine(cx + x - len, cy + y, len, color);
          ili9341_draw_line(lcd_, color, cx + x - len, cy + y, cx + x, cy + y);
          len = 0;
        }
        if (distance >= or2)
          break;
        if (x < 0 && distance < ir2)
        {
          x = -x;
        }
      }
    } while (++x <= xe);
  } while (++y <= ye);
}

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
void ILI9341Display::fillArc( uint16_t x, uint16_t y, uint16_t r1, uint16_t r2, float start, float end, Color color)
{
	const float FLT_EPSILON = 0.00001;
  if (r1 < r2)
  {
    //_swap_int16_t(r1, r2);
	  int16_t t = r1;
	  r1 = r2;
	  r2 = t;
  }
  if (r1 < 1)
  {
    r1 = 1;
  }
  if (r2 < 1)
  {
    r2 = 1;
  }
  int equal = (int)( fabs(start - end) < FLT_EPSILON );
  start = fmodf(start, 360);
  end = fmodf(end, 360);
  if (start < 0)
    start += 360.0;
  if (end < 0)
    end += 360.0;
  if (!equal && (fabsf(start - end) <= 0.0001))
  {
    start = .0;
    end = 360.0;
  }

  //startWrite();
  writeFillArcHelper( x, y, r1, r2, start, end, ELE3312::colorToILI9341Color(color));
  //endWrite();
}


/** @brief Returns the width of the screen in pixels
 * @retval The width of the screen in pixels;
 */
inline uint16_t ILI9341Display::getScreenWidth() const  {
	return 320;
}

/** @brief Returns the height of the screen in pixels.
 * @retval The height of the screen in pixels.
 */
inline uint16_t ILI9341Display::getScreenHeight() const  {
	return 240;
}

/** @brief Returns the current background color.
 * @retval The current background color.
 */
void ILI9341Display::setBackgroundColor(Color color)  {
	backgroundColor = color;
}

/** @brief Sets the background color.
 * @details The background color is used by functions that need to
 * erase graphical objects from the screen while maintaining a uniform
 * appearance of the displayed graphics.
 * @param color The new background color.
 */
inline Color ILI9341Display::getBackgroundColor() const  {
	return backgroundColor;
}

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
void ILI9341Display::readMemory(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t data[]){
	int nX = x1-x0;
	int nY = y1-y0;
	int size = nX*nY;
	if (size == 0 && nX > 0){
		size = nX;
	} else if (size == 0 && nY > 0){
		size = nY;
	}
	ili9341_read_memory(lcd_, x0, y0, x1, y1, data, size*3);
}
/**
 * @}
 */ // End group ELE3312
