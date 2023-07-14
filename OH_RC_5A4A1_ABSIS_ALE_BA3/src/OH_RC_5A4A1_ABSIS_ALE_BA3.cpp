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
 * DCS-BIOS OH5A4A1-1 Board
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

/**
 * @brief If debugging is needed include the needed library
 * 
 */

#ifdef DEBUG
  #include "avr8-stub.h"
#endif

//#include <SwitecX12.h>
//#include <SwitecX25.h>

/**
 * @brief DEFINE ALL I/O PINS USED
 * 
 */
#define L_GEN_SW_PIN        5
#define BATTERY_SW_PIN_A    2
#define BATTERY_SW_PIN_B    4
#define R_GEN_SW_PIN        3
#define VOLTE_SERVO_PIN     8
#define VOLTU_SERVO_PIN     11

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
DcsBios::Switch3Pos batterySw("BATTERY_SW", BATTERY_SW_PIN_A, BATTERY_SW_PIN_B);
DcsBios::Switch2Pos lGenSw("L_GEN_SW", L_GEN_SW_PIN);
DcsBios::Switch2Pos rGenSw("R_GEN_SW", R_GEN_SW_PIN);

DcsBios::ServoOutput voltE (0x753e,VOLTE_SERVO_PIN, 544, 2400, [](unsigned int newValue) -> unsigned int {
return map(newValue, 418, 41309, 1973, 1450);
});
DcsBios::ServoOutput voltU (0x753c, VOLTU_SERVO_PIN, 544, 2400, [](unsigned int newValue) -> unsigned int {
return map(newValue, 418, 41309, 1111,  1662);
});

/**
 * @brief ON STARTUP OF DCS CHECK AND SENT SWITCH STATE
 * 
 * @param newValue 
 */
void onAcftNameChange(char* newValue) {

// when we get the name change event the sim has started.
// read the current switch postiions and send to DCS

    if (newValue == "FA-18C_hornet") {
    const char bat_sw_a = digitalRead(BATTERY_SW_PIN_A) + '0';
    tryToSendDcsBiosMessage("L_GEN_SW", &bat_sw_a);
    const char bat_sw_b = digitalRead(BATTERY_SW_PIN_B) + '0';
    tryToSendDcsBiosMessage("L_GEN_SW", &bat_sw_b);
    const char r_gen = digitalRead(R_GEN_SW_PIN) + '0';
    tryToSendDcsBiosMessage("L_GEN_SW", &r_gen);
    const char l_gen = digitalRead(L_GEN_SW_PIN) + '0';
    tryToSendDcsBiosMessage("L_GEN_SW", &l_gen);
    __is_hornet__ = true;
    }  else {
      __is_hornet__ = false;
    } 
    
    __is_started__ == false;

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