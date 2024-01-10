#include "BSPSensors.hpp"


/*******************************************************************************
    Class Constructor initialises all sensors and starts a ticker to update 
    sensor readings at the frequency defined by the provided argument
*******************************************************************************/
BSPSensors_Class::BSPSensors_Class(float update_freq) : Sensor_Update_Freq(update_freq){
    BSP_TSENSOR_Init();
    BSP_HSENSOR_Init();
    BSP_PSENSOR_Init();

    BSP_MAGNETO_Init();
    BSP_GYRO_Init();
    BSP_ACCELERO_Init();

    BSPSensor_Data.sensor_value = 0;
    BSPSensor_Data.heading = 0;
    BSPSensor_Data.GyroDataXYZ[3] = {0};
    BSPSensor_Data.AccDataXYZ[3] = {0};

    // Collect initial data
    update_Data();
    data_aquisition_ticker.attach(callback(this, &BSPSensors_Class::update_Data), (1 / Sensor_Update_Freq)); // the address of the function to be attached (flip) and the interval (2 seconds)
}




/*******************************************************************************
 Private function to read the current sensors values and update the stored values
*******************************************************************************/
void BSPSensors_Class::update_Data(void)
{
    float sensor_value = 0.0f;
    float heading;
    float GyroDataXYZ[3];
    int16_t AccDataXYZ[3];

    BSP_GYRO_GetXYZ(GyroDataXYZ);
    BSP_ACCELERO_AccGetXYZ(AccDataXYZ);

    // Get data from Magneto sensor and convert to a heading value
    int16_t MagnetoDataXYZ[3];
    BSP_MAGNETO_GetXYZ(MagnetoDataXYZ);

    // Convert Magneto Data into compass heading, Does not account for tilt
    // Calculate the angle of the vector y,x
    float local_heading = (atan2(MagnetoDataXYZ[2], MagnetoDataXYZ[1]) * 180) / Pi; // 1, 0

    // Normalize to 0-360
    if (local_heading < 0)
    {
        heading = 360 + local_heading;
    }

    // Update the stored BSP Sensor Data
    BSPSensor_Data.sensor_value = sensor_value;
    BSPSensor_Data.heading = heading;
    memcpy(BSPSensor_Data.GyroDataXYZ, GyroDataXYZ, sizeof(GyroDataXYZ));
    memcpy(BSPSensor_Data.AccDataXYZ, AccDataXYZ, sizeof(AccDataXYZ));

    return;
}


/*******************************************************************************
 Public function to read the current sensors values  
*******************************************************************************/
BSPSensor_Data_Struct BSPSensors_Class::get_sensor_data(void){
    return BSPSensor_Data;
}
/********************************************************************************/
