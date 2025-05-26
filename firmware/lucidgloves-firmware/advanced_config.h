/**
 * @file AdvancedConfig.h
 * @brief Configuration header for LucidGloves firmware. Controls core behavior including
 * loop timing, communication mode, encoding format, sensor scaling, and filtering.
 *
 * Only modify these settings if you know what you're doing. This file affects how
 * the firmware behaves across all modules.
 */

// === Loop and Calibration Timing ===
#define LOOP_TIME 4              // Delay between main loop iterations (in ms). Set to 0 for fastest updates.
#define CALIBRATION_LOOPS -1     // Number of loops for calibration. Use -1 to always calibrate continuously.

// === Communication Modes ===
#define COMM_SERIAL   0          // USB serial
#define COMM_BTSERIAL 1          // Bluetooth serial

// === Encoding Modes ===
#define ENCODING        1        // Choose between legacy (0) and alpha (1)
#define ENCODE_LEGACY   0        // Legacy encoding: values separated by '&'
#define ENCODE_ALPHA    1        // Alpha encoding: labeled values like A420B533...

// === Finger Index Mapping (used for flexion array access) ===
#define THUMB_IND  0
#define INDEX_IND  1
#define MIDDLE_IND 2
#define RING_IND   3
#define PINKY_IND  4

// === Analog Input Configuration ===
// Automatically sets ANALOG_MAX based on detected microcontroller type
#if defined(__AVR__)
  #define ANALOG_MAX 1023
#elif defined(ESP32)
  #define ANALOG_MAX 4095 
#endif

// To manually override ANALOG_MAX (e.g. for custom boards), uncomment and modify this line:
// #define ANALOG_MAX 4095 

#ifndef ANALOG_MAX
  #error "This board doesn't have an automatic ANALOG_MAX assignment. Please set it manually."
#endif

// === Analog Input Clamping & Filtering ===
#define CLAMP_ANALOG_MAP true       // Clamp mapped analog values between 0 and ANALOG_MAX

#define CLAMP_FLEXION false         // If true, discard flexion readings outside min/max range
#define CLAMP_MIN 0                 // Minimum raw flex sensor value
#define CLAMP_MAX ANALOG_MAX        // Maximum raw flex sensor value

// === Optional Median Filter ===
// Helps reduce jitter in flex sensor input (requires RunningMedian library):
// https://www.arduino.cc/reference/en/libraries/runningmedian/
#define ENABLE_MEDIAN_FILTER false  // Enable/disable running median filter
#define MEDIAN_SAMPLES 20           // Number of samples to consider in the median
