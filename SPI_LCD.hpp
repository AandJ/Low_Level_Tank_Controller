#include "mbed.h"
#include<string>

#define ENABLE 0x08
#define COMMAND_MODE 0x00
#define DATA_MODE 0x04

#define line_2 0xc0

#ifndef Define_ONCE_SPI_LCD_Class
#define Define_ONCE_SPI_LCD_Class
    // data structure used for mail boxes
    typedef struct{
        std::string data;
        int cmd;
    }LCD_Data;

    // Class Serial_Comms expects tx and rx pins and is used for controlling a serialy conected Serial_Comms
    class SPI_LCD_Class {
        public:
            SPI_LCD_Class(PinName cs, PinName mosi, PinName miso, PinName sclk, EventQueue* Event_Queue) : CS(cs), ser_port(mosi,miso,sclk),  LCD_Queue(Event_Queue)
            { 
                // LCD_Thread.start(callback(this, &SPI_LCD_Class::LCD_Thread_main)); //start event queue thread
                CS=1;
                init();
            } //Constructor for class, defines SPI pins and (Outdated - starts event queue thread)

            void clr(void);
            void Send_Data(std::string data);
            void Send_CMD(int cmd);

        private:
            // SPI definitions for comms to LCD
            DigitalOut CS;
            SPI ser_port;

            // // Thread definitions
            // Mutex LCD_Mutex;
            // Thread LCD_Thread;
            // void LCD_Thread_main(void);

            // Event queue and mail box definitions 
            Mail<LCD_Data, 32>SPI_LCD_mail;   
            EventQueue *LCD_Queue;  //Stores a pointer to the event queue which will handle LCD events


            // Private Control functions for LCD
            void init(void);
            void print(void);
            void write_cmd(void);
            void write_cmd_raw(int cmd);
            void shift_out(int data);
            void write_data(char c);
            void write_4bit(int data, int mode);
    };
    
#endif
