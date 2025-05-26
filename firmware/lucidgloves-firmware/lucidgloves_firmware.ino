/**
 * @file lucidgloves_firmware.ino
 * @brief Main configuration file for LucidGloves Firmware v4.
 * 
 * This file defines board-specific pin mappings, communication settings,
 * gesture detection options, button inversion, and other core hardware features.
 *
 * Author: Lucas_VRTech - LucidVR
 * Source: lucidvrtech.com
 */

 #include "AdvancedConfig.h"  // Loads loop timing, encoding, and finger index config

 // =============================
 // Communication Configuration
 // =============================
 
 // Set communication method: COMM_SERIAL (USB) or COMM_BTSERIAL (Bluetooth)
 #define COMMUNICATION COMM_SERIAL
 
 // Serial (USB) configuration
 #define SERIAL_BAUD_RATE 115200
 
 // Bluetooth device name (used only if COMMUNICATION == COMM_BTSERIAL)
 #define BTSERIAL_DEVICE_NAME "lucidgloves-left"
 
 // =============================
 // Analog Input Settings
 // =============================
 
 // Set to true to invert analog readings from flex sensors (e.g., backwards pots)
 #define FLIP_POTS false
 
 // =============================
 // Gesture Input Configuration
 // =============================
 
 // Set to true to use gesture-based input (flexion thresholding)
 // Set to false to fall back to physical buttons
 #define TRIGGER_GESTURE true
 #define GRAB_GESTURE    true
 #define PINCH_GESTURE   true
 
 // =============================
 // Button Inversion
 // =============================
 
 // If buttons behave in reverse (e.g., normally-closed switches), invert them here
 #define INVERT_A       false
 #define INVERT_B       false
 #define INVERT_JOY     false
 #define INVERT_MENU    false
 #define INVERT_CALIB   false
 
 // Gesture override buttons (only used if gesture mode is disabled)
 #define INVERT_TRIGGER false
 #define INVERT_GRAB    false
 #define INVERT_PINCH   false
 
 // =============================
 // Joystick Configuration
 // =============================
 
 #define JOYSTICK_BLANK    false  // Set true if not using a joystick
 #define JOY_FLIP_X        false
 #define JOY_FLIP_Y        false
 #define JOYSTICK_DEADZONE 10     // Deadzone percentage (0–100)
 
 // =============================
 // Sensor Use Options
 // =============================
 
 #define NO_THUMB          false  // Set true to disable thumb sensor
 #define USING_CALIB_PIN   false  // Enables recalibration using dedicated button/pin
 #define USING_FORCE_FEEDBACK false // Enable servo-based force feedback
 #define SERVO_SCALING     false  // Enable dynamic (calibrated) servo scaling
 
 // =============================
 // Pin Configuration
 // =============================
 
 #if defined(ESP32)
 
 // --- ESP32 DOIT V1 Pin Mapping ---
 #define PIN_PINKY        36
 #define PIN_RING         39
 #define PIN_MIDDLE       34
 #define PIN_INDEX        35
 #define PIN_THUMB        32
 #define PIN_JOY_X        33
 #define PIN_JOY_Y        25
 #define PIN_JOY_BTN      26
 #define PIN_A_BTN        27
 #define PIN_B_BTN        14
 #define PIN_TRIG_BTN     12
 #define PIN_GRAB_BTN     13
 #define PIN_PNCH_BTN     23
 #define PIN_CALIB        12  // Can use GPIO0 for BOOT button (Bluetooth only)
 #define PIN_MENU_BTN     27
 #define DEBUG_LED         2
 
 // Servo motor pins (used for force feedback)
 #define PIN_PINKY_MOTOR   5
 #define PIN_RING_MOTOR    18
 #define PIN_MIDDLE_MOTOR  19
 #define PIN_INDEX_MOTOR   21
 #define PIN_THUMB_MOTOR   17
 
 #elif defined(__AVR__)
 
 // --- Arduino Nano Pin Mapping ---
 #define PIN_PINKY        A0
 #define PIN_RING         A1
 #define PIN_MIDDLE       A2
 #define PIN_INDEX        A3
 #define PIN_THUMB        A4
 #define PIN_JOY_X        A6
 #define PIN_JOY_Y        A7
 #define PIN_JOY_BTN       7
 #define PIN_A_BTN         8
 #define PIN_B_BTN         9
 #define PIN_TRIG_BTN     10
 #define PIN_GRAB_BTN     11
 #define PIN_PNCH_BTN     12
 #define PIN_CALIB        13
 #define PIN_MENU_BTN      8
 #define DEBUG_LED   LED_BUILTIN
 
 // Servo motor pins
 #define PIN_PINKY_MOTOR   2
 #define PIN_RING_MOTOR    3
 #define PIN_MIDDLE_MOTOR  4
 #define PIN_INDEX_MOTOR   5
 #define PIN_THUMB_MOTOR   6
 
 #endif
 