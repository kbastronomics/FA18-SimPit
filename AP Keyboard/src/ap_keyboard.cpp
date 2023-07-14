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
 * @file ap_keyboard.ino
 * @author Sandra Carroll
 * @date July 7,2023
 * @brief AP Keyboard
 *
 * DCS-BIOS AP Keyboard
 *  Supports:
 *      Backlight Neopixels (Set to GREEN).
 * 
 * REQUIRES: 
 *  DcsBios
 *  Servo
 *  Fastled
 *  Joystick
 *  PCA95x5
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

// UNCOMMENT TO RUN TEST PROGRAMS
#define RUN_TEST  

// COMMENT OUT TO DISABEL INTERUPTS
// #define _USE_INTERRUPTS_

// UN-COMMENT TO USE WS2812B
// #define _USE_WS2812B_  

#include <Arduino.h>
#include "DcsBios.h"               // smgvbest/DCS-BIOS FP-Fork@^0.3.8
#include "Fastled.h"
#include <PCA95x5.h>
//#include "Joystick.h"             // 	mheironimus/Joystick@^2.1.1

#define VERSION       0
#define RELEASE       1
#define MODIFICATION  0
#define STATE         " beta1"
// #define SELF_TEST     // comment out to skip SELF TEST

/**
 * @brief TASK SCHEDULER IF NEEDED
 * 
 */
//#define _TASK_MICRO_RES
// #include <TaskScheduler.h>
// #define T1_INIT (211L)

#define NUM_LEDS        28
#define LED_TYPE   WS2812B
#define COLOR_ORDER   GRB
#define DATA_PIN        4
#define CLK_PIN         4
#define VOLTS           5
#define MAX_MA        100

#define BL_PIN          5
/*
* DEFINE ALL I/O PINS USED
*/
#define AP_KEYPAD_BACKLIGHT_PIN         4

#define AP_SW     0x0C  // P13
#define IFF_SW    0x0D  // P14
#define TCN_SW    0x0E  // P15
#define ILS_SW    0x0F  // P16
#define DL_SW     0x03  // P03
#define BCN_SW    0x04  // P04
#define ONOFF_SW  0x05  // P05

#define READ_AP    (ioex.read(PCA95x5::Port::P13))
#define READ_IFF   (ioex.read(PCA95x5::Port::P14))
#define READ_TCN   (ioex.read(PCA95x5::Port::P15))
#define READ_ILS   (ioex.read(PCA95x5::Port::P16))
#define READ_DL    (ioex.read(PCA95x5::Port::P03))
#define READ_BCN   (ioex.read(PCA95x5::Port::P04))
#define READ_ONOFF (ioex.read(PCA95x5::Port::P05))


#ifdef _USE_INTERRUPTS_
  #define AP_KEYPAD_IRQ   XX    // PICK A PIN
#endif

#define PRESSED   LOW
#define RELEASED  HIGH

static bool lastAPState     = RELEASED;
static bool lastIFFState    = RELEASED;
static bool lastTCNState    = RELEASED;
static bool lastILSState    = RELEASED;
static bool lastDLState     = RELEASED;
static bool lastBCNState    = RELEASED;
static bool lastONOFFState  = RELEASED;

/**
 * @brief DEFINE START POSITION OF EACH SET OF BACKLIGHTS
 *
*/
#define AP_KEYPAD_BACKLIGHT             0

/**
 * @brief DEFINE NEOPIXEL LENGHT OF EACH SET OF BACKLIGHTS
 * 
 */
#define AP_KEYPAD_LEN             28
#define AP_KEYPAD_LED_START       0

/*
 * DEFINE PINS AND STATES USED FOR APU CONTROL
 */
#define BACKLIGHT_STRIP_NUMPIXELS AP_KEYPAD_LEN

PCA9555 ioex;
uint16_t cBits=0;
uint16_t pBits=0;

CRGBArray<AP_KEYPAD_LEN> leds;

#define JOYSTICK_BUTTON_COUNT     7
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

const bool initAutoSendState = true;

/**
 * @brief 
 * 
 */
// Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
//                    JOYSTICK_TYPE_JOYSTICK, 
//                    JOYSTICK_BUTTON_COUNT, 
//                    JOYSTICK_HATSWITCH_COUNT, 
//                    INCLUDE_X_AXIS, 
//                    INCLUDE_Y_AXIS, 
//                    INCLUDE_Z_AXIS, 
//                    INCLUDE_RX_AXIS, 
//                    INCLUDE_RY_AXIS, 
//                    INCLUDE_RZ_AXIS, 
//                    INCLUDE_RUDDER, 
//                    INCLUDE_THROTTLE, 
//                    INCLUDE_ACCELERATOR, 
//                    INCLUDE_BRAKE, 
//                    INCLUDE_STEERING);


/**
 * @brief ap_keypad_function_select()
 * Test the AP Keyboard
 * 
 * @returns Does not return
 */
void ap_keypad_function_select() {
  Serial.printf("========== UFC FUNCTION SELECT PANEL TEST ==========\n");

  PCA9555 ioex;
  uint16_t cBits=0;
  uint16_t pBits=0;

  Wire.begin();
  ioex.attach(Wire);
  ioex.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
  ioex.direction(PCA95x5::Direction::IN_ALL);
  ioex.write(PCA95x5::Level::L_ALL);

  pBits = cBits = ioex.read();

  for (;;) {
    cBits=ioex.read();
    if(cBits != pBits) {
      switch (cBits) 
      {
      case AP_SW:
        Serial.printf("AP KEY PRESSED\n");
        break;
      case IFF_SW:
        Serial.printf("IFF KEY PRESSED\n");
        /* code */
        break;
      case TCN_SW:
        Serial.printf("TCN KEY PRESSED\n");
        /* code */
        break;
      case ILS_SW:
        Serial.printf("ILS KEY PRESSED\n");
        /* code */
        break;
      case DL_SW:
        Serial.printf("DL KEY PRESSED\n");
        /* code */
        break;
      case BCN_SW:
        Serial.printf("BCN KEY PRESSED\n");
        /* code */
        break;
      case ONOFF_SW:
        Serial.printf("ON/OFF KEY PRESSED\n");
        /* code */
        break;
      case ONOFF_SW && AP_SW:   
        Serial.printf("AP+ON/OFF KEY PRESSED...\nExiting\n");
        return;
      default:
        break;
      }
    }
    pBits = cBits;
  }
}


/**
 * @brief void i2c_test()
 * test I2C buss for keyboard
 * 
 */
void i2c_test() {

  byte error, address;
  int nDevices;
  int ap=false;
  Wire.begin ();  

  Serial.printf("========== SCANNING I2C BUSS ==========\n");
  nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("I2C device found at address %#02X\n",address);
      switch(address) {
        case 0x20:
          ap=true;
          break;
        default:
          break;
      }
      nDevices++;
    }
    else if (error==4) {
      Serial.printf("Unknow error at address %#02X\n",address);
      }    
  }

  Serial.printf("======== START REPORT ========\n");
  (ap == true) ? Serial.printf("AP I2C Keyboard: PASSED.\n") : Serial.printf("AP I2C Keyboard: FAILED.\n");
  Serial.printf("======== END REPORT ========\n");
}


void onInstrIntLtChange(unsigned int newValue) {
  switch(newValue) {
    case 1: // ON
      #ifdef _USE_WS2812B_
      leds[AP_KEYPAD_LED_START] = CRGB(map(newValue,0,65535,0,255),0,0);  // GRB
      FastLED.show();
      #else
      analogWrite(BL_PIN,map(newValue,0,65535,0,255));
      #endif
      break; 
    case 0: // OFF
      #ifdef _USE_WS2812B_
      leds[AP_KEYPAD_LED_START] = CRGB(0,0,0);
      FastLED.show();
      #else
      analogWrite(BL_PIN,0);
      #endif
      break; 
    default: //DO NOTHING
      break;
  }

}
DcsBios::IntegerBuffer instrIntLtBuffer(0x7560, 0xffff, 0, onInstrIntLtChange);


