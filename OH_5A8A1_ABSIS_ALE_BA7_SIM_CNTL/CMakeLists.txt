cmake_minimum_required(VERSION 3.25)
project(OH_5A8A1_ABSIS_ALE_BA7_SIM_CNTL)

set(CMAKE_CXX_STANDARD 14)

include_directories(.pio/libdeps/release/Bounce2/examples)
include_directories(.pio/libdeps/release/Bounce2/examples/bounce_basic)
include_directories(.pio/libdeps/release/Bounce2/examples/bounce_button)
include_directories(.pio/libdeps/release/Bounce2/examples/more)
include_directories(.pio/libdeps/release/Bounce2/examples/more/bounce_current_duration)
include_directories(.pio/libdeps/release/Bounce2/examples/more/bounce_previous_duration)
include_directories(.pio/libdeps/release/Bounce2/examples/more/bounce_previous_duration_blink_toggle)
include_directories(.pio/libdeps/release/Bounce2/examples/more/bounceMore)
include_directories(.pio/libdeps/release/Bounce2/examples/more/bounceTwo)
include_directories(.pio/libdeps/release/Bounce2/examples/more/change)
include_directories(.pio/libdeps/release/Bounce2/examples/more/retrigger)
include_directories(.pio/libdeps/release/Bounce2/src)
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/Button_Matrix_Example")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/DefaultSerial")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/DualModeTest")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/DynamicControlMapping")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/IRQSerial")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/MasterCaution")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/OneOfEverything")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/PRESET___FREQ_UHF_RADIO_M2000C")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/PRESET___FREQ_UVHF_RADIO_M2000C")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/RS485Master")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/RS485Slave")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/RS485SlaveUART")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/examples/Synchronization")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/examples")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/examples/Knob")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/examples/Sweep")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/avr")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/mbed")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/megaavr")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/nrf52")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/sam")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/samd")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/stm32f4")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/src")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal")
include_directories(".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/UART.Mod")
include_directories(.pio/libdeps/release/Joystick/examples)
include_directories(.pio/libdeps/release/Joystick/examples/ArcadeStickExample)
include_directories(.pio/libdeps/release/Joystick/examples/DrivingControllerTest)
include_directories(.pio/libdeps/release/Joystick/examples/FlightControllerTest)
include_directories(.pio/libdeps/release/Joystick/examples/FunduinoJoystickShield)
include_directories(.pio/libdeps/release/Joystick/examples/GamepadExample)
include_directories(.pio/libdeps/release/Joystick/examples/HatSwitchTest)
include_directories(.pio/libdeps/release/Joystick/examples/JoystickButton)
include_directories(.pio/libdeps/release/Joystick/examples/JoystickKeyboard)
include_directories(.pio/libdeps/release/Joystick/examples/JoystickTest)
include_directories(.pio/libdeps/release/Joystick/examples/MultipleJoystickTest)
include_directories(.pio/libdeps/release/Joystick/src)
include_directories(.pio/libdeps/release/Joystick/src/DynamicHID)
include_directories(.pio/libdeps/release/Servo/examples)
include_directories(.pio/libdeps/release/Servo/examples/Knob)
include_directories(.pio/libdeps/release/Servo/examples/Sweep)
include_directories(.pio/libdeps/release/Servo/src)
include_directories(.pio/libdeps/release/Servo/src/avr)
include_directories(.pio/libdeps/release/Servo/src/mbed)
include_directories(.pio/libdeps/release/Servo/src/megaavr)
include_directories(.pio/libdeps/release/Servo/src/nrf52)
include_directories(.pio/libdeps/release/Servo/src/sam)
include_directories(.pio/libdeps/release/Servo/src/samd)
include_directories(.pio/libdeps/release/Servo/src/stm32f4)
include_directories(boards)

