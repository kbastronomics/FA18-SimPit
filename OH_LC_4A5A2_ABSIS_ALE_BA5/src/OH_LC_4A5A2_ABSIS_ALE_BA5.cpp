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
 * @file OH_LC_4A5A2_ABSIS_ALE_BA5.ino
 * @author Sandra Carroll
 * @date January 15,2023
 * @brief APU Control Board + Left Circuit Breakers
 *
 * DCS-BIOS APU Control Board
 *  Supports:
 *      APU Magnetic Switch (Active High).
 *      Engine Crank L/R Magnetic Switch (active High).
 *      APU Ready Light
 *      Left Circuit Breakers
 *      USB HID Buttons via Joystick
 */

#if defined(__AVR_ATmega328P__) ||  defined(__AVR_ATmega2560__)
  #define DCSBIOS_IRQ_SERIAL
#else
  #define DCSBIOS_DEFAULT_SERIAL
#endif

#ifdef __AVR__
 #include <avr/power.h> 
#endif

/**
 * @brief following #define tells DCS-BIOS that this is a RS-485 slave device.
 * It also sets the address of this slave device. The slave address should be
 * between 1 and 126 and must be unique among all devices on the same bus.
 *
 * @bug Currently does not work with the Pro Micro (32U4), Fails to compile
*/

//#define DCSBIOS_RS485_SLAVE 5

/**
 * The Arduino pin that is connected to the
 * RE and DE pins on the RS-485 transceiver.
*/
#define TXENABLE_PIN        5
#define UART1_SELECT


#include "DcsBios.h"
#include "Joystick.h"

Joystick_ JoystickBtn;

/*
 * DEFINE PINS AND STATES USED FOR ENGINE CRANK
 */
#define CRANK_SW1                 14    // Right Engine Crank
#define CRANK_SW2                 7     // Left Engine Crank
#define ENG_CRNK_CH2              3     // Magnet

#define ENGINE_CRANK_STATE_L      0
#define ENGINE_CRANK_STATE_OFF    1
#define ENGINE_CRANK_STATE_R      2


/*
 * DEFINE PINS AND STATES USED FOR APU CONTROL
 */
#define APU_SW1                   15    // APU On/Off
#define APU_JSW1                  1     // APU On/Off Joystick Button

#define APU_LAMP                  6     // APU Ready
#define APU_MAG_CH1               2     // Magnet

#define APU_CONTROL_STATE_ON      1
#define APU_CONTROL_STATE_OFF     0

/*
* DEFINE PINS USESD FOR LEFT CIRCUIT BREAKER PANEL
*/
#define FCS_CH1                   16
#define FCS_CH2                   8
#define LCSPDBRK                  10
#define LCLBAR                    9



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
      digitalWrite(APU_MAG_CH1,LOW);    // IF WE GET A APU_OFF SIGNAL TURN OFF THE MAGNET TO RELEASE THE SWITCH
      JoystickBtn.setButton(APU_JSW1, LOW);
      break;
    case APU_CONTROL_STATE_ON:
      digitalWrite(APU_MAG_CH1,HIGH);   // IF WE GET A APU_ON SIGNAL TURN ON THE MAGNET TO HOLD THE SWTICH
      JoystickBtn.setButton(APU_JSW1, HIGH);
      break;
    default:
      break;
  }
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
    //apu_strip.fill(apu_strip.Color(0, 0, 0),APU_READY_LIGHT , 3);
    break;
  case 1:
    //apu_strip.fill(apu_strip.Color(0, 255, 0),APU_READY_LIGHT , 3);
    break;
  default:
    break;
  }
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
      digitalWrite(ENG_CRNK_CH2,HIGH);
      break;
    case ENGINE_CRANK_STATE_R:    // IF WE GET A ENGINE_CRANK_SW_R SIGNAL TURN ON THE MAGNET TO HOLD THE SWITCH
      digitalWrite(ENG_CRNK_CH2,HIGH);
      break;
    case ENGINE_CRANK_STATE_OFF:
      digitalWrite(ENG_CRNK_CH2,LOW); // IF WE GET A ENGINE_CRANK_OFF SIGNAL TURN OFF BOTH MAGNETS TO RELEASE THE SWITCH 
      break;
    default:
      break;
  }
}

/**
* onCbFcsChan1Change() Callback function from APU control SW.
* 
* When DCS Set the value of the APU switch to on/off we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate.
*
* @param Value 0..1
* @returns NULL Return.
*/
void onCbFcsChan1Change(unsigned int newValue) {
    /* your code here */
}

/**
* onCbFcsChan2Change() Callback function from APU control SW.
* 
* When DCS Set the value of the APU switch to on/off we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate.
*
* @param Value 0..1
* @returns NULL Return.
*/
void onCbFcsChan2Change(unsigned int newValue) {
    /* your code here */
}

/**
* onCbLaunchBarChange() Callback function from APU control SW.
* 
* When DCS Set the value of the APU switch to on/off we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate.
*
* @param Value 0..1
* @returns NULL Return.
*/
void onCbLaunchBarChange(unsigned int newValue) {
    /* your code here */
}

/**
* onCbSpdBrkChange() Callback function from APU control SW.
* 
* When DCS Set the value of the APU switch to on/off we need to respond,
* by looking for a change in the switch and turning on/off magnet as appropiate.
*
* @param Value 0..1
* @returns NULL Return.
*/
void onCbSpdBrkChange(unsigned int newValue) {
    /* your code here */
}


/*
* DcsBios Calls and Callbacks
*/

// Switches and Direct Lights
DcsBios::Switch2Pos apuControlSw("APU_CONTROL_SW", APU_SW1);
DcsBios::LED apuReadyLt(0x74c2, 0x0800, APU_LAMP);
DcsBios::Switch3Pos engineCrankSw("ENGINE_CRANK_SW", CRANK_SW1, CRANK_SW2);
DcsBios::Switch2Pos cbFcsChan1("CB_FCS_CHAN1", FCS_CH1);
DcsBios::Switch2Pos cbFcsChan2("CB_FCS_CHAN2", FCS_CH2);
DcsBios::Switch2Pos cbLaunchBar("CB_LAUNCH_BAR", LCLBAR);
DcsBios::Switch2Pos cbSpdBrk("CB_SPD_BRK", LCSPDBRK);

// Auxiliary Power Unit Panel OnChange Events
DcsBios::IntegerBuffer apuReadyLtBuffer(0x74c2, 0x0800, 11, onApuReadyLtChange);
DcsBios::IntegerBuffer apuControlSwBuffer(0x74c2, 0x0100, 8, onApuControlSwChange);
DcsBios::IntegerBuffer engineCrankSwBuffer(0x74c2, 0x0600, 9, onEngineCrankSwChange);

// Left Essential Circuit Breakers onChange Events
DcsBios::IntegerBuffer cbFcsChan1Buffer(0x74c2, 0x8000, 15, onCbFcsChan1Change);
DcsBios::IntegerBuffer cbFcsChan2Buffer(0x74c4, 0x0100, 8, onCbFcsChan2Change);
DcsBios::IntegerBuffer cbLaunchBarBuffer(0x74c4, 0x0400, 10, onCbLaunchBarChange);
DcsBios::IntegerBuffer cbSpdBrkBuffer(0x74c4, 0x0200, 9, onCbSpdBrkChange);

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
  pinMode(APU_MAG_CH1, OUTPUT); 
  pinMode(ENG_CRNK_CH2, OUTPUT);  
  DcsBios::setup();
  JoystickBtn.begin();
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