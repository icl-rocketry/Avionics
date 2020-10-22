//command id and definition. find description on excel doc

#ifndef COMMANDS_H
#define COMMANDS_H

enum class COMMANDS {
    Launch = 1,
    Reset = 2,
    Abort = 3,
    Zero_Sensors = 4,
    Start_Logging = 5,
    Stop_Logging = 6,
    Play_Song = 7,
    Clear_Flash = 10,
    Clear_SD = 11,
    Print_Flash = 12,
    Print_Sd = 13,
    Estimator_Output = 20,
    Raw_Sensor_Output = 21,
    Detailed_All_Sensors = 50,
    Detailed_Accel = 51,
    Detailed_Gyro = 52,
    Detailed_Mag = 53,
    Detailed_Baro = 54,
    Detailed_GPS = 55,
    Detailed_Battery = 56,
    Detailed_Throttle = 57,
    Callibrate_Accel = 58,
    Callibrate_Mag = 59,
    Callibrate_Gyro = 60,
    Callibrate_Baro = 61,
    Enter_USBMode = 100,
    Enter_Groundstation = 101,
    Enter_Countdown = 102,
    Enter_Flight = 103,
    Enter_Recovery = 104,
    Set_Throttle = 200,
    Fire_pyro_1 = 201,
    Fire_pyro_2 = 202,
};



#endif