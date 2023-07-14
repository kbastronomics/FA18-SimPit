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
 * @file OH_RC_5A4A1_ABSIS_ALE_BA3.cpp
 * @author Sandra Carroll
 * @date APRIL 16,2023
 * @brief OH4A3A3-1 - UPPER LEFT CONSOLE - BRAKE PRESSURE, BRAKE & FIRE TEST.cpp
 * DCS-BIOS OH4A3A3-1 Board.
 * 
 * 
 */


/**
 * @brief Test code for the Electric Panel to validate the PCB, Backlight, Switches and Stepper 
 * are working correctly
 * 
 * 
 */
#if defined(__AVR_ATmega328P__) ||  defined(__AVR_ATmega2560__)
  #define DCSBIOS_IRQ_SERIAL
#else
  #define DCSBIOS_DEFAULT_SERIAL
#endif

/**
 * @brief if this is a AVR type device tell it to also include the power.h library
 * 
 */
#ifdef __AVR__
 #include <avr/power.h> 
#endif

/**
 * @brief  Include libaries used in this program
 * 
 */
#include "DcsBios.h"
//#include "Servo.h"              // included by DcsBios.h,  noted here for reference only
#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>

/**
 * @brief If debugging is needed include the needed library
 * 
 */

#ifdef DEBUG
  #include "avr8-stub.h"
#endif

/**
 * @brief DEFINE ALL I/O PINS USED
 * 
 */
#define  STEPS  720    // steps per revolution (limited to 315°)
#define  BRKP_M1      A3
#define  BRKP_M2      2
#define  BRKP_M3      A2
#define  BRKP_M4      A1
#define  BRKP_DIN     3   // NeoPixel Data
#define  FIRE_SW1     4
#define  FIRE_SW2     A0
#define  BRKH_SECURE  15
#define  BRKH_EMERG   6
#define  BRKH_PARK    14
#define  INST_BACKLIGHT_START  0
#define  INST_BACKLIGHT_LEN    1

int HYD_BRK = 0;
int val = 0;

AccelStepper _brake_pressure(AccelStepper::FULL4WIRE, BRKP_M1, BRKP_M2, BRKP_M3, BRKP_M4);
Adafruit_NeoPixel inst_backlight(INST_BACKLIGHT_LEN,  BRKP_DIN, NEO_GRB + NEO_KHZ800);

/**
 * @brief DEFINE RUNNING PARMS
 * 
 */

uint8_t __is_started__  = false;
uint8_t __is_hornet__   = false;
uint8_t __is_onground__ = false;


/**
 * @brief Setup DcsBios Calls 
 * 
 */

/**
 * @brief ON STARTUP OF DCS CHECK AND SENT SWITCH STATE
 * 
 * @param newValue 
 */
void onAcftNameChange(char* newValue) {

// when we get the name change event the sim has started.
// read the current switch postiions and send to DCS

    //if (strcmp(newValue,"FA-18C_hornet")) {
    const char fire_sw1 = digitalRead(FIRE_SW1) + '0';
      tryToSendDcsBiosMessage("FIRE_SW1", &fire_sw1);
      const char fire_sw2 = digitalRead(FIRE_SW2) + '0';
      tryToSendDcsBiosMessage("FIRE_SW2", &fire_sw2);
      // const char r_gen = digitalRead(R_GEN_SW_PIN) + '0';
      // tryToSendDcsBiosMessage("L_GEN_SW", &r_gen);
      // const char l_gen = digitalRead(L_GEN_SW_PIN) + '0';
      // tryToSendDcsBiosMessage("L_GEN_SW", &l_gen);
    //}   
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);

/**
 * @brief 
 * 
 * @param newValue 
 */
void onDcsBiosChange(char* newValue) {
    /* your code here */
}
DcsBios::StringBuffer<6> dcsBiosBuffer(0x0400, onDcsBiosChange);


void onAltMslFtChange(unsigned int newValue) {
    /* your code here */
}
DcsBios::IntegerBuffer altMslFtBuffer(0x0434, 0xffff, 0, onAltMslFtChange);

void onHydIndBrakeChange(unsigned int newValue) {
static int pos=0;  
  HYD_BRK = map(newValue, 0, 65000, 0, 1000);
  val = HYD_BRK;   //get the potentiometer value (range 0-1023)
  val = map(HYD_BRK,0,100,0,150);    // map Steper Needle 0-4. 0=0 - 4=150
  
  if(abs(val - pos)> 2){         //if diference is greater than 2 steps.
      if((val - pos)> 0){
          _brake_pressure.move(-1);      // move one step to the left.
          pos++;
          }
      if((val - pos)< 0){
          _brake_pressure.move(1);       // move one step to the right.
          pos--;
          }
      }
  
  _brake_pressure.run();
}
DcsBios::IntegerBuffer hydIndBrakeBuffer(0x7506, 0xffff, 0, onHydIndBrakeChange);

DcsBios::Switch3Pos fireTestSw("FIRE_TEST_SW", FIRE_SW1, FIRE_SW2);

void onInstrIntLtChange(unsigned int newValue) {
    inst_backlight.fill(inst_backlight.Color(0, 255, 0),INST_BACKLIGHT_START , INST_BACKLIGHT_LEN);
    inst_backlight.setBrightness(map(newValue, 0, 65535, 0, 255));
    inst_backlight.show();     /* your code here */
}
DcsBios::IntegerBuffer instrIntLtBuffer(0x7560, 0xffff, 0, onInstrIntLtChange);

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

  _brake_pressure.setMaxSpeed(200.0);
  _brake_pressure.setAcceleration(100.0);

  _brake_pressure.moveTo(250);
  _brake_pressure.moveTo(-17);

  // turn off backlight 
  inst_backlight.fill(inst_backlight.Color(0, 0, 0),INST_BACKLIGHT_START , INST_BACKLIGHT_LEN);
  inst_backlight.show(); 

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