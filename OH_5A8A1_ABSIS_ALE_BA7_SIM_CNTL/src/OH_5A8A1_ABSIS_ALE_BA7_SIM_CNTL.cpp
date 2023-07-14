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
 * @file OH_5A8A1_ABSIS_ALE_BA7_SIM_CNTL.ino
 * @author <Sandra Carroll> @ <smgvbest@gmail.com>
 * @date <01.27.2023>
 * @version u.0.0.1 Alpha
 * @warning This sketch is based on a wiring diagram, and was not yet tested on hardware.
 *
 * @details 
 * 
 * 
 *  * **Intended Board:**
 * ABSIS SIMULATOR CONTROL PANEL (SIM CNTL)
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

#define SIM_CNTL_HT_FRZE      15
#define SIM_CNTL_HT_CTR       6
#define SIM_CNTL_TIME_FAST    14      
#define SIM_CNTL_TIME_REAL    7
#define SIM_CNTL_TOG_NVG      16
#define SIM_CNTL_TOG_LABL     8
#define SIM_CNTL_GME_PAUSE    10
#define SIM_CNTL_GME_FREEZE   9
#define SIM_CNTL_VIEW_CHASE   A3
#define SIM_CNTL_VIEW_EXT     2
#define SIM_CNTL_VIEW_FLBY    A2
#define SIM_CNTL_VIEW_WPN     3
#define SIM_CNTL_VIEW_ENMY    A1
#define SIM_CNTL_VIEW_HUD     4
#define SIM_CNTL_VIEW_MAP     A0


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

// Buttons
#define JOYBTN0           0
#define JOYBTN1           1
#define JOYBTN2           2
#define JOYBTN3           3
#define JOYBTN4           4
#define JOYBTN5           5
#define JOYBTN6           6
#define JOYBTN7           7
#define JOYBTN8           8
#define JOYBTN9           9
#define JOYBTN10          10
#define JOYBTN11          11
#define JOYBTN12          12
#define JOYBTN13          13
#define JOYBTN14          14
#define JOYBTN15          15
#define JOYBTN16          16

// HID Setup
#define JOYSTICK_BUTTON_COUNT     17
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
 * @brief 
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

/**
 * @brief 
 * 
 */
Bounce game_pause = Bounce();
Bounce game_freeze = Bounce();
Bounce time_real = Bounce();
Bounce time_fast = Bounce();
Bounce ht_freeze = Bounce();
Bounce ht_ctr = Bounce();
Bounce toggle_labl = Bounce();
Bounce toggle_nvg = Bounce();
Bounce view_hud = Bounce();
Bounce view_map = Bounce();
Bounce view_ckpt = Bounce();
Bounce view_chase = Bounce();
Bounce view_ext = Bounce();
Bounce view_flby = Bounce();
Bounce view_wpn = Bounce();
Bounce view_enmy = Bounce();

bool power_state = false;

/**
 * @brief game_mode()
 * 
 */
void game_mode() {
  static uint8_t last_mode = 0;

  game_freeze.update();
  game_pause.update();

  if (( game_freeze.read() == HIGH ) && ( game_pause.read() == HIGH )) {
    switch (last_mode) {
      case JOYBTN0:
        break;
      case JOYBTN1:
        Joystick.releaseButton(JOYBTN1);
        Joystick.pressButton(JOYBTN1);
        Joystick.releaseButton(JOYBTN1);
        last_mode = JOYBTN0;
        break;
      case JOYBTN2:
        Joystick.releaseButton(JOYBTN2);
        Joystick.pressButton(JOYBTN2);
        Joystick.releaseButton(JOYBTN2);
        last_mode = JOYBTN0;
        break;
      default:
        break;
    }
    return;
  }

  if (( game_freeze.read() == LOW ) && ( game_pause.read() == HIGH )) {
      Joystick.pressButton(JOYBTN2);
      Joystick.releaseButton(JOYBTN1);
      Joystick.releaseButton(JOYBTN0);
      last_mode = JOYBTN2;
  }

  if (( game_freeze.read() == HIGH ) && ( game_pause.read() == LOW )) {
      Joystick.pressButton(JOYBTN1);
      Joystick.releaseButton(JOYBTN2);
      Joystick.releaseButton(JOYBTN0);
      last_mode = JOYBTN1;
  }
}

/**
 * @brief time_mode()
 * 
 */
void time_mode() {

  time_fast.update();
  time_real.update();

  if ( time_fast.read() == LOW ) {
    Joystick.pressButton(JOYBTN3);
  }  else {
    Joystick.releaseButton(JOYBTN3);
  }
  
  if ( time_real.read() == LOW ) {
    Joystick.pressButton(JOYBTN4);
  } else {
    Joystick.releaseButton(JOYBTN4);
  }

}

/**
 * @brief head_track_mode() 
 * 
 */
