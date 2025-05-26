/**
 * @file gesture.ino
 * @brief Implements basic gesture recognition logic using flex sensor values.
 *
 * Each gesture function returns a boolean value based on how bent the fingers are.
 * Threshold for activation is typically halfway (ANALOG_MAX / 2).
 * 
 * Gestures:
 * - Grab: all four fingers (excluding thumb) are curled
 * - Pinch: index and thumb pressed together
 * - Trigger: index finger is curled (like pulling a trigger)
 */

 #include "AdvancedConfig.h"  // Ensure finger indices and ANALOG_MAX are defined

 /**
  * @brief Detects a grab gesture by averaging the flexion of all fingers except the thumb.
  * @param flexion Pointer to array of finger values: [thumb, index, middle, ring, pinky]
  * @return true if the average flexion of index–pinky exceeds half the analog range
  */
 bool grabGesture(int* flexion) {
   int avg = (flexion[INDEX_IND] + flexion[MIDDLE_IND] + flexion[RING_IND] + flexion[PINKY_IND]) / 4;
   return avg > (ANALOG_MAX / 2);
 }
 
 /**
  * @brief Detects a pinch gesture by averaging thumb and index flexion.
  * @param flexion Pointer to array of finger values
  * @return true if thumb and index are pressed together past the halfway point
  */
 bool pinchGesture(int* flexion) {
   int avg = (flexion[THUMB_IND] + flexion[INDEX_IND]) / 2;
   return avg > (ANALOG_MAX / 2);
 }
 
 /**
  * @brief Detects a trigger gesture using just the index finger.
  * @param flexion Pointer to array of finger values
  * @return true if index finger is curled more than halfway
  */
 bool triggerGesture(int* flexion) {
   return flexion[INDEX_IND] > (ANALOG_MAX / 2);
 }
 