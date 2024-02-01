#include "DigitalOut.h"
#include "mbed.h"
#include <string>

#include "BSPSensors.hpp"
#include "Serial_Comms.hpp"
#include "SPI_LCD.hpp"
#include "Motor_Control.hpp"

/********************************************************************************/
/************************ Debug and Tests definitions ***************************/
/********************************************************************************/
// // Debug LEDs
// DigitalOut led_1(LED1);
// DigitalOut led_2(LED2);

#define ENA1 D13
#define ENA2 D12
#define IN1 D11
#define IN2 D10
#define IN3 D9
#define IN4 D8

/********************************************************************************/



/********************************************************************************/
/********************** Thread and Event queue definitions **********************/
/********************************************************************************/
// EventQueue EventQueue_1(32 * EVENTS_EVENT_SIZE);    //Event queue used for SPI events such as pritning to the LCD
/********************************************************************************/



/********************************************************************************/
/************ Create objects and threads for attached peripherals ***************/
/********************************************************************************/

// Create Object of serial communication class with Set USBTX and USBRX pins
// This handles the Serial Connection with the PC in debug and with the Pi in Release
void command_Handler(int Data); // Function called to handle data recived from PC object
Serial_Comms PC(USBTX, USBRX, 9600, command_Handler);
  
//
Motor_Control motor( ENA1, IN1, IN2, ENA2, IN3, IN4);
Motor_State Determine_State(char cmd);


// // Create BSPSensors object to read sensors at rate specified in Hz, handles collection of data from Gyro, Accellerometer, Magnometer
// BSPSensors_Class BSPSensors(50);

// Create SPI_LCD_Class object to allow printing debug information to onboard LCD Display
// SPI_LCD_Class LCD(D10, D11, D12, D13, &EventQueue_1); //CS, Mosi, Miso, SCLK, and a pointer to the event que to handle events


/********************************************************************************/
  

// Command called to handle recieved USB data and pass commands on to motors
void command_Handler(int Data)
{
    // Input int command is 32 bits first 4 bits are direction commands, then 24 bits for the speed commands (12-bit command per motor), and the last four bits will be a checksum
    char Direction_Commands = ( (Data & 0xF0000000) >> 28 );
    short Right_Speed_Command = ( (Data & 0x0FFF0000) >> 16 );
    short Left_Speed_Command = ( (Data & 0x0000FFF0) >>  4 );
    char Checksum = ( Data & 0x0000000F );  // Currently not used

    Motor_State Right_Motor_State = Determine_State(Direction_Commands & 0x3);
    Motor_State Left_Motor_State = Determine_State((Direction_Commands >> 2) & 0x3);

    motor.set_right_motor_direction(Right_Motor_State);
    motor.set_left_motor_direction(Left_Motor_State);

    motor.set_right_motor_speed(float(Right_Speed_Command) / 4095);
    motor.set_left_motor_speed(float(Left_Speed_Command) / 4095);

    return;
}


Motor_State Determine_State(char cmd)
{
    Motor_State motor_state;

    switch (cmd)
    {
        case brake:
            motor_state = Brake;
            break;

        case forward:
            motor_state = Forward;
            break;

        case backward:
            motor_state = Backward;
            break;

        case floating:
            motor_state = Floating;
            break;
    }

    return motor_state;
}


/********************************************************************************/
/************************* Primary thread main loop *****************************/
/********************************************************************************/

int main()
{
    while(1)
    {
        // Disptach the event queue for LCD commands
        // EventQueue_1.dispatch_forever();
    }   
}

/********************************************************************************/

