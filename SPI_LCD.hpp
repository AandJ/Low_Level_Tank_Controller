#include "mbed.h"
#include<string>

#define ENABLE 0x08
#define COMMAND_MODE 0x00
#define DATA_MODE 0x04

#define line_2 0xc0

#ifndef Define_ONCE_SPI_LCD_Class
#define Define_ONCE_SPI_LCD_Class
    // data structure used for LCD mail boxes
    typedef struct{
        std::string data;
        int cmd;
    }LCD_Data;

    // Class SPI_LCD_Class enables thread safe interaction with an SPI controlled LCD
    // This class expects the SPI pins to be defined as well as an event queue which is dispatched within another thread
    // None of the SPI functions are blocking allowing the LCD to be handled in a thread with runs other peripherals.
    class SPI_LCD_Class {
        public:
            // Class Constructor expects the SPI pins (CS, MOSI, MISO & SCLK) to be defined along with an event queue to dispatch the queued commands
            SPI_LCD_Class(PinName cs, PinName mosi, PinName miso, PinName sclk, EventQueue* Event_Queue) : CS(cs), spi_port(mosi,miso,sclk),  LCD_Queue(Event_Queue)
            { 
                CS=1;

                init(); // Initialise the LCD
            } 

            void clr(void); // queues the command to clear the LCD
            void Send_Data(std::string data); // Public function - Queues data to be printed on the LCD
            void Send_CMD(int cmd); // Public function - Queues a command for the LCD

        private:
            // SPI definitions for comms to LCD, Pins assigned within the constructor of the class
            DigitalOut CS; // Chip select line for the SPI LCD
            SPI spi_port;  // SPI pins for comms to LCD


            // Event queue and mail box definitions 
            Mail<LCD_Data, 32>SPI_LCD_mail;   
            EventQueue *LCD_Queue;  //Stores a pointer to the event queue which will handle LCD events


            // Private Control functions for LCD
            void init(void);
            void shift_out(int data);
            void write_4bit(int data, int mode);


            void print(void);     // Private function, is queued on the provided eventqueue when another thread add data to the print queue
            void write_cmd(void); // Private function, is queued on the provided eventqueue when another thread gives a command for the LCD
    };
    
#endif
