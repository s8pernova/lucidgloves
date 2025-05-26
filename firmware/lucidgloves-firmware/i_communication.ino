/**
 * @file icommunication.ino
 * @brief Interface class for communication protocols used in the LucidGloves firmware.
 *
 * Any communication class (e.g., SerialCommunication, BTSerialCommunication) must inherit from
 * this interface and implement its methods.
 */

 class ICommunication {
  public:
    /**
     * @brief Returns whether the communication channel is initialized and ready.
     */
    virtual bool isOpen() = 0;

    /**
     * @brief Initializes the communication channel (e.g., begin Serial/Bluetooth).
     */
    virtual void start() = 0;

    /**
     * @brief Sends output data over the channel.
     * @param data Pointer to a character array containing the message to send.
     */
    virtual void output(char* data) = 0;

    /**
     * @brief Reads input data from the channel.
     * @param input Pointer to a character array where received data will be stored.
     * @return true if data was successfully read; false otherwise.
     */
    virtual bool readData(char* input) = 0;
};
