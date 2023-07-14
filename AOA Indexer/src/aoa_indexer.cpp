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
 * @file aoa_indexer.ino
 * @author Sandra Carroll
 * @date May 29,2023
 * @brief AOA Indexer
 *
 * DCS-BIOS AOA INDEXER
 *  Supports:
 *      Backlight Neopixels (Set to GREEN).
 * 
 * REQUIRES: 
 *  DcsBios
 *  Servo
 *  Adafruit_NeoPixel
 * 
 */

#if defined(__AVR_ATmega328P__) ||  defined(__AVR_ATmega2560__)
  #define DCSBIOS_IRQ_SERIAL
#else
  #define DCSBIOS_DEFAULT_SERIAL
#endif

#ifdef __AVR__
 #include <avr/power.h> 
#endif

#include <Arduino.h>
#include "DcsBios.h"            // smgvbest/DCS-BIOS FP-Fork@^0.3.8
#include "FastLED.h"            // fastled/FastLED@^3.5.0
//#include <Adafruit_NeoPixel.h>  // adafruit/Adafruit NeoPixel@^1.11.0
//#include <NeoPixelBus.h>        // makuna/NeoPixelBus@^2.7.5

#define VERSION       0
#define RELEASE       1
#define MODIFICATION  2
// #define SELF_TEST     // comment out to skip SELF TEST

/**
 * @brief TASK SCHEDULER IF NEEDED
 * 
 */
//#define _TASK_MICRO_RES
// #include <TaskScheduler.h>
// #define T1_INIT (211L)

#define NUM_LEDS        3
#define LED_TYPE   WS2812B
#define COLOR_ORDER   GRB
#define DATA_PIN        4
#define CLK_PIN         4
#define VOLTS           5
#define MAX_MA        100

/*
* DEFINE ALL I/O PINS USED
*/
#define AOA_BACKLIGHT_PIN         4

/**
 * @brief DEFINE START POSITION OF EACH SET OF BACKLIGHTS
 *
*/
#define AOA_BACKLIGHT             0

/**
 * @brief DEFINE NEOPIXEL LENGHT OF EACH SET OF BACKLIGHTS
 * 
 */
#define AOA_LEN             3
#define AOA_HIGH_START      2
#define AOA_NORMAL_START    1
#define AOA_LOW_START       0


/*
 * DEFINE PINS AND STATES USED FOR APU CONTROL
 */
#define BACKLIGHT_STRIP_NUMPIXELS AOA_LEN

CRGBArray<AOA_LEN> leds;

/**
 * @brief 
 * 
 * @param newValue 
 */
void onAoaIndexerHighChange(unsigned int newValue) {
  /* GREEN */
  switch(newValue) {
    case 1: // ON
      leds[AOA_HIGH_START] = CRGB(255,0,0);  // GRB
      FastLED.show();
      break; 
    case 0: // OFF
      leds[AOA_HIGH_START] = CRGB(0,0,0);
      FastLED.show();
      break; 
    default: //DO NOTHING
      break;
  }
}
DcsBios::IntegerBuffer aoaIndexerHighBuffer(0x7408, 0x0008, 3, onAoaIndexerHighChange);

/**
 * @brief 
 * 
 * @param newValue 
 */
void onAoaIndexerHighFChange(unsigned int newValue) {
    /* your code here */
}
DcsBios::IntegerBuffer aoaIndexerHighFBuffer(0x758c, 0xffff, 0, onAoaIndexerHighFChange);

/**
 * @brief 
 * 
 * @param newValue 
 */
void onAoaIndexerNormalChange(unsigned int newValue) {
    /* YELLOW */
  switch(newValue) {
    case 1: // ON
      leds[AOA_NORMAL_START] = CRGB(192,240,0); // GRB
      FastLED.show();
      break; 
    case 0: // OFF      
      leds[AOA_NORMAL_START] = CRGB(0,0,0);
      FastLED.show();
      break; 
    default: //DO NOTHING
      break;
  }
}
DcsBios::IntegerBuffer aoaIndexerNormalBuffer(0x7408, 0x0010, 4, onAoaIndexerNormalChange);

/**
 * @brief 
 * 
 * @param newValue 
 */