#ifdef _USE_INTERRUPTS_
/**
 * @brief AP Keyboard Interrupt Handler
 * 
 */
void apCallback() {
  // WE KNOW A PIN HAS CHANGES SO
  // *BLOCK IRQ
  // *READ 9555
  // FIGURE WHICH PIN CHANGED
  // UPDATE PIN FLAG
  // ENABLE IRQ
  // RETURN

  Disable_global_interrupt();
  pBits = ioex.read();
  Enable_global_interrupt();
}
#endif 

/**
* Arduino Setup Function
* 
* Arduino standard Setup Function. Code who should be executed
* only once at the programm start, belongs in this function.
*/
void setup() {
  Serial.begin(250000);
  while (!Serial); // wait for serial attach

  Serial.printf("KB10A1A2-1 AP Keyboard for OpenHornet V%dR%dM%d%s\n",VERSION,RELEASE,MODIFICATION,STATE);
  Serial.flush();

  digitalWrite(LED_BUILTIN,false);  // TURN OFF BUILTIN LED TO SHOW WE ENTERED SETUP

  #ifndef _USE_WS2812B_
    pinMode(BL_PIN,OUTPUT);
  #endif

  #ifdef RUN_TEST
    i2c_test();
    ap_keypad_function_select();
  #endif

  Wire.begin();
  ioex.attach(Wire);
  ioex.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
  ioex.direction(PCA95x5::Direction::IN_ALL);
  ioex.write(PCA95x5::Level::L_ALL);

  #ifdef _USE_INTERRUPTS_
    attachInterrupt(digitalPinToInterrupt(37),apCallback,RISING);
  #endif

  pBits = cBits = ioex.read();

  #ifdef _USE_WS2812B_
  // INITIALIZE WS2812B strip object (REQUIRED)
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  // Turn WS2812B's Off

  FastLED.clear();
  FastLED.show();
  #endif

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

   cBits=ioex.read();

  if(cBits != pBits) {
    if ( READ_AP == PRESSED && lastAPState == RELEASED ) {
      Serial.printf("Joystick.pressButton(JOYBTN1)\n");
      lastAPState = PRESSED;
    } else {
      Serial.printf("Joystick.releaseButton(JOYBTN1)\n");
      lastAPState = RELEASED;
    }

    if ( READ_IFF == PRESSED && lastIFFState == RELEASED ) {
      //Joystick.pressButton(JOYBTN2);
      lastIFFState = PRESSED;
    } else {
      //Joystick.releaseButton(JOYBTN2);
      lastIFFState = RELEASED;
    }

    if ( READ_TCN == PRESSED && lastTCNState == RELEASED ) {
      //Joystick.pressButton(JOYBTN3);
      lastTCNState = PRESSED;
    } else {
      //Joystick.releaseButton(JOYBTN3);
      lastTCNState = RELEASED;
    }

    if ( READ_ILS == PRESSED && lastILSState == RELEASED ) {
      //Joystick.pressButton(JOYBTN4);
      lastILSState = PRESSED;
    } else {
      //Joystick.releaseButton(JOYBTN4);
      lastILSState = RELEASED;
    }

    if ( READ_DL == PRESSED && lastDLState == RELEASED ) {
      //Joystick.pressButton(JOYBTN3);
      lastDLState = PRESSED;
    } else {
      //Joystick.releaseButton(JOYBTN3);
      lastDLState = RELEASED;
    }

    if ( READ_BCN == PRESSED && lastBCNState == RELEASED ) {
      //Joystick.pressButton(JOYBTN3);
      lastBCNState = PRESSED;
    } else {
      //Joystick.releaseButton(JOYBTN3);
      lastBCNState = RELEASED;
    }

    if ( READ_ONOFF == PRESSED && lastONOFFState == RELEASED ) {
      //Joystick.pressButton(JOYBTN3);
      lastONOFFState = PRESSED;
    } else {
      //Joystick.releaseButton(JOYBTN3);
      lastONOFFState = RELEASED;
    }
    pBits = cBits;
  }
} 
