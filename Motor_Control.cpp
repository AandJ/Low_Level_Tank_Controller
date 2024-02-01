#include "Motor_Control.hpp"



void Motor_Control::set_right_motor_speed(float Speed)
{
    Right_Motor_Speed = Speed;
}


void Motor_Control::set_left_motor_speed(float Speed)
{
    Left_Motor_Speed = Speed;
}


void Motor_Control::set_right_motor_direction(Motor_State State)
{
    switch(State)
    {
        case Brake:
            Right_Motor_Direction = brake;
            break;

        case Forward:
            Right_Motor_Direction = forward;
            break;

        case Backward:
            Right_Motor_Direction = backward;
            break;

        case Floating:
            Right_Motor_Direction = floating;
            break;
    }
}


void Motor_Control::set_left_motor_direction(Motor_State State)
{
    switch(State)
    {
        case Brake:
            Left_Motor_Direction = brake;
            break;

        case Forward:
            Left_Motor_Direction = forward;
            break;

        case Backward:
            Left_Motor_Direction = backward;
            break;

        case Floating:
            Left_Motor_Direction = floating;
            break;
    }
}

