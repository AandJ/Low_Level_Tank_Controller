/*******************************************************************************
This header defines Serial_Comms class which has all the functions for controlling a 
Serial_Comms, the serial object is kept private and therefore can only be accessed
through function of the Serial_Comms class.
*******************************************************************************/
#include "mbed.h"
#include<string>

#define brake 0
#define foward 1
#define backward 2
#define floating 3

enum Motor_Dir { spring, summer, autumn, winter };

#ifndef Define_ONCE_Motor_Control
#define Define_ONCE_Motor_Control
    typedef struct{
        int Right_Motor;            // stores current point in Serial_Comms input line
        int Left_Motor; 
    }Tracked_Motor_Commands;

    typedef struct{
        int Linear_Velocity;            // stores current point in Serial_Comms input line
        int Angular_Velocity; 
    }Unicycle_Motor_Commands;

    // Class Serial_Comms expects tx and rx pins and is used for controlling a serialy conected Serial_Comms
    class Motor_Control {
        public:
            Motor_Control(PinName ENA1, PinName IN1, PinName IN2, PinName ENA2, PinName IN3, PinName IN4) : Right_Motor_Speed(ENA1), Right_Motor_Direction(IN1,IN2), Left_Motor_Speed(ENA2), Left_Motor_Direction(IN3,IN4)
            {
                Right_Motor_Direction = brake;
                Left_Motor_Direction = brake;
                Right_Motor_Speed = 0;
                Left_Motor_Speed = 0;
            }

            BusOut Right_Motor_Direction;
            PwmOut Right_Motor_Speed;

            BusOut Left_Motor_Direction;
            PwmOut Left_Motor_Speed;

        private:
        };
    
#endif

