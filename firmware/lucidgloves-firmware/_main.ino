/**
 * @file _main.ino
 * @brief Core firmware loop for LucidGloves. Manages communication, sensor reading,
 * gesture interpretation, calibration, and optional force feedback response.
 *
 * This sketch initializes communication and sensor systems, processes gesture input
 * in a loop, and optionally sends haptic feedback to servos. Supports both USB serial
 * and Bluetooth communication, and includes calibration features.
 */

 #define ALWAYS_CALIBRATING (CALIBRATION_LOOPS == -1)

 // Safety check for incompatible calibration pin assignment on USB
 #define CALIB_OVERRIDE false
 #if USING_CALIB_PIN && COMMUNICATION == COMM_SERIAL && PIN_CALIB == 0 && !CALIB_OVERRIDE
   #error "You can't set your calibration pin to 0 over USB. Use the BOOT button when using Bluetooth, or set CALIB_OVERRIDE to true."
 #endif
 
 ICommunication* comm;
 int loops = 0;
 
 /**
  * @brief Sets up communication, inputs, and (optional) haptic feedback servos.
  */
 void setup() {
   // Initialize communication method based on configuration
   #if COMMUNICATION == COMM_SERIAL
     comm = new SerialCommunication();
   #elif COMMUNICATION == COMM_BTSERIAL
     comm = new BTSerialCommunication();
   #endif  
   comm->start();
 
   // Set up button and flex sensor input pins
   setupInputs();
 
   // Initialize servos for haptic feedback (if enabled)
   #if USING_FORCE_FEEDBACK
     setupServoHaptics();  
   #endif
 }
 
 /**
  * @brief Main loop for collecting input, determining gestures, and sending data.
  * Also handles calibration logic and incoming haptic feedback commands.
  */
 void loop() {
   if (!comm->isOpen()) return;
 
   // --- Calibration Handling ---
   bool calibButton = false;
   #if USING_CALIB_PIN
     calibButton = getButton(PIN_CALIB) != INVERT_CALIB;
     if (calibButton) loops = 0;
   #endif
 
   bool calibrate = (loops < CALIBRATION_LOOPS) || ALWAYS_CALIBRATING;
   if (calibrate) loops++;
 
   // --- Read Input States ---
   int* fingerPos = getFingerPositions(calibrate, calibButton);
   bool joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;
   bool aButton   = getButton(PIN_A_BTN)   != INVERT_A;
   bool bButton   = getButton(PIN_B_BTN)   != INVERT_B;
   bool menuButton = getButton(PIN_MENU_BTN) != INVERT_MENU;
 
   // --- Determine Gesture States ---
   bool triggerButton =
     #if TRIGGER_GESTURE
       triggerGesture(fingerPos);
     #else
       getButton(PIN_TRIG_BTN) != INVERT_TRIGGER;
     #endif
 
   bool grabButton =
     #if GRAB_GESTURE
       grabGesture(fingerPos);
     #else
       getButton(PIN_GRAB_BTN) != INVERT_GRAB;
     #endif
 
   bool pinchButton =
     #if PINCH_GESTURE
       pinchGesture(fingerPos);
     #else
       getButton(PIN_PNCH_BTN) != INVERT_PINCH;
     #endif
 
   // --- Encode and Transmit Data ---
   comm->output(
     encode(
       fingerPos, getJoyX(), getJoyY(), joyButton,
       triggerButton, aButton, bButton,
       grabButton, pinchButton, calibButton, menuButton
     )
   );
 
   // --- Optional: Handle Force Feedback ---
   #if USING_FORCE_FEEDBACK
     char received[100];
     if (comm->readData(received)) {
       int hapticLimits[5];
       if (strlen(received) >= 10) {  // sanity check on input length
         decodeData(received, hapticLimits);
         writeServoHaptics(hapticLimits);
       }
     }
   #endif
 
   delay(LOOP_TIME);  // delay for loop pacing
 }