/*******************************************************************************
This header defines Serial_Comms class which has all the functions for controlling a 
Serial_Comms, the serial object is kept private and therefore can only be accessed
through function of the Serial_Comms class.
*******************************************************************************/
#include "mbed.h"
#include<string>

#define brake 0
#define forward 1
#define backward 2
#define floating 3

enum Motor_State { Brake, Forward, Backward, Floating };

#ifndef Define_ONCE_Motor_Control
#define Define_ONCE_Motor_Control
    // Struct for motor commands using the two wheel or tracked model
    typedef struct{
        int Right_Motor;            
        int Left_Motor; 
    }Tracked_Motor_Commands;

    // Struct for motor commands using the unicycle model
    typedef struct{
        int Linear_Velocity;            
        int Angular_Velocity; 
    }Unicycle_Motor_Commands;

    // Class Motor_Control enables control of two motors through a H-bridge with PWM speed control
    class Motor_Control {
        public:
            // Class Constructor
            Motor_Control(PinName RM_1, PinName RM_2, PinName RM_3, PinName LM_1, PinName LM_2, PinName LM_3) : Right_Motor_Speed(RM_1), Right_Motor_Direction(RM_2,RM_3), Left_Motor_Speed(LM_1), Left_Motor_Direction(LM_2,LM_3)
            {
                // Initialise the motors in brake with a speed of 0
                Right_Motor_Direction = brake;
                Left_Motor_Direction = brake;
                Right_Motor_Speed = 0;
                Left_Motor_Speed = 0;
            }

            // Public motor control functions
            void set_right_motor_speed(float Speed);
            void set_left_motor_speed(float Speed);
            void set_right_motor_direction(Motor_State State);
            void set_left_motor_direction(Motor_State State);

        private:

            // Connections to the motor H-Bridge, Pins are assigned through the constructor of the class
            BusOut Right_Motor_Direction;
            PwmOut Right_Motor_Speed;
            BusOut Left_Motor_Direction;
            PwmOut Left_Motor_Speed;

        };
    
#endif

