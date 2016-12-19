#ifndef HEX_H_
#define HEX_H_

class Hex
{
public:
    static void Print(char message[], int messageLength)
    {
        std::cout << std::hex;
        for(int i = 0; i < messageLength; i++)
        {
            std::cout << (unsigned int)(unsigned char)message[i] << " ";
        }
        std::cout << std::dec << std::endl;
    }

    static void Print(char* data)
    {
        int dataLength = strlen(data);
        char dataResponse[dataLength];
        memcpy(&dataResponse, &data, dataLength);
        Hex::Print(dataResponse, dataLength);
    }
};

#endif // HEX_H_
