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
 * @file OH_DCS-BIOS_APU_Control_Board.ino
 * @author Sandra Carroll
 * @date November 11,2022
 * @brief APU Control Board
 *
 * DCS-BIOS APU Control Board
 *  Supports:
 *      APU Magnetic Switch (Active High).
 *      Engine Crank L/R Magnetic Switch (active High).
 *      APU Ready Light
 *      Backlight Neopixels (Set to GREEN).
 *      Instrument Panel Dimming (Scaled Fram the 65535 to 0-255 for NeoPixel).
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
//#include <SwitecX12.h>
//#include <SwitecX25.h>

#define CONSOLE_TEST

/*
* DEFINE ALL I/O PINS USED
*/
#define BACKLIGHT_PIN            2
#define APU_NEOPIXEL_PIN         11

/*
 * DEFINE PINS AND STATES USED FOR ENGINE CRANK
 */
#define ENGINE_CRANK_SW_L_PIN     7
#define ENGINE_CRANK_SW_R_PIN     8

#define ENGINE_CRANK_HOLD_R_PIN   4
#define ENGINE_CRANK_HOLD_L_PIN   5

#define ENGINE_CRANK_STATE_L      0
#define ENGINE_CRANK_STATE_OFF    1
#define ENGINE_CRANK_STATE_R      2

/*
*   DEFINE PINS USED FOR INSTRUMENT PANEL DIMMER
*/
#ifdef CONSOLE_TEST 
  #define CONSOLES_DIMMER_PIN       A0
  #define INST_PNL_DIMMER_PIN       A1
#endif

/*
 * DEFINE PINS AND STATES USED FOR APU CONTROL
 */
#define APU_CONTROL_SW_PIN        12
#define APU_READY_LT_PIN          13
#define APU_HOLD_PIN              3

#define APU_CONTROL_STATE_ON      1
#define APU_CONTROL_STATE_OFF     0

// How many NeoPixels are attached to the Arduino?
#define LIGHTS_STRIP_NUMPIXELS    21
#define APU_STRIP_NUMPIXELS       12

// define start indexes for neopixels for strip1
#define MASTER_CAUTION            0
#define CONSOLE_BACKLIGHT         3
#define INSTRUMENT_BACKLIGHT      6
#define FLOOD_BACKLIGHT           9
#define WARN_BACKLIGHT            12
#define CHART_BACKLIGHT           15

// define start indexes for neopixels for strip2
#define APU_READY_LIGHT           0
#define APU_HOLD_LT               3
#define ENG_HOLD_LEFT             6
#define ENG_HOLD_RIGHT            9

