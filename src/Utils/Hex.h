#ifndef HEX_H_
#define HEX_H_

//* Hex
/**
 * \brief Utility to print char arrays/pointers as hexadecimal values.
 */
class Hex
{
public:
    /*!
     * \brief Prints char array as hexadecimal values.
     *
     * For printing the char array as hexadecimal, it uses the std::hex option.
     * The char array is casted and printed char by char.
     *
     * \param message        Char array with message to be displayed.
     * \param messageLength  Length of the char array.
     */
    static void Print(char message[], int messageLength)
    {
        std::cout << std::hex;
        for(int i = 0; i < messageLength; i++)
        {
            std::cout << (unsigned int)(unsigned char)message[i] << " ";
        }
        std::cout << std::dec << std::endl;
    }

    /*!
     * \brief Prints char pointer as hexadecimal values.
     *
     * Casts the pointer to a char array and uses the char array version of the function.
     *
     * \param data           Char pointer with message to be displayed.
     */
    static void Print(char* data)
    {
        int dataLength = strlen(data);
        char dataResponse[dataLength];
        memcpy(&dataResponse, &data, dataLength);
        Hex::Print(dataResponse, dataLength);
    }
};

#endif // HEX_H_
