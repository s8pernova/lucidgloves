/**
 * @file input.ino
 * @brief Handles input setup and analog readings from flex sensors and joystick.
 * Supports calibration, optional median filtering, and deadzone compensation.
 */

 #include "AdvancedConfig.h"

 #if ENABLE_MEDIAN_FILTER
   #include <RunningMedian.h>
   RunningMedian rmSamples[5] = {
       RunningMedian(MEDIAN_SAMPLES),
       RunningMedian(MEDIAN_SAMPLES),
       RunningMedian(MEDIAN_SAMPLES),
       RunningMedian(MEDIAN_SAMPLES),
       RunningMedian(MEDIAN_SAMPLES)
   };
 #endif
 
 int maxFingers[5] = {0, 0, 0, 0, 0};
 int minFingers[5] = {ANALOG_MAX, ANALOG_MAX, ANALOG_MAX, ANALOG_MAX, ANALOG_MAX};
 
 /**
  * @brief Initializes all input pins (buttons and optional calibration pin).
  */
 void setupInputs() {
   pinMode(PIN_JOY_BTN, INPUT_PULLUP);
   pinMode(PIN_A_BTN, INPUT_PULLUP);
   pinMode(PIN_B_BTN, INPUT_PULLUP);
   pinMode(PIN_MENU_BTN, INPUT_PULLUP);
 
   #if !TRIGGER_GESTURE
     pinMode(PIN_TRIG_BTN, INPUT_PULLUP);
   #endif
 
   #if !GRAB_GESTURE
     pinMode(PIN_GRAB_BTN, INPUT_PULLUP);
   #endif
 
   #if !PINCH_GESTURE
     pinMode(PIN_PNCH_BTN, INPUT_PULLUP);
   #endif
 
   #if USING_CALIB_PIN
     pinMode(PIN_CALIB, INPUT_PULLUP);
   #endif
 }
 
 /**
  * @brief Reads raw flexion data and processes it into calibrated values.
  * Applies flipping, median filtering, calibration logic, and clamping.
  * @param calibrating Whether to update min/max values
  * @param reset Whether to reset calibration data
  * @return Pointer to calibrated array of flexion values (0–ANALOG_MAX)
  */
 int* getFingerPositions(bool calibrating, bool reset) {
   int rawFingers[5] = {
     NO_THUMB ? 0 : analogRead(PIN_THUMB),
     analogRead(PIN_INDEX),
     analogRead(PIN_MIDDLE),
     analogRead(PIN_RING),
     analogRead(PIN_PINKY)
   };
 
   #if FLIP_POTS
   for (int i = 0; i < 5; i++) {
     rawFingers[i] = ANALOG_MAX - rawFingers[i];
   }
   #endif
 
   #if ENABLE_MEDIAN_FILTER
   for (int i = 0; i < 5; i++) {
     rmSamples[i].add(rawFingers[i]);
     rawFingers[i] = rmSamples[i].getMedian();
   }
   #endif
 
   if (reset) {
     for (int i = 0; i < 5; i++) {
       maxFingers[i] = 0;
       minFingers[i] = ANALOG_MAX;
     }
   }
 
   if (calibrating) {
     for (int i = 0; i < 5; i++) {
       if (rawFingers[i] > maxFingers[i]) {
         #if CLAMP_FLEXION
           maxFingers[i] = min(rawFingers[i], CLAMP_MAX);
         #else
           maxFingers[i] = rawFingers[i];
         #endif
       }
 
       if (rawFingers[i] < minFingers[i]) {
         #if CLAMP_FLEXION
           minFingers[i] = max(rawFingers[i], CLAMP_MIN);
         #else
           minFingers[i] = rawFingers[i];
         #endif
       }
     }
   }
 
   static int calibrated[5] = {511, 511, 511, 511, 511};
   for (int i = 0; i < 5; i++) {
     if (minFingers[i] != maxFingers[i]) {
       calibrated[i] = map(rawFingers[i], minFingers[i], maxFingers[i], 0, ANALOG_MAX);
 
       #if CLAMP_ANALOG_MAP
         if (calibrated[i] < 0) calibrated[i] = 0;
         if (calibrated[i] > ANALOG_MAX) calibrated[i] = ANALOG_MAX;
       #endif
     } else {
       calibrated[i] = ANALOG_MAX / 2;  // fallback if no calibration range
     }
   }
 
   return calibrated;
 }
 
 /**
  * @brief Applies joystick deadzone to raw analog reading.
  * @param pin Analog pin number
  * @return Deadzone-corrected analog value
  */
 int analogReadDeadzone(byte pin) {
   int raw = analogRead(pin);
   int center = ANALOG_MAX / 2;
   int deadzone = JOYSTICK_DEADZONE * ANALOG_MAX / 100;
 
   return (abs(center - raw) < deadzone) ? center : raw;
 }
 
 /**
  * @brief Reads X-axis joystick position with optional flipping.
  */
 int getJoyX() {
   #if JOYSTICK_BLANK
     return ANALOG_MAX / 2;
   #elif JOY_FLIP_X
     return ANALOG_MAX - analogReadDeadzone(PIN_JOY_X);
   #else
     return analogReadDeadzone(PIN_JOY_X);
   #endif
 }
 
 /**
  * @brief Reads Y-axis joystick position with optional flipping.
  */
 int getJoyY() {
   #if JOYSTICK_BLANK
     return ANALOG_MAX / 2;
   #elif JOY_FLIP_Y
     return ANALOG_MAX - analogReadDeadzone(PIN_JOY_Y);
   #else
     return analogReadDeadzone(PIN_JOY_Y);
   #endif
 }
 
 /**
  * @brief Reads the state of a digital button (active low).
  * @param pin Digital input pin
  * @return true if pressed, false otherwise
  */
 bool getButton(byte pin) {
   return digitalRead(pin) != HIGH;
 }
 