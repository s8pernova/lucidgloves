/**
 * @file serial_bt_communication.ino
 * @brief Implements Bluetooth serial communication using ESP32's BluetoothSerial library.
 *
 * This class is only compiled if COMMUNICATION is set to COMM_BTSERIAL.
 * It inherits from the ICommunication interface and provides start, send, and receive methods.
 */

 #if COMMUNICATION == COMM_BTSERIAL

 #include "BluetoothSerial.h"
 
 class BTSerialCommunication : public ICommunication {
   private:
     bool m_isOpen;
     BluetoothSerial m_SerialBT;
 
   public:
     /**
      * @brief Constructor, initializes connection state.
      */
     BTSerialCommunication() {
       m_isOpen = false;
     }
 
     /**
      * @brief Returns whether Bluetooth is currently initialized.
      */
     bool isOpen() {
       return m_isOpen;
     }
 
     /**
      * @brief Starts USB Serial for debugging and Bluetooth Serial for communication.
      */
     void start() {
       Serial.begin(115200);  // Debug USB serial output
       m_SerialBT.begin(BTSERIAL_DEVICE_NAME);  // Start Bluetooth with configured name
       Serial.println("The device started, now you can pair it with Bluetooth!");
       m_isOpen = true;
     }
 
     /**
      * @brief Sends a string message via Bluetooth.
      * @param data Pointer to a null-terminated character array.
      */
     void output(char* data) {
       m_SerialBT.print(data);
     }
 
     /**
      * @brief Reads a newline-terminated message from Bluetooth input.
      * @param input Character buffer to store received data (should be at least 100 bytes)
      * @return true if valid data was received; false otherwise.
      */
     bool readData(char* input) {
       String message = m_SerialBT.readStringUntil('\n');
       if (message.length() == 0) return false;
 
       strncpy(input, message.c_str(), 100);
       input[99] = '\0';  // Ensure null termination
       return true;
     }
 };
 
 #endif  // COMM_BTSERIAL
 