add_executable(OH_5A8A1_ABSIS_ALE_BA7_SIM_CNTL
        .pio/libdeps/release/Bounce2/examples/bounce_basic/bounce_basic.ino
        .pio/libdeps/release/Bounce2/examples/bounce_button/bounce_button.ino
        .pio/libdeps/release/Bounce2/examples/more/bounce_current_duration/bounce_current_duration.ino
        .pio/libdeps/release/Bounce2/examples/more/bounce_previous_duration/bounce_previous_duration.ino
        .pio/libdeps/release/Bounce2/examples/more/bounce_previous_duration_blink_toggle/bounce_previous_duration_blink_toggle.ino
        .pio/libdeps/release/Bounce2/examples/more/bounceMore/bounceMore.ino
        .pio/libdeps/release/Bounce2/examples/more/bounceTwo/bounceTwo.ino
        .pio/libdeps/release/Bounce2/examples/more/change/change.ino
        .pio/libdeps/release/Bounce2/examples/more/retrigger/retrigger.ino
        .pio/libdeps/release/Bounce2/src/Bounce2.cpp
        .pio/libdeps/release/Bounce2/src/Bounce2.h
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/Button_Matrix_Example/Button_Matrix_Example.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/CDU Font/DefaultFonts.c"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/DefaultSerial/DefaultSerial.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/DualModeTest/DualModeTest.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/DynamicControlMapping/DynamicControlMapping.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/IRQSerial/IRQSerial.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/MasterCaution/MasterCaution.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/OneOfEverything/OneOfEverything.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/PRESET___FREQ_UHF_RADIO_M2000C/PRESET___FREQ_UHF_RADIO_M2000C.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/PRESET___FREQ_UVHF_RADIO_M2000C/PRESET___FREQ_UVHF_RADIO_M2000C.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/RS485Master/RS485Master.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/RS485Slave/RS485Slave.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/RS485SlaveUART/RS485SlaveUART.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/examples/Synchronization/Synchronization.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/examples/Knob/Knob.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/examples/Sweep/Sweep.ino"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/avr/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/avr/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/mbed/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/mbed/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/megaavr/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/megaavr/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/nrf52/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/nrf52/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/sam/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/sam/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/samd/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/samd/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/stm32f4/Servo.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/stm32f4/ServoTimers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/lib/Servo/src/Servo.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/UART.Mod/DcsBiosNgRS485Slave.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/AnalogMultiPos.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/BcdWheels.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Buttons.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/DcsBiosNgRS485Master.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/DcsBiosNgRS485Slave.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Dimmer.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/DualModeButton.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Encoders.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/ExportStreamListener.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Leds.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/MatrixSwitches.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/PollingInput.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Potentiometers.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Protocol.cpp"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Protocol.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/RingBuffer.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/RotarySwitch.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/RotarySyncingPotentiometer.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Servos.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/SetStateInput.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/Switches.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/internal/SyncingSwitches.h"
        ".pio/libdeps/release/DCS-BIOS FP-Fork/src/DcsBios.h"
        .pio/libdeps/release/Joystick/examples/ArcadeStickExample/ArcadeStickExample.ino
        .pio/libdeps/release/Joystick/examples/DrivingControllerTest/DrivingControllerTest.ino
        .pio/libdeps/release/Joystick/examples/FlightControllerTest/FlightControllerTest.ino
        .pio/libdeps/release/Joystick/examples/FunduinoJoystickShield/FunduinoJoystickShield.ino
        .pio/libdeps/release/Joystick/examples/GamepadExample/GamepadExample.ino
        .pio/libdeps/release/Joystick/examples/HatSwitchTest/HatSwitchTest.ino
        .pio/libdeps/release/Joystick/examples/JoystickButton/JoystickButton.ino
        .pio/libdeps/release/Joystick/examples/JoystickKeyboard/JoystickKeyboard.ino
        .pio/libdeps/release/Joystick/examples/JoystickTest/JoystickTest.ino
        .pio/libdeps/release/Joystick/examples/MultipleJoystickTest/MultipleJoystickTest.ino
        .pio/libdeps/release/Joystick/src/DynamicHID/DynamicHID.cpp
        .pio/libdeps/release/Joystick/src/DynamicHID/DynamicHID.h
        .pio/libdeps/release/Joystick/src/Joystick.cpp
        .pio/libdeps/release/Joystick/src/Joystick.h
        .pio/libdeps/release/Servo/examples/Knob/Knob.ino
        .pio/libdeps/release/Servo/examples/Sweep/Sweep.ino
        .pio/libdeps/release/Servo/src/avr/Servo.cpp
        .pio/libdeps/release/Servo/src/avr/ServoTimers.h
        .pio/libdeps/release/Servo/src/mbed/Servo.cpp
        .pio/libdeps/release/Servo/src/mbed/ServoTimers.h
        .pio/libdeps/release/Servo/src/megaavr/Servo.cpp
        .pio/libdeps/release/Servo/src/megaavr/ServoTimers.h
        .pio/libdeps/release/Servo/src/nrf52/Servo.cpp
        .pio/libdeps/release/Servo/src/nrf52/ServoTimers.h
        .pio/libdeps/release/Servo/src/sam/Servo.cpp
        .pio/libdeps/release/Servo/src/sam/ServoTimers.h
        .pio/libdeps/release/Servo/src/samd/Servo.cpp
        .pio/libdeps/release/Servo/src/samd/ServoTimers.h
        .pio/libdeps/release/Servo/src/stm32f4/Servo.cpp
        .pio/libdeps/release/Servo/src/stm32f4/ServoTimers.h
        .pio/libdeps/release/Servo/src/Servo.h
        boards/openhornet_pro_micro.json
        boards/pins_arduino.h
        src/OH_5A8A1_ABSIS_ALE_BA7_SIM_CNTL.cpp)