void head_track_mode() {
static uint8_t last_mode = JOYBTN0;

  ht_ctr.update();
  ht_freeze.update();

if (( ht_ctr.read() == HIGH ) && ( ht_freeze.read() == HIGH )) {
    switch (last_mode) {
      case JOYBTN0:
        break;
      case JOYBTN7:
        Joystick.releaseButton(JOYBTN7);
        Joystick.pressButton(JOYBTN7);
        Joystick.releaseButton(JOYBTN7);
        last_mode = JOYBTN0;
        break;
      case JOYBTN8:
        Joystick.releaseButton(JOYBTN8);
        Joystick.pressButton(JOYBTN8);
        Joystick.releaseButton(JOYBTN8);
        last_mode = JOYBTN0;
        break;
      default:
        break;
    }
    return;
  }

  if (( ht_freeze.read() == LOW ) && ( ht_ctr.read() == HIGH )) {
      Joystick.pressButton(JOYBTN7);
      Joystick.releaseButton(JOYBTN8);
      last_mode = JOYBTN7;
  }

  if (( ht_freeze.read() == HIGH ) && ( ht_ctr.read() == LOW )) {
      Joystick.pressButton(JOYBTN8);
      Joystick.releaseButton(JOYBTN7);
      last_mode = JOYBTN8;
  }

}

/**
 * @brief toggle_mode()
 * 
 */
void toggle_mode() {

  toggle_labl.update();
  toggle_nvg.update();

  if ( toggle_labl.read() == LOW ) {
    Joystick.pressButton(JOYBTN5);
  }  else {
    Joystick.releaseButton(JOYBTN5);
  }
  
  if ( toggle_nvg.read() == LOW ) {
    Joystick.pressButton(JOYBTN6);
  } else {
    Joystick.releaseButton(JOYBTN6);
  }

}

/**
 * @brief view_mode()
 * 
 */
void view_mode() {

  view_chase.update();
  view_ckpt.update();
  view_enmy.update();
  view_ext.update();
  view_flby.update();
  view_hud.update();
  view_map.update();
  view_wpn.update();

  if ( view_hud.read() == LOW ) {
    Joystick.pressButton(JOYBTN9);
  }  else {
    Joystick.releaseButton(JOYBTN9);
  }

  if ( view_map.read() == LOW ) {
    Joystick.pressButton(JOYBTN10);
  }  else {
    Joystick.releaseButton(JOYBTN10);
  }

  if ( view_ckpt.read() == LOW ) {
    Joystick.pressButton(JOYBTN11);
  }  else {
    Joystick.releaseButton(JOYBTN11);
  }

  if ( view_chase.read() == LOW ) {
    Joystick.pressButton(JOYBTN12);
  }  else {
    Joystick.releaseButton(JOYBTN12);
  }

  if ( view_ext.read() == LOW ) {
    Joystick.pressButton(JOYBTN13);
  }  else {
    Joystick.releaseButton(JOYBTN13);
  }

  if ( view_flby.read() == LOW ) {
    Joystick.pressButton(JOYBTN14);
  }  else {
    Joystick.releaseButton(JOYBTN14);
  }

  if ( view_wpn.read() == LOW ) {
    Joystick.pressButton(JOYBTN15);
  }  else {
    Joystick.releaseButton(JOYBTN15);
  } 

  if ( view_enmy.read() == LOW ) {
    Joystick.pressButton(JOYBTN16);
  }  else {
    Joystick.releaseButton(JOYBTN16);
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

game_freeze.attach(SIM_CNTL_GME_FREEZE, INPUT_PULLUP);
game_pause.attach(SIM_CNTL_GME_PAUSE, INPUT_PULLUP);
game_freeze.interval(50);
game_pause.interval(50);

time_fast.attach(SIM_CNTL_TIME_FAST,INPUT_PULLUP);
time_real.attach(SIM_CNTL_TIME_REAL,INPUT_PULLUP);
time_fast.interval(50);
time_real.interval(50);

toggle_labl.attach(SIM_CNTL_TOG_LABL,INPUT_PULLUP);
toggle_nvg.attach(SIM_CNTL_TOG_NVG,INPUT_PULLUP);
toggle_labl.interval(50);
toggle_nvg.interval(50);

ht_freeze.attach(SIM_CNTL_HT_FRZE,INPUT_PULLUP);
ht_ctr.attach(SIM_CNTL_HT_CTR,INPUT_PULLUP);
ht_freeze.interval(50);
ht_ctr.interval(50);

view_chase.attach(SIM_CNTL_VIEW_CHASE,INPUT_PULLUP);
//view_ckpt.attach(SIM)
view_enmy.attach(SIM_CNTL_VIEW_ENMY,INPUT_PULLUP);
view_ext.attach(SIM_CNTL_VIEW_EXT,INPUT_PULLUP);
view_flby.attach(SIM_CNTL_VIEW_FLBY,INPUT_PULLUP);
view_hud.attach(SIM_CNTL_VIEW_HUD,INPUT_PULLUP);
view_map.attach(SIM_CNTL_VIEW_MAP,INPUT_PULLUP);
view_wpn.attach(SIM_CNTL_VIEW_WPN,INPUT_PULLUP);

// Run DCS Bios setup function
DcsBios::setup();
Joystick.begin();
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

  game_mode();
  time_mode();
  toggle_mode();
  head_track_mode();
  view_mode();
  
 }