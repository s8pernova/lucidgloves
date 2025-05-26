/**
 * @file serial_communication.ino
 * @brief Implements USB serial communication for LucidGloves using the ICommunication interface.
 *
 * This class handles initialization, output, and input over standard Serial.
 * Intended for use when COMMUNICATION is set to COMM_SERIAL.
 */

 class SerialCommunication : public ICommunication {
  private:
    bool m_isOpen;

  public:
    /**
     * @brief Constructor. Initializes internal state.
     */
    SerialCommunication() {
      m_isOpen = false;
    }

    /**
     * @brief Returns whether Serial has been started.
     */
    bool isOpen() {
      return m_isOpen;
    }

    /**
     * @brief Initializes Serial with the configured baud rate.
     */
    void start() {
      Serial.begin(SERIAL_BAUD_RATE);
      m_isOpen = true;
    }

    /**
     * @brief Sends a null-terminated string over Serial.
     * @param data Pointer to character array to send
     */
    void output(char* data) {
      Serial.print(data);
      Serial.flush();  // Waits until all data is sent
    }

    /**
     * @brief Reads a newline-terminated message into the given buffer.
     * @param input Buffer to store received characters (should be at least 100 bytes)
     * @return true if any valid input was received, false otherwise
     */
    bool readData(char* input) {
      byte size = Serial.readBytesUntil('\n', input, 100);
      input[size] = '\0';  // Proper null terminator (not NULL, which is a pointer!)
      return strlen(input) > 0;
    }
};
