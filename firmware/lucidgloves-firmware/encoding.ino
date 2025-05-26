/**
 * @file encoding.ino
 * @brief Encodes glove input data into a string for serial/Bluetooth transmission,
 * and decodes incoming haptic feedback commands. Supports legacy and labeled (alpha) modes.
 *
 * Alpha encoding uses human-readable identifiers (A–P), while legacy encoding uses '&'-delimited values.
 */

 #if ENCODING == ENCODING_LEGACY

 /**
  * @brief Encodes glove input values into a legacy format string.
  * Format: val1&val2&...&val13\n
  */
 char* encode(int* flexion, int joyX, int joyY, bool joyClick, bool triggerButton,
              bool aButton, bool bButton, bool grab, bool pinch, bool calib, bool menu) {
   static char stringToEncode[75];
 
   sprintf(stringToEncode, "%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d&%d\n", 
           flexion[0], flexion[1], flexion[2], flexion[3], flexion[4],
           joyX, joyY, joyClick,
           triggerButton, aButton, bButton, grab, pinch);
   return stringToEncode;
 }
 
 /**
  * @brief Decodes '&'-separated integer values into the hapticLimits array.
  * Expects 5 values corresponding to each finger.
  */
 void decodeData(char* stringToDecode, int* hapticLimits) {
   byte index = 0;
   char* ptr = strtok(stringToDecode, "&");
   while (ptr != NULL && index < 5) {
     hapticLimits[index++] = atoi(ptr);
     ptr = strtok(NULL, "&");
   }
 }
 
 #endif // ENCODING_LEGACY
 
 
 #if ENCODING == ENCODE_ALPHA
 
 /**
  * @brief Encodes glove input values into labeled format using A–P identifiers.
  * Each value is tagged with a prefix like A420, B500, etc.
  */
 char* encode(int* flexion, int joyX, int joyY, bool joyClick, bool triggerButton,
              bool aButton, bool bButton, bool grab, bool pinch, bool calib, bool menu) {
   static char stringToEncode[75];
   int trigger = (flexion[1] > ANALOG_MAX / 2) ? (flexion[1] - ANALOG_MAX / 2) * 2 : 0;
 
   sprintf(stringToEncode, "A%dB%dC%dD%dE%dF%dG%dP%d%s%s%s%s%s%s%s%s\n", 
           flexion[0], flexion[1], flexion[2], flexion[3], flexion[4],
           joyX, joyY, trigger,
           joyClick     ? "H" : "",
           triggerButton? "I" : "",
           aButton      ? "J" : "",
           bButton      ? "K" : "",
           grab         ? "L" : "",
           pinch        ? "M" : "",
           menu         ? "N" : "",
           calib        ? "O" : "");
   return stringToEncode;
 }
 
 /**
  * @brief Extracts and converts labeled values from the input string.
  * Used for decoding servo/haptic finger force values.
  */
 void decodeData(char* stringToDecode, int* hapticLimits) {
   hapticLimits[0] = getArgument(stringToDecode, 'A'); // thumb
   hapticLimits[1] = getArgument(stringToDecode, 'B'); // index
   hapticLimits[2] = getArgument(stringToDecode, 'C'); // middle
   hapticLimits[3] = getArgument(stringToDecode, 'D'); // ring
   hapticLimits[4] = getArgument(stringToDecode, 'E'); // pinky
 }
 
 /**
  * @brief Retrieves an integer that follows a specific command letter in the string.
  * @param stringToDecode The encoded string
  * @param command The letter to search for (e.g., 'A', 'B', 'C', etc.)
  * @return The integer value after the command, or -1 if not found.
  */
 int getArgument(char* stringToDecode, char command) {
   char* start = strchr(stringToDecode, command);
   return (start == NULL) ? -1 : atoi(start + 1);
 }
 
 #endif // ENCODE_ALPHA
 