Adafruit_NeoPixel backlight(LIGHTS_STRIP_NUMPIXELS, BACKLIGHT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel apu_strip(APU_STRIP_NUMPIXELS, APU_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


/**
* onApuControlSwChange() Callback function from APU control SW.
* 
* When DCS Set the value of the APU switch to on/off we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate.
*
* @param Value 0..1
* @returns NULL Return.
*/
void onApuControlSwChange(unsigned int Value) {
  switch (Value) {
    case APU_CONTROL_STATE_OFF:
      digitalWrite(APU_HOLD_PIN,LOW);    // IF WE GET A APU_OFF SIGNAL TURN OFF THE MAGNET TO RELEASE THE SWITCH
      apu_strip.fill(apu_strip.Color(0, 0, 0), APU_HOLD_LT, 3);
      break;
    case APU_CONTROL_STATE_ON:
      digitalWrite(APU_HOLD_PIN,HIGH);   // IF WE GET A APU_ON SIGNAL TURN ON THE MAGNET TO HOLD THE SWTICH
      apu_strip.fill(apu_strip.Color(0, 0, 255), APU_HOLD_LT, 3);
      break;
    default:
      break;
  }
  apu_strip.show();
}

/**
* onApuReadyLtChange() Callback function from APU Ready Light.
* 
* When DCS Set the value of the APU Ready Light on/off we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate
* in onApuControlSwChange
*
* @param newValue 0..1
* @returns NULL Return.
*/
void onApuReadyLtChange(unsigned int newValue) {
  switch(newValue) {
  case 0:
    apu_strip.fill(apu_strip.Color(0, 0, 0),APU_READY_LIGHT , 3);
    break;
  case 1:
    apu_strip.fill(apu_strip.Color(0, 255, 0),APU_READY_LIGHT , 3);
    break;
  default:
    break;
  }
apu_strip.show();
}

/**
* onEngineCrankSwChange() Callback function from Engine Crank Control SW.
* 
* When DCS Set the value of the Engine Crank switch to Left/Off/Right we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate.
*
* @param Value 0..2
* @returns NULL Return.
*/
void onEngineCrankSwChange(unsigned int Value) {
  switch (Value) {
    case ENGINE_CRANK_STATE_L:     // IF WE GET A ENGINE_CRANK_SW_L SIGNAL TURN ON THE MAGNET TO HOLD THE SWITCH
      digitalWrite(ENGINE_CRANK_HOLD_L_PIN,HIGH);
      apu_strip.fill(apu_strip.Color(128, 0, 0), ENG_HOLD_LEFT, 3);
      break;
    case ENGINE_CRANK_STATE_R:    // IF WE GET A ENGINE_CRANK_SW_R SIGNAL TURN ON THE MAGNET TO HOLD THE SWITCH
      digitalWrite(ENGINE_CRANK_HOLD_R_PIN,HIGH);
      apu_strip.fill(apu_strip.Color(255, 0, 0), ENG_HOLD_RIGHT, 3);
      break;
    case ENGINE_CRANK_STATE_OFF:
      digitalWrite(ENGINE_CRANK_HOLD_L_PIN,LOW); // IF WE GET A ENGINE_CRANK_OFF SIGNAL TURN OFF BOTH MAGNETS TO RELEASE THE SWITCH
      digitalWrite(ENGINE_CRANK_HOLD_R_PIN,LOW); 
      apu_strip.fill(apu_strip.Color(0, 0, 0), ENG_HOLD_LEFT, 3);
      apu_strip.fill(apu_strip.Color(0, 0, 0), ENG_HOLD_RIGHT, 3);
      break;
    default:
      break;
  }
apu_strip.show();
}

#ifdef CONSOLE_TEST 
  void onMasterCautionLtChange(unsigned int newValue) {
    switch(newValue) {
    case 0:
      backlight.fill(apu_strip.Color(0, 0, 0),MASTER_CAUTION , 3);
      break;
    case 1:
      backlight.fill(apu_strip.Color(255, 0, 0),MASTER_CAUTION , 3);
      break;
    default:
      break;
    }
  backlight.show();
  }

/**
* onConsoleIntLtChange() Callback function from Console Panel Brightness Knob
* 
* When the Console Panel Brightness Knob is adjusted we need to scale the value to
* the range of the NeoPixel (0..255) from the (0..65535) passed into Level
*
* @param Level 0..65535
* @returns NULL Return.
*/
  void onConsoleIntLtChange(unsigned int newValue) {
      backlight.fill(backlight.Color(0, map(newValue, 0, 65535, 0, 255), 0),CONSOLE_BACKLIGHT , 3);
      backlight.show();   
  }

  void onInstrIntLtChange(unsigned int newValue) {
      backlight.fill(backlight.Color(map(newValue, 0, 65535, 0, 255), 0, map(newValue, 0, 65535, 0, 255)),INSTRUMENT_BACKLIGHT , 3);
      backlight.show(); 
  }

  void onFloodIntLtChange(unsigned int newValue) {
      backlight.fill(backlight.Color(0, map(newValue, 0, 65535, 0, 255), 0),FLOOD_BACKLIGHT , 3);
      backlight.show(); 
  }

  void onWarnCautionDimmerChange(unsigned int newValue) {
      backlight.fill(backlight.Color(map(newValue, 0, 65535, 0, 255), 128, 0),WARN_BACKLIGHT, 3);
      backlight.show(); 
  }

  void onChartIntLtChange(unsigned int Value) {
      backlight.fill(backlight.Color(0, map(Value, 0, 65535, 0, 255), 0),CHART_BACKLIGHT , 3);
      backlight.show(); 
  }

#endif

/*
* DcsBios Calls and Callbacks
*/
// Switched
DcsBios::Switch2Pos apuControlSw("APU_CONTROL_SW", APU_CONTROL_SW_PIN);
DcsBios::Switch3Pos engineCrankSw("ENGINE_CRANK_SW", ENGINE_CRANK_SW_L_PIN, ENGINE_CRANK_SW_R_PIN);
DcsBios::LED apuReadyLt(0x74c2, 0x0800, APU_READY_LT_PIN);
// APU LIGHTS
DcsBios::IntegerBuffer apuReadyLtBuffer(0x74c2, 0x0800, 11, onApuReadyLtChange);
DcsBios::IntegerBuffer apuControlSwBuffer(0x74c2, 0x0100, 8, onApuControlSwChange);
DcsBios::IntegerBuffer engineCrankSwBuffer(0x74c2, 0x0600, 9, onEngineCrankSwChange);

#ifdef CONSOLE_TEST 
// INT LIGHTS
DcsBios::IntegerBuffer consoleIntLtBuffer(0x7558, 0xffff, 0, onConsoleIntLtChange);
DcsBios::IntegerBuffer chartIntLtBuffer(0x755e, 0xffff, 0, onChartIntLtChange);
DcsBios::IntegerBuffer instrIntLtBuffer(0x7560, 0xffff, 0, onInstrIntLtChange);
DcsBios::IntegerBuffer floodIntLtBuffer(0x755a, 0xffff, 0, onFloodIntLtChange);
DcsBios::IntegerBuffer warnCautionDimmerBuffer(0x754c, 0xffff, 0, onWarnCautionDimmerChange);

DcsBios::Potentiometer consolesDimmer("CONSOLES_DIMMER", CONSOLES_DIMMER_PIN);
DcsBios::Potentiometer instPnlDimmer("INST_PNL_DIMMER", INST_PNL_DIMMER_PIN);
// MASTER CAUTION
DcsBios::IntegerBuffer masterCautionLtBuffer(0x7408, 0x0200, 9, onMasterCautionLtChange);
#endif 
/*
* For testing purposes only, 
*/
#ifdef CONSOLE_TEST 

#endif

/**
* Arduino Setup Function
* 
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() {
  #ifdef DEBUG
    debug_init();
  #endif
  pinMode(APU_HOLD_PIN, OUTPUT); 
  pinMode(ENGINE_CRANK_HOLD_L_PIN, OUTPUT);
  pinMode(ENGINE_CRANK_HOLD_R_PIN, OUTPUT);  
  backlight.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  apu_strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  //clear neopixels
  backlight.fill(0,0,21);
  apu_strip.fill(0,0,12);
  backlight.show();
  apu_strip.show();
  DcsBios::setup();
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