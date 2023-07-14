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
 *   The OpenHornet Project is a F/A-18C OFP 13C Lot 20 1:1 Replica Simulator,
 *   consisting of a physical structure and electrical/software interfaces to a PC
 *   to be driven by Digital Combat Simulator (DCS).
 *
 *   ---------------------------------------------------------------------------------
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
 *   ---------------------------------------------------------------------------------
 *
 *   The OpenHornet Software is based on DCS-BIOS
 *   You can find more information here: http://dcs-bios.a10c.de
 *
 *   DCS-BIOS is released under the following terms:
 *   https://github.com/dcs-bios/dcs-bios/blob/develop/DCS-BIOS-License.txt
 * 
 *   Use only the following DCS-BIOS Arduino Library
 *   https://github.com/DCSFlightpanels/dcs-bios-arduino-library
 *
 *   ---------------------------------------------------------------------------------
 *
 *   This Project uses Doxygen as a documentation generator.
 *   Please use Doxigen capable comments.
 *
 **************************************************************************************/

/**
 * @file OH_2A15A1_ABSIS_PMS.ino
 * @author <Sandra Carroll> @ <smgvbest@gmail.com>
 * @date <01.10.2023>
 * @version u.0.0.1 Alpha
 * @warning This sketch is based on a wiring diagram, and was not yet tested on hardware.
 *
 * @details 
 * 
 * 
 *  * **Intended Board:**
 * ABSIS Power Managment System (PMS)
 * 
 * **Wiring diagram:**
 * OH_2A12A5_SHORE_BULKHEAD -> OH_2A15A1_ABSIS_PMS 		-> OH_2A14A1_ABSIS_ATX_PSU_BREAKOUT
 * 							            OH_2A14A2_Generic ATX PSU	->
 * 
 * FUNCTION           | PIN      | DIRECTION 					
 * PSU_ON				      | D8	OUTPUT
 * PSU_OK				      | D9 	INPUT
 * SIMPWR_PCPWR 		  | A3	INPUT
 * SIMPWR_PCRESET 		| A2	INPUT
 * SIMPWR_MASTERON		| A1	INPUT
 * SIMPWR_DISPON		  | A0	INPUT
 * SIMPWR_PFLT_MODE	  | D15	INPUT
 * SIMPWR_MAINT_MODE	| D14	INPUT
 * EXT_ACC1			      | D4	OUTPUT
 * EXT_ACC2			      | D5	OUTPUT
 * EXT_ACC3			      | D6	OUTPUT
 * EXT_ACC4			      | D7	OUTPUT
 */
#include <Arduino.h>

/**
 * Check if we're on a Mega328 or Mega2560 and define the correct
 * serial interface
 * 
 */
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
#define DCSBIOS_IRQ_SERIAL
#else
#define DCSBIOS_DEFAULT_SERIAL
#endif

#ifdef __AVR__
 #include <avr/power.h> 
#endif

/**
 * @brief  IF DEFINED __USE_HID__ THE ALE BOARD WILL SEND JOYSTICK BUTTON EVENTS OVER USB
 * 
*/
#define __USE_HID__ 


/**
 * @brief DCS Bios library include
 */
#include "DcsBios.h"
#include "Joystick.h"
#include <Bounce2.h>


/**
 * @brief Define Control I/O  
 * 
 */
// INPUTS
#define PSU_OK				    9
#define SIMPWR_PCPWR 		  A3
#define SIMPWR_PCRESET 		A2
#define SIMPWR_MASTERON		A1
#define SIMPWR_DISPON		  A0
#define SIMPWR_PFLT_MODE	15
#define SIMPWR_MAINT_MODE	14
// OUTPUTS
#define PSU_ON				    8
#define EXT_ACC1			    4
#define EXT_ACC2			    5
#define EXT_ACC3			    6
#define EXT_ACC4			    7

#ifdef __USE_HID__
// Buttons
#define JOYBTN0           0
#define JOYBTN1           1
#define JOYBTN2           2
#define JOYBTN3           3
#define JOYBTN4           4
#define JOYBTN5           5
#define JOYBTN6           6
#define JOYBTN7           7


// HID Setup
#define JOYSTICK_BUTTON_COUNT     8
#define JOYSTICK_HATSWITCH_COUNT  0 
#define INCLUDE_X_AXIS            false
#define INCLUDE_Y_AXIS            false
#define INCLUDE_Z_AXIS            false
#define INCLUDE_RX_AXIS           false
#define INCLUDE_RY_AXIS           false
#define INCLUDE_RZ_AXIS           false
#define INCLUDE_RUDDER            false
#define INCLUDE_THROTTLE          false
#define INCLUDE_ACCELERATOR       false
#define INCLUDE_BRAKE             false
#define INCLUDE_STEERING          false

