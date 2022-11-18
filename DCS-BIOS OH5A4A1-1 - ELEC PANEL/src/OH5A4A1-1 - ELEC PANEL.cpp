/**************************************************************************************
 *        ____                   _    _                       _
 *       / __ \                 | |  | |                     | |
 *      | |  | |_ __   ___ _ __ | |__| | ___  _ __ _ __   ___| |_
 *      | |  | | '_ \ / _ \ '_ \|  __  |/ _ \| '__| '_ \ / _ \ __|
 *      | |__| | |_) |  __/ | | | |  | | (_) | |  | | | |  __/ |_
 *       \____/| .__/ \___|_| |_|_|  |_|\___/|_|  |_| |_|\___|\__|
 *             | |
 *             |_|
 *
 *
 *   Project OpenHornet
 *
 *   This Project is released under the Creative Commons 
 *   Atribution - Non Comercal - Share Alike License.
 *   
 *   CC BY-NC-SA 3.0
 *   
 *   You are free to:
 *   - Share — copy and redistribute the material in any medium or format
 *   - Adapt — remix, transform, and build upon the material
 *   The licensor cannot revoke these freedoms as long as you follow the license terms.
 *   
 *   Under the following terms:
 *   - Attribution — You must give appropriate credit, provide a link to the license, 
 *     and indicate if changes were made. You may do so in any reasonable manner, 
 *     but not in any way that suggests the licensor endorses you or your use.
 *   - NonCommercial — You may not use the material for commercial purposes.
 *   - ShareAlike — If you remix, transform, or build upon the material, 
 *     you must distribute your contributions under the same license as the original.
 *     
 *   No additional restrictions — You may not apply legal terms or technological 
 *   measures that legally restrict others from doing anything the license permits.
 *   
 *   More Information about the license can be found under:
 *   https://creativecommons.org/licenses/by-nc-sa/3.0/
 *   
 *   
 *   This Project uses Doxygen as a documentation generator.
 *   Please use Doxigen capable comments.
 *   
 **************************************************************************************/

/**
 * @file OH5A4A1-1 - ELEC PANEL.cpp
 * @author Sandra Carroll
 * @date November 16,2022
 * @brief OH5A4A1-1 - ASSY, ELEC PANEL
 *
 * DCS-BIOS OH5A4A1-1 Board
 */

#if defined(__AVR_ATmega328P__) ||  defined(__AVR_ATmega2560__)
  #define DCSBIOS_IRQ_SERIAL
#else
  #define DCSBIOS_DEFAULT_SERIAL
#endif

#ifdef __AVR__
 #include <avr/power.h> 
#endif

#include "DcsBios.h"
#include <Adafruit_NeoPixel.h>

#ifdef DEBUG
  #include "avr8-stub.h"
#endif
//#include <SwitecX12.h>
//#include <SwitecX25.h>

/*
* DEFINE ALL I/O PINS USED
*/
#define L_GEN_SW_PIN        5
#define BATTERY_SW_PIN_A    2
#define BATTERY_SW_PIN_B    4
#define R_GEN_SW_PIN        3
#define VOLTE_SERVO_PIN     8
#define VOLTU_SERVO_PIN     11
/*
* Define NeoPixel PIN and Count
*/
#define BACKLIGHT_PIN       12
#define BACKLIGHT_PIXELS    21

Adafruit_NeoPixel backlight( BACKLIGHT_PIXELS, BACKLIGHT_PIN, NEO_GRB + NEO_KHZ800);

/**
* @brief onConsoleIntLtChange() Callback function for when the console brightness changes
* 
* When the Console Panel Brightness Knob is adjusted we need to scale the value to
* the range of the NeoPixel (0..255) from the (0..65535) passed into Level.
* NeoPixel is set to Green
*
* @param Level 0..65535
* @returns NULL Return.
*/
void onConsoleIntLtChange(unsigned int newValue) {
      backlight.fill(backlight.Color(0, map(newValue, 0, 65535, 0, 255), 0),0 , BACKLIGHT_PIXELS);
      backlight.show();   
}

/**
 * @brief Setup DcsBios Calls 
 * 
 */
DcsBios::Switch3Pos batterySw("BATTERY_SW", BATTERY_SW_PIN_A, BATTERY_SW_PIN_B);
DcsBios::Switch2Pos lGenSw("L_GEN_SW", L_GEN_SW_PIN);
DcsBios::Switch2Pos rGenSw("R_GEN_SW", R_GEN_SW_PIN);
DcsBios::IntegerBuffer consoleIntLtBuffer(0x7558, 0xffff, 0, onConsoleIntLtChange);
DcsBios::ServoOutput voltE (0x753e,VOLTE_SERVO_PIN, 544, 2400, [](unsigned int newValue) -> unsigned int {
return map(newValue, 418, 41309, 1973, 1450);
});
DcsBios::ServoOutput voltU (0x753c, VOLTU_SERVO_PIN, 544, 2400, [](unsigned int newValue) -> unsigned int {
return map(newValue, 418, 41309, 1111,  1662);
});


/**
* @brief Arduino Setup Function
* 
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() { 
  #ifdef DEBUG
    debug_init();
  #endif
  backlight.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  //clear neopixels
  backlight.fill(0,0,21);
  backlight.show();
  DcsBios::setup();
}

/**
* @brief Arduino Loop Function
* 
* Arduino standard Loop Function. Code who should be executed
* over and over in a loop, belongs in this function.
*/
void loop() {
  DcsBios::loop(); 
}