void onAoaIndexerNormalFChange(unsigned int newValue) {
    /* your code here */
}
DcsBios::IntegerBuffer aoaIndexerNormalFBuffer(0x758e, 0xffff, 0, onAoaIndexerNormalFChange);


/**
 * @brief 
 * 
 * @param newValue 
 */
void onAoaIndexerLowChange(unsigned int newValue) {
    /* RED*/
  switch(newValue) {
    case 1: // ON
      leds[AOA_LOW_START] = CRGB(0,255,0); // GRB
      FastLED.show();
      break; 
    case 0: // OFF
      leds[AOA_LOW_START] = CRGB(0,0,0);
      FastLED.show();
      break; 
    default: //DO NOTHING
      break;
  }
}
DcsBios::IntegerBuffer aoaIndexerLowBuffer(0x7408, 0x0020, 5, onAoaIndexerLowChange);

/** 
 * TODO
 * ADD BRIGHTNESS CONTOL BASED on AOA
*/
void onHudAoaIndexerChange(unsigned int newValue) {
  /* We need to scale newValue from its range of 0-65535 to that of the pixel which is 0-255 */
  FastLED.setBrightness(map(newValue,0,65535,0,255));
  FastLED.show();
}
DcsBios::IntegerBuffer hudAoaIndexerBuffer(0x745e, 0xffff, 0, onHudAoaIndexerChange);

// DcsBios::Potentiometer hudAoaIndexer("HUD_AOA_INDEXER", 3);

/**
 * @brief 
 * 
 */
void AoA_Test() {
  // clear all pixels
  FastLED.clear();
  FastLED.show();
  // turn on hi
  leds[AOA_HIGH_START] = CRGB(255,0,0);  // GRB
  FastLED.show();
  delay(250);
  // turn on low
  leds[AOA_LOW_START] = CRGB(0,255,0); // GRBRB
  FastLED.show();
  delay(250);
  // turn on normal
  leds[AOA_NORMAL_START] = CRGB(192,240,0); // GRB
  FastLED.show();
  delay(250);
  // turn off normal
  leds[AOA_NORMAL_START] = CRGB(0,0,0); // GRB
  FastLED.show();
  delay(250);
  // turn off low
  leds[AOA_LOW_START] = CRGB(0,0,0); // GRB
  FastLED.show();
  delay(250);
  // turn off hi
  leds[AOA_HIGH_START] = CRGB(0,0,0);  // GRB
  FastLED.show();
  delay(250);
  // turn on all
  leds[AOA_NORMAL_START] = CRGB(192,240,0); // GRB
  leds[AOA_LOW_START] = CRGB(0,255,0); // GRB
  leds[AOA_HIGH_START] = CRGB(255,0,0);  // GRB
  FastLED.show();
  delay(250);
  // turn off all
  leds.fadeLightBy(255);
  delay(250);
  FastLED.clear();
  FastLED.show();
  delay(250);
}

/**
* Arduino Setup Function
* 
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() {
  Serial.begin(250000);
  //while (!Serial); // wait for serial attach

  Serial.printf("KB10A1A2-1 AoA Indexer for OpenHornet V%dR%dM%d\n",VERSION,RELEASE,MODIFICATION);
  Serial.flush();

  digitalWrite(LED_BUILTIN,false);  // TURN OFF BUILTIN LED TO SHOW WE ENTERED SETUP

  // INITIALIZE NeoPixel strip object (REQUIRED)
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  
  #ifdef SELF_TEST
    Serial.printf("AoA_Test() Started\n");
    for (int i=0;i<5;i++) {
      AoA_Test();
    }
    Serial.printf("AoA_Test() Ended\n");
  #endif

  FastLED.clear();
  FastLED.show();

  // Setup DCS-BIOS
  DcsBios::setup();

  // Indicate Setup is Completed
  digitalWrite(LED_BUILTIN,true); //TURN ON BUILTIN LED TO SHOW WE FINISHED SETUP
  Serial.printf("DcsBios::setup() Completed\n");
  Serial.printf("Entering DcsBios::loop()\n");
  Serial.flush();
}

/**
* Arduino Loop Function
* 
* Arduino standard Loop Function. Code who should be executed
* over and over in a loop, belongs in this function.
*/
void loop() {
    DcsBios::loop();
} 