const bool initAutoSendState = true;

/**
 * @brief Our Joystick configuration
 * 
 */
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
                   JOYSTICK_TYPE_JOYSTICK, 
                   JOYSTICK_BUTTON_COUNT, 
                   JOYSTICK_HATSWITCH_COUNT, 
                   INCLUDE_X_AXIS, 
                   INCLUDE_Y_AXIS, 
                   INCLUDE_Z_AXIS, 
                   INCLUDE_RX_AXIS, 
                   INCLUDE_RY_AXIS, 
                   INCLUDE_RZ_AXIS, 
                   INCLUDE_RUDDER, 
                   INCLUDE_THROTTLE, 
                   INCLUDE_ACCELERATOR, 
                   INCLUDE_BRAKE, 
                   INCLUDE_STEERING);
#endif

/**
 * @brief Instantiate our button debouce control
 * 
 */
Bounce masteron = Bounce();
Bounce pcpwr = Bounce();
Bounce pcreset = Bounce();
Bounce dispon = Bounce();
Bounce pfltmode = Bounce();
Bounce maintmode = Bounce();

static bool power_state = false;

/**
 * @brief simpwr_mode subroute to 
 * 
 */
void simpwr_mode() {

  maintmode.update();
  pfltmode.update();

  if (( maintmode.read() == HIGH ) && ( pfltmode.read() == HIGH )) {
    #ifdef __USE_HID__
      Joystick.pressButton(JOYBTN6);
      Joystick.releaseButton(JOYBTN5);
      Joystick.releaseButton(JOYBTN4);
    #endif
  }

  if (( maintmode.read() == LOW ) && ( pfltmode.read() == HIGH )) {
    #ifdef __USE_HID__
      Joystick.pressButton(JOYBTN5);
      Joystick.releaseButton(JOYBTN4);
      Joystick.releaseButton(JOYBTN6);
    #endif
  }

  if (( maintmode.read() == HIGH ) && ( pfltmode.read() == LOW )) {
    #ifdef __USE_HID__
      Joystick.pressButton(JOYBTN4);
      Joystick.releaseButton(JOYBTN5);
      Joystick.releaseButton(JOYBTN6);
    #endif
  }
}

/**
 * @brief 
 * 
 */
void simpwr_maint_mode() {

  maintmode.update();

  if ( maintmode.changed() ) {
  int deboucedInput = maintmode.read();
    if ( (deboucedInput == LOW ) ) {
    #ifdef __USE_HID__
      Joystick.pressButton(JOYBTN5);
    #endif
    }

    if ( (deboucedInput == HIGH ) ) {
    #ifdef __USE_HID__
      Joystick.releaseButton(JOYBTN5);
    #endif
    }
  }
}

/**
 * @brief 
 * 
 */
void simpwr_pflt_mode() {

  pfltmode.update();

  if ( pfltmode.changed() ) {
  int deboucedInput = pfltmode.read();
    if ( (deboucedInput == LOW ) ) {
      #ifdef __USE_HID__
        Joystick.pressButton(JOYBTN4);
      #endif
    }

    if ( (deboucedInput == HIGH ) ) {
      #ifdef __USE_HID__
        Joystick.releaseButton(JOYBTN4);
      #endif
    }
  }
}

/**
 * @brief simpwr_dispon()
 * 
 */
void simpwr_dispon() {

  dispon.update();
  
  if ( dispon.changed() ) {
  int deboucedInput = dispon.read();
    if ( (deboucedInput == LOW ) ) {
      digitalWrite(EXT_ACC3, HIGH);
      #ifdef __USE_HID__
        Joystick.pressButton(JOYBTN3);
      #endif
    }

    if ( (deboucedInput == HIGH ) ) {
      digitalWrite(EXT_ACC3, LOW);
      #ifdef __USE_HID__
        Joystick.releaseButton(JOYBTN3);
      #endif
    }
  }
}

/**
 * @brief 
 * 
 */
void simpwr_masteron() {

  masteron.update();

  if ( masteron.changed() ) {
  int deboucedInput = masteron.read();
    if ( (deboucedInput == LOW ) ) {
      digitalWrite(PSU_ON, HIGH); 
      power_state = HIGH;
      #ifdef __USE_HID__  
        Joystick.pressButton(JOYBTN2);
      #endif
    }
    if ( (deboucedInput == HIGH) ) {
      digitalWrite(PSU_ON, LOW);
      power_state = LOW;
      #ifdef __USE_HID__  
        Joystick.releaseButton(JOYBTN2);
      #endif
    }
  }
}

