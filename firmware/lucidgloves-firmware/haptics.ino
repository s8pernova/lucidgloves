/**
 * @file haptics.ino
 * @brief Controls force feedback haptics using servo motors for each finger.
 *
 * Includes functions to initialize servos, scale haptic force data to servo angles,
 * and write the angles to the motors. Supports both static and dynamic scaling.
 */

 #if USING_FORCE_FEEDBACK

 #if defined(ESP32)
   #include "ESP32Servo.h"
 #else
   #include "Servo.h"
 #endif
 
 // === Servo instances for each finger ===
 Servo pinkyServo;
 Servo ringServo;
 Servo middleServo;
 Servo indexServo;
 Servo thumbServo;
 
 #define NUM_FINGERS 5  // for loops
 
 /**
  * @brief Attaches each servo to its corresponding motor control pin.
  */
 void setupServoHaptics() {
   pinkyServo.attach(PIN_PINKY_MOTOR);
   ringServo.attach(PIN_RING_MOTOR);
   middleServo.attach(PIN_MIDDLE_MOTOR);
   indexServo.attach(PIN_INDEX_MOTOR);
   thumbServo.attach(PIN_THUMB_MOTOR);
 }
 
 /**
  * @brief Converts haptic intensity (0–1000) to a servo angle (0–180°) using static scaling.
  *        Inverts the direction to match "curl = increase force = lower angle".
  * @param hapticLimits Array of input force values (0–1000)
  * @param scaledLimits Output array of converted servo angles
  */
 void scaleLimits(int* hapticLimits, float* scaledLimits) {
   for (int i = 0; i < NUM_FINGERS; i++) {
     scaledLimits[i] = 180.0f - (hapticLimits[i] / 1000.0f * 180.0f);
   }
 }
 
 /**
  * @brief Applies dynamic scaling (non-inverted) from haptic force to servo angle.
  *        This assumes a linear 0–180° mapping.
  *        NOTE: Replace this logic with min/max calibration-aware scaling for real use.
  * @param hapticLimits Array of input force values (0–1000)
  * @param scaledLimits Output array of converted servo angles
  */
 void dynScaleLimits(int* hapticLimits, float* scaledLimits) {
   for (int i = 0; i < NUM_FINGERS; i++) {
     scaledLimits[i] = hapticLimits[i] / 1000.0f * 180.0f;
   }
 }
 
 /**
  * @brief Writes the scaled servo angles to each finger's motor.
  * @param hapticLimits Array of 5 values (one per finger), expected range: 0–1000
  */
 void writeServoHaptics(int* hapticLimits) {
   float scaledLimits[NUM_FINGERS];
   scaleLimits(hapticLimits, scaledLimits);  // or dynScaleLimits(...) if enabled
 
   if (hapticLimits[THUMB_IND]  >= 0) thumbServo.write((int)scaledLimits[THUMB_IND]);
   if (hapticLimits[INDEX_IND]  >= 0) indexServo.write((int)scaledLimits[INDEX_IND]);
   if (hapticLimits[MIDDLE_IND] >= 0) middleServo.write((int)scaledLimits[MIDDLE_IND]);
   if (hapticLimits[RING_IND]   >= 0) ringServo.write((int)scaledLimits[RING_IND]);
   if (hapticLimits[PINKY_IND]  >= 0) pinkyServo.write((int)scaledLimits[PINKY_IND]);
 }
 
 #endif  // USING_FORCE_FEEDBACK
 