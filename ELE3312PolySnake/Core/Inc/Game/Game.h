#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "main.h"

#include "NucleoImp/AnalogInput/ADCInput.h"
#include "NucleoImp/Display/ILI9341Display.h"
#include "NucleoImp/Distance/HCSR04Distance.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"
#include "NucleoImp/MotionInput/MPU6050MotionInput.h"
#include "NucleoImp/RGBLight/RGBLED.h"
#include "Game/Sections/SnakeGame.h"
#include "NucleoImp/SerialCom/UART.h"

#include "Game/Sections/MusicPlayer.h"
#include "NucleoImp/Sound/DACSound.h"
#include "NucleoImp/SerialCom/Ringbuffer.h"
#include "Game/Sections/Menu.h"
#include "cpp_main.h"
#include <memory>

#define BUFFER_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif

void main_run(peripheral_handles *handlers);

enum class GameState {
    Menu, Play, ResultScreen
};

/* @brief The game class represents the Snake game. It contains
 * all aspects of the game as well as its own event loop.
 */
class Game {
public:
    Game();
    void setup(peripheral_handles *handles);
    virtual ~Game() = default;
    void run();
    void handleUART(uint8_t data);
    void handleUART(uint8_t *data, uint16_t size);

    // --- UART global buffer accessible partout ---
    static Ringbuffer uartBuffer;
    static MusicPlayer musicPlayer;
    static uint8_t buff[];

private:
    peripheral_handles *handles = nullptr;

    // Peripherals
    static ADCInput adcInput;
    static MPU6050MotionInput motionInput;
    static ILI9341Display display;
    static HCSR04Distance distance;
    static GPIOKeypad keypad;
    static RGBLED rgbLed;
    static UART uart;

    static DACSound sound;

    static SnakeGame snakeGame;
    static Menu menu;

    // État du jeu
    GameState gameState = GameState::Menu;
};

#ifdef __cplusplus
}
#endif

#endif /* INC_GAME_H_ */
