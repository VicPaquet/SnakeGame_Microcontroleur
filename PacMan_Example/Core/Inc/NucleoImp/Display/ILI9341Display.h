/**
 * @file ILI9341Display.h
 * @date June 2025
 * @brief The class declared in this file implements the generic Graphics interface 
 * for the ILI9341 chip used on the Adafruit TFT board.

 * @defgroup ELE3312
 * @{
 */

#ifndef INC_ILI9341GRAPHICS_H_
#define INC_ILI9341GRAPHICS_H_


#include "Interfaces/Display/Display.h"
#include "NucleoImp/Display/ili9341.h"
#include "NucleoImp/Display/ili9341_gfx.h"

#include "main.h"

namespace ELE3312 {

	/** @brief Transforms a Color enum to an ili9341_color_t enum.
	 * @details The function is used to transform a generic color representation
	 * in the device specific color format of the ILI9341 LCD chip.
	 * @param [in] color A Color enum value.
	 * @retval A ILI9341 color enum value.
	 */
	constexpr ili9341_color_t colorToILI9341Color(Color color){
		switch (color){
			case Color::BLACK:
				return ILI9341_BLACK ;
			case Color::NAVY:
				return ILI9341_NAVY;
			case Color::DARKGREEN:
				return ILI9341_DARKGREEN;
			case Color::DARKCYAN:
				return ILI9341_DARKCYAN;
			case Color::MAROON:
				return ILI9341_MAROON;
			case Color::PURPLE:
				return ILI9341_PURPLE;
			case Color::OLIVE:
				return ILI9341_OLIVE;
			case Color::LIGHTGREY:
				return ILI9341_LIGHTGREY;
			case Color::DARKGREY:
				return ILI9341_DARKGREY;
			case Color::BLUE:
				return ILI9341_BLUE;
			case Color::GREEN:
				return ILI9341_GREEN;
			case Color::CYAN:
				return ILI9341_CYAN;
			case Color::RED:
				return ILI9341_RED;
			case Color::MAGENTA:
				return ILI9341_MAGENTA;
			case Color::YELLOW:
				return ILI9341_YELLOW;
			case Color::WHITE:
				return ILI9341_WHITE;
			case Color::ORANGE:
				return ILI9341_ORANGE;
			case Color::GREENYELLOW:
				return ILI9341_GREENYELLOW;
			case Color::PINK:
				return ILI9341_PINK;
		}
		// Default return black
		return ILI9341_BLACK ;
	}

	/** @brief This class implements the generic Graphics interface for the 
	 * ILI9341 chip used for the Adafruit TFT.
	 */
	class ILI9341Display : public Display {
		public:
			/** @brief Default constructor
			  */
			ILI9341Display();
			void operator=(ILI9341Display &obj) =  delete;
			ILI9341Display(ILI9341Display &obj) = delete;
			virtual ~ILI9341Display();
			void  setup(SPI_HandleTypeDef *hspi);

			void clearScreen() override;

			void fillScreen(Color color) override;

			void drawPixel(Color color, uint16_t x, uint16_t y) override;

			void drawLine(Color color,
					uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) override;

			void drawRect(Color color,
					uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;

			void drawRect(Color color, const Rect &rect) override;

			void fillRect(Color color,
					uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;


			void fillRect(Color color, const Rect &rect) override;

			void drawCircle(Color color,
					uint16_t x, uint16_t y, uint16_t r) override;

			void fillCircle(Color color,
					uint16_t x, uint16_t y, uint16_t r) override;

			void drawChar(uint16_t x, uint16_t y, char ch) override;

			void drawString(uint16_t x, uint16_t y, const std::string text, Color color= Color::WHITE) override;

			void fillArc( uint16_t x, uint16_t y, uint16_t r1, uint16_t r2, float start, float end, Color color) override;

			void drawBitmap(uint16_t *data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;


			uint16_t getScreenWidth() const override;

			uint16_t getScreenHeight() const override;

			void setBackgroundColor(Color color) override;

			Color getBackgroundColor() const override ;

			void readMemory(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t data[]);
		private:
			ili9341_text_attr_t *ptext_attr_; //!< Holds a pointer to a text attribute used for formatting text output on the TFT
			
			void writeFillArcHelper(int16_t cx, uint16_t cy, uint16_t oradius, uint16_t iradius, float start, float end, uint16_t color);
		protected:
			ili9341_t *lcd_; //!< Holds a pointer to the ILI9341 driver handler
			Color backgroundColor = Color::BLACK; //!< Holds the background color 


	};
} // End of namespace ELE3312

/**
 * @}
 */ // End group ELE3312

#endif /* INC_ILI9341GRAPHICS_H_ */
