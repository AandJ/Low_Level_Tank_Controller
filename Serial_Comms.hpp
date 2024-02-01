/*******************************************************************************
This header defines Serial_Comms class which has all the functions for controlling a 
Serial_Comms, the serial object is kept private and therefore can only be accessed
through function of the Serial_Comms class.
*******************************************************************************/
#include "mbed.h"
#include<string>

// Use generic to set number of rows in table
#define Com_Protocol_Length 5
#define Com_Buffer_Length Com_Protocol_Length*7


// Define 8 bit colours for printing to Serial_Comms 
#define ColourRED    9
#define ColourAMBER  166
#define ColourGREEN  118
#define ColourBLUE   12
#define ColourWHITE  255
#define ColourPURPLE 5

#ifndef Define_ONCE_Serial_Comms
#define Define_ONCE_Serial_Comms
    typedef struct{
        char buffer_pointer;            // stores current point in Serial_Comms input line
        char Serial_Comms_buffer[100]; 
    }Serial_Comms_Data;

    // Class Serial_Comms expects tx and rx pins and is used for controlling a serialy conected Serial_Comms
    class Serial_Comms {
        public:
            Serial_Comms(PinName tx, PinName rx, const std::function<void(int)> & Recieved_Command_Handler) : pc(tx, rx), Recieved_Command_Handler_(Recieved_Command_Handler)
            {    
                Serial_Comms_Thread.start(callback(this, &Serial_Comms::Serial_Comms_Thread_main));      
            }

            void init(void);                                // Initialise Serial_Comms and start all timers/interupts
            void Print(std::string string); // Queues the recieved string to be printed when Comms next available


        private:
            // define serial objects and function needed to change its parameters
            UnbufferedSerial pc;                              // Serial object to be used for Serial_Comms
            // Mutex Serial_Mutex;
            Thread Serial_Comms_Thread;
            void Serial_Comms_Thread_main(void);

            Mail<Serial_Comms_Data, 32>Serial_Comms_mailbox;   

            //  Function needed to handle command inputs from Serial_Comms
            void Input_Handler(void);       // ISR handler for whenever a charecter is input to Serial_Comms
            void checkKEY(void);            // Functrion gets charecter from Serial_Comms input
            void Decode_Command(void);
            void Send(void);

            char buffer_pointer_In;                 // stores current point in Serial_Comms input line
            char Serial_Comms_buffer_In[30];       // Buffer to store command

            std::function<void(int)> Recieved_Command_Handler_;
    };
    
#endif

float strTOflt(char ary[4]);
int strTOint(char ary[3]);
std::string Char_Buffer_To_STD_String(char* array, int len);