/**
 * @brief 
 * 
 */
void simpwr_pcreset() {

  pcreset.update();

  if ( pcreset.changed() ) {
  int deboucedInput = pcreset.read();
    if ( (deboucedInput == LOW ) ) {
      digitalWrite(EXT_ACC2, HIGH);
      #ifdef __USE_HID__ 
        Joystick.pressButton(JOYBTN1);
      #endif
    }
    if ( (deboucedInput == HIGH) ) {
      digitalWrite(EXT_ACC2, LOW);
      #ifdef __USE_HID__
        Joystick.releaseButton(JOYBTN1);
      #endif
    }
  }
}

/**
 * @brief 
 * 
 */
void simpwr_pcpwr() {

  pcpwr.update();

  if ( pcpwr.changed() ) {
  int deboucedInput = pcpwr.read();
    if ( (deboucedInput == LOW ) ) {
      digitalWrite(EXT_ACC1, HIGH); 
      #ifdef __USE_HID__  
        Joystick.pressButton(JOYBTN0);
      #endif
    }
    if ( (deboucedInput == HIGH) ) {
      digitalWrite(EXT_ACC1, LOW);
      #ifdef __USE_HID__
        Joystick.releaseButton(JOYBTN0);
      #endif
    }
  }
}

/**
 * @brief 
* Arduino Setup Function
*
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() {
/**
 * @brief Setup Inputs and Outputs
 * 
 */
pinMode(PSU_OK, INPUT);                      // power supply is on and ok

pcpwr.attach(SIMPWR_PCPWR, INPUT_PULLUP);
// pinMode(SIMPWR_PCPWR, INPUT_PULLUP);      // PC POWER

pcreset.attach(SIMPWR_PCRESET, INPUT_PULLUP);
// pinMode(SIMPWR_PCRESET, INPUT_PULLUP);    // PC RESERT

masteron.attach(SIMPWR_MASTERON, INPUT_PULLUP );
// pinMode(SIMPWR_MASTERON, INPUT_PULLUP);   // MASTER ON 

dispon.attach(SIMPWR_DISPON, INPUT_PULLUP);
// pinMode(SIMPWR_DISPON, INPUT_PULLUP);     // DISPLAYS ON

pfltmode.attach(SIMPWR_PFLT_MODE, INPUT_PULLUP);
// pinMode(SIMPWR_PFLT_MODE, INPUT_PULLUP);  //

maintmode.attach(SIMPWR_MAINT_MODE, INPUT_PULLUP );
// pinMode(SIMPWR_MAINT_MODE, INPUT_PULLUP); // SIM IN MAINTENANCE MODE

pinMode(PSU_ON, OUTPUT);                  // TURN ON/OFF POWER SUPPLY
pinMode(EXT_ACC1,OUTPUT);                 // EXTERNAL ACCESSORY 1 // PC POWER
pinMode(EXT_ACC2,OUTPUT);                 // EXTERNAL ACCESSORY 2 // PC RESET
pinMode(EXT_ACC3,OUTPUT);                 // EXTERNAL ACCESSORY 3 // DISPLAY POWER
pinMode(EXT_ACC4,OUTPUT);                 // EXTERNAL ACCESSORY 4 // NC

// DEBOUNCE INTERVAL IN MILLISECONDS
masteron.interval(50); 
maintmode.interval(50); 
pfltmode.interval(50);
dispon.interval(50);
pcpwr.interval(50);
pcreset.interval(50);

// get current power state

power_state = digitalRead(PSU_OK);

// Run DCS Bios setup function
DcsBios::setup();
#ifdef __USE_HID__
  Joystick.begin();
#endif
}

/**
* @brief Arduino Loop Function
*
* Arduino standard Loop Function. Code who should be executed
* over and over in a loop, belongs in this function.
*/
void loop() {

  //Run DCS Bios loop function
  DcsBios::loop();

  // check if PSU_OK is good,  if it is begin checking all switches otherwise only check masteron
  if( digitalRead(PSU_OK) == true && power_state == HIGH ) {
    simpwr_masteron();
    simpwr_pcpwr();
    simpwr_dispon();
    simpwr_pcreset();
    simpwr_mode();
  } else {
    simpwr_masteron();
  }
 }