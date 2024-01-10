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
DigitalOut led_1(LED1);
// DigitalOut led_2(LED2);

#define ENA1 D13
#define ENA2 D12
#define IN1 D11
#define IN2 D10
#define IN3 D9
#define IN4 D8

/********************************************************************************/


/********************************************************************************/
/************************ Thread and Event que definitions***********************/
/********************************************************************************/
EventQueue EventQueue_1(32 * EVENTS_EVENT_SIZE);    //Event queue used for SPI events  
/********************************************************************************/



/********************************************************************************/
/************ Create objects and threads for attached peripherals ***************/
/********************************************************************************/

// // Create Object of serial communication class with Set USBTX and USBRX pins
// // This handles the Serial Connection with the PC in debug and with the Pi in Release
void command_Handler(int Data); // Function called to handle data recived from PC object
Serial_Comms PC(USBTX, USBRX, command_Handler);

/********************************************************************************/
  
// Motor_Control motor( ENA1, IN1, IN2, ENA2, IN3, IN4);



// // Command called to handle recieved USB data and pass commands on to motors
void command_Handler(int Data)
{
    // in final version int is 32 bits first 4 bits are direction commands, then 24 bits for the speed commands (12 bit command per motor), and the last four bits are a checksum
    char Direction_Commands = ( (Data & 0xF0000000) >> 28 );
    short Right_Speed_Command = ( (Data & 0x0FFF0000) >> 16 );
    short Left_Speed_Command = ( (Data & 0x0000FFF0) >>  4 );
    char Misc = ( Data & 0x0000000F );


    // motor.Left_Motor_Direction = (Direction_Commands & 0x3);
    // motor.Right_Motor_Direction = ((Direction_Commands >> 2) & 0x3) ;


    // motor.Right_Motor_Speed =  float(Right_Speed_Command) / 4095;
    // motor.Left_Motor_Speed = float(Left_Speed_Command) / 4095;

    return;

}


// // Create BSPSensors object to read sensors at rate specified in Hz, handles collection of data from Gyro, Accellerometer, Magnometer
// BSPSensors_Class BSPSensors(50);

// Create SPI_LCD_Class object to allow printing debug information to onboard LCD Display
// SPI_LCD_Class LCD(D10, D11, D12, D13, &EventQueue_1); //CS, Mosi, Miso, SCLK, and a pointer to the event que to handle events

/********************************************************************************/
  





/********************************************************************************/
/************************* Primary thread main loop *****************************/
/********************************************************************************/

int main()
{
    while(1) {
         // while(1)
        // {
        //     EventQueue_1.dispatch_forever();
        // }   



        //  debug 
        // BSPSensor_Data_Struct Sensor_Data;
        // Sensor_Data = BSPSensors.get_sensor_data();

        // char buffer[250];
        // int len = sprintf(buffer, "X - %d\n", Sensor_Data.AccDataXYZ[0]); //, Sensor_Data.AccDataXYZ[1], Sensor_Data.AccDataXYZ[2]);      
        // std::string S;
        // for (int i = 0; i < len; i++) {
        //     S = S + buffer[i];
        // }
        // PC.Print(S);       

        // // ThisThread::sleep_for(200);

        // int len2 = sprintf(buffer, "Y - %d\n", Sensor_Data.AccDataXYZ[1]); //, Sensor_Data.AccDataXYZ[1], Sensor_Data.AccDataXYZ[2]);      
        // std::string S2;
        // for (int i = 0; i < len2; i++) {
        //     S2 = S2 + buffer[i];
        // }
        // PC.Print(S2);    

        // // ThisThread::sleep_for(200);

        // int len3 = sprintf(buffer, "Z - %d\n", Sensor_Data.AccDataXYZ[2]); //, Sensor_Data.AccDataXYZ[1], Sensor_Data.AccDataXYZ[2]);      
        // std::string S3;
        // for (int i = 0; i < len3; i++) {
        //     S3 = S3 + buffer[i];
        // }
        // PC.Print(S3);    

        // // PC.Print("Test line 3\n");    
        // // PC.Print("Test line 2\n");    


        // led_2 = !led_2;

        ThisThread::sleep_for(200);
    }
}

/********************************************************************************/


