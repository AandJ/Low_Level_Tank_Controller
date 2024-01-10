#include "mbed.h"

// Sensors drivers present in the BSP library
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_magneto.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"

#define Pi 3.141592654

// data structure to hold sensor readings
typedef struct{
    float sensor_value;
    float heading;
    float GyroDataXYZ[3];
    int16_t AccDataXYZ[3];
}BSPSensor_Data_Struct;

// BSP sensors class, takes one arg (float) to define update freq in Hz
class BSPSensors_Class {
    public:
        BSPSensors_Class(float update_rate);
        
        BSPSensor_Data_Struct get_sensor_data(void);
    private:
        Ticker data_aquisition_ticker;      // Ticker object to update stored sensor data at specified rate
        float Sensor_Update_Freq;           // Float stores the current update rate of the sensors
        std::chrono::microseconds Sensor_Delay;

        void update_Data(void);             // Function attached to ticker updates stored sensor values at the specified rate

        BSPSensor_Data_Struct BSPSensor_Data;

};



